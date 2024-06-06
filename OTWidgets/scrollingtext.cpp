#include "scrollingtext.h"
#include "ui_scrollingtext.h"

#include <QPainter>

ScrollingText::ScrollingText(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ScrollingText),
    _speed(80),
    _currentPosition(0),
    _fps(40),
    _movePixels(0),
    _direction(LeftDirection),
    _textWidthCache(0){
    ui->setupUi(this);

    recalcFontDescent();

    setFps(40);
    setFont(_font);

    connect(&timer, &QTimer::timeout, this, [this](){
        update();
    });
    timer.start();
}

ScrollingText::~ScrollingText() {
    delete ui;
}

QString ScrollingText::text() const {
    return _text;
}

void ScrollingText::setText(const QString &newText) {
    _text = newText;
    recalcTextWidthCache();
}

int ScrollingText::speed() const {
    return _speed;
}

void ScrollingText::setSpeed(const int &newSpeed) {
    _speed = newSpeed;
    recalcMovePixels();
}

ScrollingText::Direction ScrollingText::direction() const {
    return _direction;
}

void ScrollingText::setDirection(const Direction &newDirection) {
    _direction = newDirection;
}

QFont ScrollingText::font() const {
    return _font;
}

void ScrollingText::setFont(const QFont &newFont) {
    _font = newFont;
    recalcTextWidthCache();
    recalcFontDescent();

    QFontMetrics fm(_font);
    setMinimumHeight(fm.height());
}

QColor ScrollingText::textColor() const {
    return _color;
}

void ScrollingText::setTextColor(const QColor &newColor) {
    _color = newColor;
    emit textColorChanged(_color);
}

void ScrollingText::resetColor() {
    _color = QColor();
    emit textColorChanged(_color);
}

int ScrollingText::fps() const {
    return _fps;
}

void ScrollingText::setFps(const int &newFps) {
    _fps = newFps;
    timer.setInterval((1.0 / (float)_fps) * 1000.0);
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
    p.setPen(_color);
    p.setFont(_font);
    p.drawText(_currentPosition, height() - _fontDescendCache, _text);

    if(_textWidthCache < wdgWidth) {
        if(_currentPosition < 0)
            p.drawText(_currentPosition + wdgWidth, height() - _fontDescendCache, _text);

        if(_currentPosition > wdgWidth - _textWidthCache)
            _currentPosition = - _textWidthCache;

        if(_currentPosition < - _textWidthCache)
            _currentPosition += wdgWidth;
    } else {
        const int textOverlap = _textWidthCache - wdgWidth;

        if(_currentPosition < - textOverlap)
            p.drawText(_currentPosition + _textWidthCache, height() - _fontDescendCache, _text);

        if(_currentPosition < - _textWidthCache)
            _currentPosition += _textWidthCache;

        if(_currentPosition > 0)
            _currentPosition -= _textWidthCache;

    }

    p.end();
}
