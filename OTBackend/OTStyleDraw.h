#ifndef OTSTYLEDRAW_H
#define OTSTYLEDRAW_H

#include <QProxyStyle>
#include <QStyle>
#include <QStyleOption>

class OTStyleDraw : public QProxyStyle
{
public:
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        if (element == QStyle::PE_FrameStatusBarItem)
            return;

        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

#endif // OTSTYLEDRAW_H
