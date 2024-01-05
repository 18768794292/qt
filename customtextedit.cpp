// CustomTextEdit.cpp

#include "CustomTextEdit.h"
#include <QDesktopServices>
#include <QUrl>
#include <QRegularExpression>
#include <QMenu>
#include <QAction>
#include<QDebug>
CustomTextEdit::CustomTextEdit(QWidget *parent)
    : QPlainTextEdit(parent)
{
    connect(this, &CustomTextEdit::customContextMenuRequested, this, &CustomTextEdit::onCustomContextMenuRequested);
    connect(this, &CustomTextEdit::hyperlinkClicked, this, &CustomTextEdit::onOpenHyperlink);
}

bool CustomTextEdit::isHyperlink(const QString &text)
{
    QUrl url(text);
    return url.isValid() && (url.scheme().startsWith("http") || url.scheme().startsWith("https"));
}

void CustomTextEdit::openHyperlink(const QString &link)
{
    qDebug() << "Opening hyperlink:" << link;
    bool opened = QDesktopServices::openUrl(QUrl(link));
    if (!opened)
    {
        qDebug() << "Failed to open hyperlink:" << link;
    }
}


void CustomTextEdit::contextMenuEvent(QContextMenuEvent *event)
{
    selectedHyperlink = textCursor().selectedText();

    if (isHyperlink(selectedHyperlink))
    {
        QMenu menu(this);
        QAction *openAction = menu.addAction("打开超链接");
        connect(openAction, &QAction::triggered, this, &CustomTextEdit::onOpenHyperlink);

        menu.exec(event->globalPos());
    }
    else
    {
        QPlainTextEdit::contextMenuEvent(event);
    }
}

void CustomTextEdit::onCustomContextMenuRequested(const QPoint &pos)
{
    contextMenuEvent(new QContextMenuEvent(QContextMenuEvent::Reason::Mouse, pos));
}

void CustomTextEdit::onOpenHyperlink()
{
    emit hyperlinkClicked(selectedHyperlink);
}
