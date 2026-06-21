#include "yavui/VoronoiGraphicsView.h"

#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QWheelEvent>
#include <algorithm>
#include <qdebug.h>
#include <qnumeric.h>
#include <qpoint.h>
#include <qpointingdevice.h>
#include <qtransform.h>
#include <ranges>
#include <yav/generator/VoronoiQuadtreeNode.h>
#include <yav/space/Space2.h>
#include <yav/space/site/Edge2.h>
#include <yav/space/site/Vertex2.h>
#include <yav/voronoi/Cell.h>
#include <yav/voronoi/Diagram.h>

#include <boost/geometry/algorithms/centroid.hpp>
#include <spdlog/spdlog.h>


VoronoiGraphicsView::VoronoiGraphicsView(QWidget* parent)
    : min_zoom_(35)
    , max_zoom_(100)
    , scene_(new QGraphicsScene(this))
    , overlay_(scene_->createItemGroup({}))
{
    resetZoom();
    setScene(scene_);
    setMouseTracking(true);
    overlay_->setZValue(1.0);
}

void VoronoiGraphicsView::setSpace(const yav::Space2& space)
{
    for (const yav::AbstractSite::Ptr site : space.sites())
    {
        if (const std::shared_ptr<yav::Vertex2> site_vertex2 = std::dynamic_pointer_cast<yav::Vertex2>(site))
        {
            QRectF rect(0, 0, 0.01, 0.01);
            rect.moveCenter(QPointF(site_vertex2->basePoint().x(), site_vertex2->basePoint().y()));
            scene_->addEllipse(rect, Qt::NoPen, QBrush("#e74c3c"));
        }
    }
}

void VoronoiGraphicsView::setDiagram(const yav::Diagram& diagram)
{
    for (const yav::Segment2& segment : diagram.boundarySegments())
    {
        scene_->addLine(segment.first.x(), segment.first.y(), segment.second.x(), segment.second.y(), QPen(QColor("#2ecc71"), 0.0004));
    }
}

void VoronoiGraphicsView::setTree(const yav::VoronoiQuadtreeNode::Ptr& root)
{
    tree_root_ = root;

    constexpr bool add_children = true;
    addTreeNode(root, QColor("#ff8f00"), add_children);
}

void VoronoiGraphicsView::setTreeLeaves(const std::vector<yav::VoronoiQuadtreeNode::Ptr>& leaves)
{
    for (const yav::VoronoiQuadtreeNode::Ptr& leaf : leaves)
    {
        addTreeNode(leaf, QColor("#008fff"));
    }
}

QGraphicsItem* VoronoiGraphicsView::addTreeNode(
    const yav::VoronoiQuadtreeNode::Ptr& node,
    const QColor& color,
    bool add_children,
    const double pen_scale)
{
    QRectF rect(0, 0, node->width(), node->width());
    rect.moveCenter(QPointF(node->center().x(), node->center().y()));
    QGraphicsItem* item = scene_->addRect(rect, QPen(color, 0.0001 * pen_scale));

    if (add_children)
    {
        for (const yav::VoronoiQuadtreeNode::Ptr& child : node->children())
        {
            if (child)
            {
                addTreeNode(child, color, add_children);
            }
        }
    }

    return item;
}

void VoronoiGraphicsView::setOverlayNode(const std::shared_ptr<yav::VoronoiQuadtreeNode>& node)
{
    if (node == current_overlay_node_)
    {
        return;
    }

    while (! overlay_->childItems().empty())
    {
        delete overlay_->childItems().front();
    }

    current_overlay_node_ = node;

    spdlog::info("=================================");
    constexpr bool add_children = false;
    constexpr double pen_scale = 2.0;
    addTreeNode(current_overlay_node_, QColor("#ffd600"), add_children, pen_scale)->setParentItem(overlay_);

    for (const auto& [corner_index, closest_site] : node->cornerClosestSites() | std::views::enumerate)
    {
        if (closest_site.has_value())
        {
            const yav::Point2 corner = node->cornerAt(corner_index);
            const yav::Point2 site_pos = closest_site->site->basePoint();
            scene_->addLine(corner.x(), corner.y(), site_pos.x(), site_pos.y(), QPen(QColor("#ffd600"), 0.0002))->setParentItem(overlay_);
            spdlog::info("V-site to corner {}@{}: {} at distance {}", corner_index, corner, site_pos, closest_site->distance);
        }
    }

    const QPen edge_site_pen(QColor("#ff00d6"), 0.0002);
    for (const yav::FaceSite& edge_site : node->edgeSites())
    {
        const yav::Point2 site_pos = edge_site.site->basePoint();
        scene_
            ->addLine(
                edge_site.closest_segment_part.first.x(),
                edge_site.closest_segment_part.first.y(),
                site_pos.x(),
                site_pos.y(),
                edge_site_pen)
            ->setParentItem(overlay_);
        scene_
            ->addLine(
                edge_site.closest_segment_part.second.x(),
                edge_site.closest_segment_part.second.y(),
                site_pos.x(),
                site_pos.y(),
                edge_site_pen)
            ->setParentItem(overlay_);
        spdlog::info("F-site to edge {}: {}-{}", site_pos, edge_site.closest_segment_part.first, edge_site.closest_segment_part.second);
    }

    for (const yav::AbstractSite::Ptr& interior_site : node->interiorSites())
    {
        const yav::Point2 site_pos = interior_site->basePoint();
        const yav::Point2& center = node->center();
        scene_->addLine(center.x(), center.y(), site_pos.x(), site_pos.y(), QPen(QColor("#8fff00"), 0.0002))->setParentItem(overlay_);
    }
}

