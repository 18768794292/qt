// codehighlighter.cpp
#include "codehighlighter.h"
#include <QTextDocument>
#include <QTextBlock>
#include <QRegularExpressionMatchIterator>

CodeHighlighter::CodeHighlighter(QTextDocument *parent) : QSyntaxHighlighter(parent)
{
    // 初始化语法规则和格式
    HighlightingRule rule;

    // 关键字格式
    keywordFormat.setForeground(Qt::blue);

    // C++ 关键字
    QStringList cppKeywords;
    cppKeywords << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
                << "\\bchar\\b" << "\\bbool\\b" << "\\bvoid\\b"
                << "\\bif\\b" << "\\belse\\b" << "\\bwhile\\b"
                << "\\bfor\\b" << "\\breturn\\b"; // 添加更多 C++ 关键字
    foreach (const QString &pattern, cppKeywords)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // Java 关键字
    QStringList javaKeywords;
    javaKeywords << "\\bint\\b" << "\\bfloat\\b" << "\\bdouble\\b"
                 << "\\bchar\\b" << "\\bboolean\\b" << "\\bvoid\\b"
                 << "\\bif\\b" << "\\belse\\b" << "\\bwhile\\b"
                 << "\\bfor\\b" << "\\breturn\\b"; // 添加更多 Java 关键字
    foreach (const QString &pattern, javaKeywords)
    {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    // 其他语法规则，包括单行注释、多行注释、字符串和数字
    singleLineCommentFormat.setForeground(Qt::green);
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::green);
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");

    quotationFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegularExpression("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    numberFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegularExpression("\\b\\d+\\.?\\d*\\b");
    rule.format = numberFormat;
    highlightingRules.append(rule);
}

void CodeHighlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext())
        {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }

    setCurrentBlockState(0);
    int startIndex = 0;
    if (previousBlockState() != 1)
    {
        startIndex = text.indexOf(commentStartExpression);
    }

    while (startIndex >= 0)
    {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}
