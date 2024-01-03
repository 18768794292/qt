// codehighlighter.h
#ifndef CODEHIGHLIGHTER_H
#define CODEHIGHLIGHTER_H

#include <QSyntaxHighlighter>
#include <QTextCharFormat>
#include <QRegularExpression>

class CodeHighlighter : public QSyntaxHighlighter
{
    Q_OBJECT

public:
    CodeHighlighter(QTextDocument *parent = nullptr);

protected:
    void highlightBlock(const QString &text) override;

private:
    struct HighlightingRule
    {
        QRegularExpression pattern;
        QTextCharFormat format;
    };

    QVector<HighlightingRule> highlightingRules;

    QTextCharFormat keywordFormat;
    QTextCharFormat singleLineCommentFormat;
    QTextCharFormat multiLineCommentFormat;
    QTextCharFormat quotationFormat;
    QTextCharFormat numberFormat;

    QRegularExpression commentStartExpression;
    QRegularExpression commentEndExpression;
};

#endif // CODEHIGHLIGHTER_H
