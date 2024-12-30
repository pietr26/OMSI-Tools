#include "wdggraphicsview.h"

wdgGraphicsView::wdgGraphicsView(QWidget *parent) : QGraphicsView(parent)
{
    setTransformationAnchor(QGraphicsView::NoAnchor);
    setResizeAnchor(QGraphicsView::NoAnchor);
}

void wdgGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (QApplication::keyboardModifiers() == Qt::ControlModifier)
    {
        QPointF scenePos = mapToScene(event->position().toPoint());

        const double scaleFactor = 1.125;
        if (event->angleDelta().y() > 0)
            scale(scaleFactor, scaleFactor);
        else
            scale(1.0 / scaleFactor, 1.0 / scaleFactor);

        QPointF newScenePos = mapToScene(event->position().toPoint());
        QPointF delta = newScenePos - scenePos;
        translate(delta.x(), delta.y());
    }
    else QGraphicsView::wheelEvent(event);
}
