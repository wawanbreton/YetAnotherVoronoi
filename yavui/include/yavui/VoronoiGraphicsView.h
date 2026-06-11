#pragma once

#include <QGraphicsView>

class QGraphicsScene;

namespace yav
{

namespace space
{
class Space2;
}

namespace voronoi
{
class Diagram;
}

} // namespace yav

class VoronoiGraphicsView : public QGraphicsView
{
    // Q_OBJECT

public:
    explicit VoronoiGraphicsView(QWidget* parent = nullptr);

    void setSpace(const yav::space::Space2& space);

    void setDiagram(const yav::voronoi::Diagram& diagram);

    void autoFit();

protected:
    virtual void wheelEvent(QWheelEvent* event) override;

    virtual void mousePressEvent(QMouseEvent* event) override;

    virtual void mouseReleaseEvent(QMouseEvent* event) override;

private:
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
