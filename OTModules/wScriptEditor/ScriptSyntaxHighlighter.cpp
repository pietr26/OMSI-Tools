#include "ScriptSyntaxHighlighter.h"

ScriptSyntaxHighlighter::ScriptSyntaxHighlighter(const QFont &font, QTextDocument *parent) :
    QSyntaxHighlighter(parent),
    _font(font) {
    QFont italicFont = font;
    italicFont.setItalic(true);

    bool dark = QApplication::palette().color(QPalette::Window).lightness() < 128;

    commentFormat.setForeground(dark ? QColor(96, 128, 96) : QColor(0, 128, 0));
    stringFormat.setForeground(dark ? QColor(128, 192, 128) : QColor(0, 128, 0));
    numberFormat.setForeground(dark ? QColor(128, 128, 192) : QColor(32, 32, 128));
    blockFormat.setForeground(dark ? QColor(128, 128, 255) : QColor(0, 0, 128));
    ifFormat.setForeground(QColor(215, 58, 73));
    varFormat.setForeground(dark ? QColor(0, 160, 192) : QColor(0, 103, 124));
    commandFormat.setForeground(dark ? QColor(214, 149, 69) : QColor(192, 102, 0));
}

void ScriptSyntaxHighlighter::highlightBlock(const QString &text) {
    if(text.startsWith("'")) {
        setFormat(0, text.length(), commentFormat);
        return;
    }

    formatParts(text, ifKeywords, ifFormat);
    formatParts(text, blockKeywords, blockFormat);
    formatParts(text, varKeywords, varFormat);
    formatParts(text, commandKeywords, commandFormat);
    formatParts(text, {"\".+?\""}, stringFormat);
    formatParts(text, {"-?([0-9]+|[0-9]*\\.[0-9]+)"}, numberFormat);
}

void ScriptSyntaxHighlighter::formatParts(const QString &text, const QStringList &patterns, const QTextCharFormat &format) {
    foreach (const QString &keyword, patterns) {
        QRegularExpression regex("(?<=\\s|^)" + keyword + "(?=\\s|$)");
        QRegularExpressionMatchIterator it = regex.globalMatch(text);
        while(it.hasNext()) {
            QRegularExpressionMatch match = it.next();
            setFormat(match.capturedStart(), match.capturedLength(), format);
        }
    }
}
