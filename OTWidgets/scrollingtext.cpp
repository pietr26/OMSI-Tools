#include "scrollingtext.h"
#include "ui_scrollingtext.h"

#include <QPainter>

ScrollingText::ScrollingText(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScrollingText),
    _speed(1),
    _currentPosition(0),
    _fps(40),
    _movePixels(0),
    _direction(LeftDirection),
    _textWidthCache(0){
    ui->setupUi(this);

    recalcFontDescent();

    timer.setInterval(25);
    connect(&timer, &QTimer::timeout, this, [this](){
        update();
    });
    timer.start();
}

ScrollingText::~ScrollingText() {
    delete ui;
}

void ScrollingText::setText(const QString &newText) {
    _text = newText;
    recalcTextWidthCache();
}

void ScrollingText::setSpeed(const int &newSpeed) {
    _speed = newSpeed;
    recalcMovePixels();
}

void ScrollingText::setDirection(const Direction &newDirection) {
    _direction = newDirection;
}

void ScrollingText::setFontSize(const int &newSize) {
    _font.setPixelSize(newSize);
    recalcTextWidthCache();
    recalcFontDescent();
}

void ScrollingText::setFps(const int &newFps) {
    _fps = newFps;
    timer.setInterval((1.0 / (float)_fps * 1000.0));
    recalcMovePixels();
}

void ScrollingText::recalcTextWidthCache() {
    QFontMetrics fm(_font);
    _textWidthCache = fm.horizontalAdvance(_text);
}

void ScrollingText::recalcMovePixels() {
    _movePixels = (float)_speed / (float)_fps;
}

void ScrollingText::recalcFontDescent() {
    QFontMetrics fm(_font);
    _fontDescendCache = fm.descent();
}

void ScrollingText::paintEvent(QPaintEvent *e) {
    Q_UNUSED(e);
    const int wdgWidth = width();

    if(_direction == RightDirection)
        _currentPosition += _movePixels;
    else
        _currentPosition -= _movePixels;

    QPainter p(this);
    p.setFont(_font);
    p.drawText(_currentPosition, height() - _fontDescendCache, _text);

    if(_textWidthCache < wdgWidth) {
        if(_currentPosition > wdgWidth - _textWidthCache)
            _currentPosition = - _textWidthCache;

        if(_currentPosition < - _textWidthCache)
            _currentPosition += wdgWidth;

        if(_currentPosition < 0)
            p.drawText(_currentPosition + wdgWidth, height() - _fontDescendCache, _text);
    } else {
        const int textOverlap = _textWidthCache - wdgWidth;
        if(_currentPosition < - _textWidthCache)
            _currentPosition += _textWidthCache;

        if(_currentPosition > 0)
            _currentPosition -= _textWidthCache;

        if(_currentPosition < - textOverlap)
            p.drawText(_currentPosition + _textWidthCache, height() - _fontDescendCache, _text);
    }

    p.end();
}
