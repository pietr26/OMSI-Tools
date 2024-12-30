#ifndef WDGGRAPHICSVIEW_H
#define WDGGRAPHICSVIEW_H

#include <QGraphicsView>
#include <QWheelEvent>
#include <QApplication>

class wdgGraphicsView : public QGraphicsView {
public:
    wdgGraphicsView(QWidget* parent = nullptr);

protected:
    void wheelEvent(QWheelEvent* event) override;
};

#endif // WDGGRAPHICSVIEW_H
