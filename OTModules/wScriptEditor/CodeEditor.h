#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QObject>
#include <QPlainTextEdit>

#include "LineNumberArea.h"

#include "ScriptSyntaxHighlighter.h"

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT
public:
    CodeEditor(QWidget *parent = nullptr);

    int lineNumberAreaWidth();

protected:
    void resizeEvent(QResizeEvent *event) override;

public slots:
    void updateLineNumberAreaWidth();
    void updateLineNumberArea(const QRect &rect, int dy);
    void highlightCurrentLine();

protected slots:
    void onDocumentChanged();

private:
    QWidget *_lineNumberArea;

    friend class LineNumberArea;
};

#endif // CODEEDITOR_H
