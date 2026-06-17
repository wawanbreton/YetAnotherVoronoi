#pragma once

#include <QGraphicsView>

class QGraphicsScene;

namespace yav
{

class Space2;
class Diagram;
class VoronoiQuadtreeNode;

} // namespace yav

class VoronoiGraphicsView : public QGraphicsView
{
    // Q_OBJECT

public:
    explicit VoronoiGraphicsView(QWidget* parent = nullptr);

    void setSpace(const yav::Space2& space);

    void setDiagram(const yav::Diagram& diagram);

    void setTree(const std::shared_ptr<yav::VoronoiQuadtreeNode>& root);

    void setTreeLeaves(const std::vector<std::shared_ptr<yav::VoronoiQuadtreeNode>>& leaves);

    void autoFit();

protected:
    virtual void wheelEvent(QWheelEvent* event) override;

    virtual void mousePressEvent(QMouseEvent* event) override;

    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
    void addTreeNode(const std::shared_ptr<yav::VoronoiQuadtreeNode>& node, const QColor& color, bool add_children = false);

    void zoomIn();

    void zoomOut();

    void resetZoom();

    qreal getActualZoom() const;

    qreal getScaleFactor() const;

    void zoom(qreal scale);

private:
    QGraphicsScene* const scene_;
    int min_zoom_;
    int max_zoom_;
};