void VoronoiGraphicsView::autoFit()
{
    fitInView(scene_->sceneRect(), Qt::KeepAspectRatio);
}

void VoronoiGraphicsView::zoomIn()
{
    zoom(getActualZoom() + 1);
}

void VoronoiGraphicsView::zoomOut()
{
    zoom(getActualZoom() - 1);
}

void VoronoiGraphicsView::resetZoom()
{
    zoom(min_zoom_);
}

qreal VoronoiGraphicsView::getActualZoom() const
{
    return ((std::log(getScaleFactor() / 0.005) / 1.6) + 1.0) * 5.0;
}

qreal VoronoiGraphicsView::getScaleFactor() const
{
    return transform().m11();
}

void VoronoiGraphicsView::wheelEvent(QWheelEvent* event)
{
    zoom(getActualZoom() + event->angleDelta().y() / 200.0);
}

void VoronoiGraphicsView::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton)
    {
        // We want to drag the scene with the middle button, and Qt implements it with the left button, so make it believe the left
        // button has been pressed;
        setDragMode(QGraphicsView::ScrollHandDrag);
        QMouseEvent pseudo_event(
            QMouseEvent::MouseButtonPress,
            event->pos().toPointF(),
            event->globalPosition(),
            Qt::LeftButton,
            event->buttons(),
            event->modifiers(),
            qobject_cast<const QPointingDevice*>(event->device()));
        QGraphicsView::mousePressEvent(&pseudo_event);
        viewport()->setCursor(Qt::ClosedHandCursor);
    }
    else if (event->button() == Qt::RightButton)
    {
        if (current_overlay_node_ && current_overlay_node_->parent())
        {
            setOverlayNode(current_overlay_node_->parent());
        }
    }
    else if (event->button() == Qt::LeftButton)
    {
        if (current_overlay_node_)
        {
            const QPointF scene_pos = mapToScene(event->pos());
            const yav::Point2 scene_pos_point(scene_pos.x(), scene_pos.y());
            yav::VoronoiQuadtreeNode::Ptr new_overlay;
            for (yav::VoronoiQuadtreeNode::Ptr child : current_overlay_node_->children())
            {
                if (child && child->containsPoint(scene_pos_point))
                {
                    new_overlay = child;
                }
            }

            if (new_overlay)
            {
                setOverlayNode(new_overlay);
            }
        }
    }
    else
    {
        QGraphicsView::mousePressEvent(event);
    }
}

void VoronoiGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::MiddleButton)
    {
        QMouseEvent pseudo_event(
            QMouseEvent::MouseButtonRelease,
            event->pos().toPointF(),
            event->globalPosition(),
            Qt::LeftButton,
            event->buttons(),
            event->modifiers(),
            qobject_cast<const QPointingDevice*>(event->device()));
        QGraphicsView::mouseReleaseEvent(&pseudo_event);
        viewport()->setCursor(Qt::ArrowCursor);
        setDragMode(QGraphicsView::NoDrag);
    }
    else
    {
        QGraphicsView::mouseReleaseEvent(event);
    }
}

void VoronoiGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    if (tree_root_)
    {
        const QPointF scene_pos = mapToScene(event->pos());
        setOverlayNode(tree_root_->findDeepestChildAt(yav::Point2(scene_pos.x(), scene_pos.y())));
    }

    QGraphicsView::mouseMoveEvent(event);
}

void VoronoiGraphicsView::zoom(qreal zoom)
{
    zoom = std::clamp(zoom, static_cast<qreal>(min_zoom_), static_cast<qreal>(max_zoom_));

    if (! qFuzzyIsNull(zoom - getActualZoom()))
    {
        const QPoint cursor_global_pos = QCursor::pos();
        const QPoint cursor_local_pos = mapFromGlobal(cursor_global_pos);
        const QPointF cursor_scene_pos = mapToScene(cursor_local_pos);

        resetTransform();
        const qreal scale_value = std::exp(((zoom / 5.0) - 1) * 1.6) * 0.005;
        scale(scale_value, scale_value);

        const QPoint cursor_center_pos = rect().center();
        const QPointF center_scene_pos = mapToScene(cursor_center_pos);

        const QPointF new_cursor_scene_pos = mapToScene(cursor_local_pos);
        const QPointF delta = new_cursor_scene_pos - cursor_scene_pos;
        const QPointF new_scene_center = center_scene_pos - delta;

        centerOn(new_scene_center);
    }
}
