// CustomTextEdit.h

#ifndef CUSTOMTEXTEDIT_H
#define CUSTOMTEXTEDIT_H

#include <QPlainTextEdit>

class CustomTextEdit : public QPlainTextEdit
{
    Q_OBJECT
signals:
    // 定义 hyperlinkClicked 信号
    void hyperlinkClicked(const QString &link);

public:
    CustomTextEdit(QWidget *parent = nullptr);

    // 检查给定的文本是否是超链接
    static bool isHyperlink(const QString &text);

    // 在默认浏览器中打开超链接
    static void openHyperlink(const QString &link);

protected:
    void contextMenuEvent(QContextMenuEvent *event) override;

private slots:
    void onCustomContextMenuRequested(const QPoint &pos);
    void onOpenHyperlink();

private:
    QString selectedHyperlink;
};

#endif // CUSTOMTEXTEDIT_H
