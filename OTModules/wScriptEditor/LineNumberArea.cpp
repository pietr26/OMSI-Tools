#include "LineNumberArea.h"

#include "CodeEditor.h"

LineNumberArea::LineNumberArea(CodeEditor *parent) :
    QWidget(parent), _codeEditor(parent) {

    connect(_codeEditor, &QPlainTextEdit::cursorPositionChanged, this, [this](){update();});
}

QSize LineNumberArea::sizeHint() const {
    return QSize(lineNumberAreaWidth(), 0);
}

void LineNumberArea::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.setFont(_codeEditor->document()->defaultFont());
    painter.setPen(QColor(128, 128, 128));
    painter.save();

    const int currentLineNumber = _codeEditor->textCursor().blockNumber();

    CodeEditor *editor = qobject_cast<CodeEditor *>(_codeEditor);
    QTextBlock block = editor->firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = static_cast<int>(editor->blockBoundingGeometry(block).translated(editor->contentOffset()).top());
    int bottom = top + static_cast<int>(editor->blockBoundingRect(block).height());

    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);

            if(blockNumber == currentLineNumber)
                painter.setPen(ScriptSyntaxHighlighter::darkMode() ? Qt::white : Qt::black);

            painter.drawText(0, top, width() - 4, editor->fontMetrics().height(),
                             Qt::AlignRight, number);

            if(blockNumber == currentLineNumber)
                painter.restore();
        }

        block = block.next();
        top = bottom;
        bottom = top + static_cast<int>(editor->blockBoundingRect(block).height());
        ++blockNumber;
    }
}

int LineNumberArea::lineNumberAreaWidth() const {
    return _codeEditor->lineNumberAreaWidth();
}
