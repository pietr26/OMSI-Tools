#ifndef WDGGRAPHICSVIEW_H
#define WDGGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QApplication>

class wdgGraphicsView : public QGraphicsView {
public:
    wdgGraphicsView(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event) override;

    void mouseMoveEvent(QMouseEvent* event) override;

    void mouseReleaseEvent(QMouseEvent* event) override;

    void wheelEvent(QWheelEvent* event) override;

private:
    bool dragging;
    QPoint dragStartPos;
};

#endif // WDGGRAPHICSVIEW_H
