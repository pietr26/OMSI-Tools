#ifndef GRAPHICSVIEW_H
#define GRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWidget>
#include <QWheelEvent>
#include <QScrollBar>

class GraphicsView : public QGraphicsView
{
public:
    GraphicsView();

protected:
    void wheelEvent(QWheelEvent *event) override;

    void keyPressEvent(QKeyEvent *event) override;
    void keyReleaseEvent(QKeyEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;

private:
    bool ctrlActive = false;
    bool middleButtonActive = false;

    float factor = 1.2;

    int _posX, _posY;
};

#endif // GRAPHICSVIEW_H
