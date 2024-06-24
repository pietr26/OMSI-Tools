#include "graphicsview.h"

GraphicsView::GraphicsView()
{

}

void GraphicsView::wheelEvent(QWheelEvent *event)
{
    if (ctrlActive)
    {
        setTransformationAnchor(ViewportAnchor::AnchorUnderMouse);

        if (event->angleDelta().y() > 0) scale(factor, factor);
        else scale(1 / factor, 1 / factor);
    }

    QWidget::wheelEvent(event);
}

void GraphicsView::keyPressEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) ctrlActive = true;
}

void GraphicsView::keyReleaseEvent(QKeyEvent *event)
{
    if (event->modifiers() & Qt::ControlModifier) ctrlActive = false;
}

void GraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        middleButtonActive = true;
        _posX = event->position().x();
        _posY = event->position().y();
        setCursor(Qt::ClosedHandCursor);
    }
}

void GraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::MiddleButton)
    {
        middleButtonActive = false;
        setCursor(Qt::ArrowCursor);
    }
}

void GraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    if (middleButtonActive)
    {
        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->position().x() - _posX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->position().y() - _posY));
        _posX = event->position().x();
        _posY = event->position().y();
    }
}
