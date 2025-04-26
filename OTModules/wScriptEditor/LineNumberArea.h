#ifndef LINENUMBERAREA_H
#define LINENUMBERAREA_H

#include <QPlainTextEdit>

#include <QWidget>
#include <QPainter>
#include <QTextBlock>

#include "ScriptSyntaxHighlighter.h"

class CodeEditor;

class LineNumberArea : public QWidget
{
    Q_OBJECT
public:
    explicit LineNumberArea(CodeEditor *parent = nullptr);

    QSize sizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    int lineNumberAreaWidth() const;
    CodeEditor *_codeEditor;
};

#endif // LINENUMBERAREA_H
