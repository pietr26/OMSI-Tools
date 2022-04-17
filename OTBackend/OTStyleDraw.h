#ifndef OTSTYLEDRAW_H
#define OTSTYLEDRAW_H

#include <QProxyStyle>
#include <QStyle>
#include <QStyleOption>

/// Class for style fixes
class OTStyleDraw : public QProxyStyle
{
public:
    /// [OVERRIDE]
    void drawPrimitive(PrimitiveElement element, const QStyleOption *option, QPainter *painter, const QWidget *widget) const
    {
        // If the element is the frame of statusbar items, do nothing (removes the line behind each item)
        if (element == QStyle::PE_FrameStatusBarItem)
            return;

        QProxyStyle::drawPrimitive(element, option, painter, widget);
    }
};

#endif // OTSTYLEDRAW_H
