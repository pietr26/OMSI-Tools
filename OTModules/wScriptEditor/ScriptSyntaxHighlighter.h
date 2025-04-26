#ifndef SCRIPTSYNTAXHIGHLIGHTER_H
#define SCRIPTSYNTAXHIGHLIGHTER_H

#include <QObject>
#include <QSyntaxHighlighter>
#include <QRegularExpression>
#include <QApplication>
#include <QPalette>

class ScriptSyntaxHighlighter : public QSyntaxHighlighter {
    Q_OBJECT
public:
    ScriptSyntaxHighlighter(const QFont &font, QTextDocument *parent = nullptr);

    static bool darkMode();

protected:
    void highlightBlock(const QString &text) override;

    void formatParts(const QString &text, const QStringList &patterns, const QTextCharFormat &format);

    QTextCharFormat commentFormat;
    QTextCharFormat stringFormat;
    QTextCharFormat numberFormat;
    QTextCharFormat blockFormat;
    QTextCharFormat ifFormat;
    QTextCharFormat varFormat;
    QTextCharFormat commandFormat;

    QFont _font;

    static const inline QStringList blockKeywords = {"{init}", "{frame}", "{(macro|trigger):\\w+}", "{end}"};
    static const inline QStringList ifKeywords = {"{if}", "{else}", "{endif}"};
    static const inline QStringList varKeywords = {"\\(L\\.L\\.\\w+\\)",   // load var
                                                   "\\(S\\.L\\.\\w+\\)",   // save var
                                                   "\\(L\\.S\\.\\w+\\)",   // load global var
                                                   "\\(L\\.\\$\\.\\w+\\)", // load string
                                                   "\\(S\\.\\$\\.\\w+\\)", // save string
                                                   "\\(C\\.L\\.\\w+\\)",   // load const
                                                   "\\(T\\.L\\.\\w+\\)",   // simple sound trigger
                                                   "\\(T\\.F\\.\\w+\\)",   // file switch sound trigger
                                                   "\\(M\\.L\\.\\w+\\)",   // macro call
                                                   "\\(F\\.L\\.\\w+\\)"};  // load curve

    static const inline QStringList commandKeywords = {"l0", "l1", "l2", "l3", "l4", "l5", "l6", "l7",
                                                        "s0", "s1", "s2", "s3", "s4", "s5", "s6", "s7",
                                                        "d", "\\­$msg", "\\$d", "&&", "\\|\\|", "!",
                                                        "=", "<", ">", "<=", ">=", "\\$=", "\\$<", "\\$>", "\\$<=", "\\$>=",
                                                        "\\+", "-", "\\*", "/", "%", "/-/", "sin", "arcsin", "arctan", "min", "max", "exp", "sqrt", "sqr", "sgn", "pi", "random", "abs", "trunc",
                                                        "\\$\\+", "\\$\\*", "\\$length", "\\$cutBegin", "\\$cutEnd", "\\$SetLengthR", "\\$SetLengthC", "\\$SetLengthL", "\\$IntToStr", "\\$IntToStrEnh", "\\$StrToFloat", "\\$RemoveSpaces"
                                                        };
};

#endif // SCRIPTSYNTAXHIGHLIGHTER_H
