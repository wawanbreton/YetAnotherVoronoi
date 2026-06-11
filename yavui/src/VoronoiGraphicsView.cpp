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
#include <yav/space/Space2.h>
#include <yav/space/site/Edge2.h>
#include <yav/space/site/Vertex2.h>
#include <yav/voronoi/Cell.h>
#include <yav/voronoi/Diagram.h>


VoronoiGraphicsView::VoronoiGraphicsView(QWidget* parent)
    : min_zoom_(35)
    , max_zoom_(40)
    , scene_(new QGraphicsScene(this))
{
    resetZoom();
    setScene(scene_);
}

void VoronoiGraphicsView::setSpace(const yav::space::Space2& space)
{
    for (const yav::space::site::AbstractSite::Ptr site : space.sites())
    {
        if (const auto site_vertex2 = std::dynamic_pointer_cast<yav::space::site::Vertex2>(site))
        {
            QRectF rect(0, 0, 0.02, 0.02);
            rect.moveCenter(QPointF(site_vertex2->basePoint().get<0>(), site_vertex2->basePoint().get<1>()));
            scene_->addEllipse(rect, Qt::NoPen, QBrush("#e74c3c"));
        }
    }
}

void VoronoiGraphicsView::setDiagram(const yav::voronoi::Diagram& diagram)
{
    for (const yav::voronoi::Cell::Ptr& cell : diagram.cells())
    {
        for (const yav::geometry::Segment2& segment : cell->boundarySegments())
        {
            scene_->addLine(
                segment.first.get<0>(),
                segment.first.get<1>(),
                segment.second.get<0>(),
                segment.second.get<1>(),
                QPen(QColor("#2ecc71"), 0.01));
        }
    }
}

void VoronoiGraphicsView::autoFit()
{
    qDebug() << scene_->sceneRect();
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
        // We want to drag the scene with the middle button, and Qt implements it with the left button, so make it believe the left button
        // has been pressed;
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
