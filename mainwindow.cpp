#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "aboutdialog.h"
#include"searchdialog.h"
#include"replacedialog.h"
#include "qfiledialog.h"
#include"QMessageBox"
#include"QTextStream"
#include"QColorDialog"
#include"QFontDialog"
#include <QDebug>
#include"recentfilesmanager.h"

#include "customtextedit.h"
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    textchanged = false;
    on_actionNew_triggered();
    statusLabel.setMaximumWidth(190);
    statusLabel.setText("length: " + QString::number(0) + "  lines:" + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusLabel);
    statusCursorLabel.setMaximumWidth(190);
    statusCursorLabel.setText("length: " + QString::number(0) + "  lines:" + QString::number(1));
    ui->statusbar->addPermanentWidget(&statusCursorLabel);

    QLabel *author = new QLabel(ui->statusbar);
    author->setText(tr("杨东矫"));



    recentFilesAction = new QAction("最近打开文件", this);
    connect(recentFilesAction, &QAction::triggered, this, &MainWindow::showRecentFilesList);
    ui->menubar->addAction(recentFilesAction);
    connect(&recentFilesManager, &RecentFilesManager::recentFilesChanged, this, &MainWindow::updateRecentFilesMenu);
    CustomTextEdit *customTextEdit = new CustomTextEdit(this);
    connect(customTextEdit, &CustomTextEdit::hyperlinkClicked, this, &MainWindow::onOpenHyperlink);


    ui->statusbar->addPermanentWidget(author);
    ui->actionUndo->setEnabled(false);
    ui->action_redo->setEnabled(false);
    ui->actionCopy->setEnabled(false);
    ui->actionCut->setEnabled(false);
    ui->actionPaste->setEnabled(false);
    ui->actionStatusBar->setChecked(true);
    ui->actionToolBar->setChecked(true);
    ui->actionListNumber->setChecked(false);
    on_actionListNumber_triggered(false);

    connect(ui->actionAddBookmark, &QAction::triggered, this, &MainWindow::on_actionAddBookmark_triggered);
    connect(ui->actionShowBookmarks, &QAction::triggered, this, &MainWindow::on_actionShowBookmarks_triggered);

    connect(ui->actionFontcolor, &QAction::triggered, this, &MainWindow::on_actionFontcolor_triggered);
    connect(ui->actionbackcolor, &QAction::triggered, this, &MainWindow::on_actionbackcolor_triggered);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionAddBookmark_triggered()
{
    QTextCursor cursor = ui->textEdit->textCursor();
    Bookmark bookmark;
    bookmark.lineNumber = cursor.blockNumber() + 1;
    bookmark.columnNumber = cursor.columnNumber() + 1;
    bookmarks.append(bookmark);

    qDebug() << "Bookmark added at Line:" << bookmark.lineNumber << "Column:" << bookmark.columnNumber;
}

void MainWindow::on_actionShowBookmarks_triggered()
{
    QMenu *bookmarksMenu = new QMenu(this);

    for (const Bookmark &bookmark : bookmarks)
    {
        QString label = QString("Line %1, Column %2").arg(bookmark.lineNumber).arg(bookmark.columnNumber);
        QAction *bookmarkAction = bookmarksMenu->addAction(label);
        connect(bookmarkAction, &QAction::triggered, this, [this, bookmark]()
        {

            scrollToBookmark(bookmark);
        });
    }


    bookmarksMenu->popup(ui->actionShowBookmarks->parentWidget()->mapToGlobal(ui->actionShowBookmarks->parentWidget()->pos()));
}


void MainWindow::scrollToBookmark(const Bookmark &bookmark)
{

    QTextCursor cursor = ui->textEdit->textCursor();
    cursor.setPosition(ui->textEdit->document()->findBlockByLineNumber(bookmark.lineNumber - 1).position());
    cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor, bookmark.columnNumber - 1);
    ui->textEdit->setTextCursor(cursor);

    // 如果需要，可以将光标所在的位置滚动到可见区域
    ui->textEdit->ensureCursorVisible();
}


void MainWindow::updateRecentFilesMenu()
{
    showRecentFilesList();
}
void MainWindow::onOpenHyperlink(const QString &link)
{
    CustomTextEdit::openHyperlink(link);
}

// 在 showRecentFilesList 槽函数中获取最近打开文件列表并显示
void MainWindow::showRecentFilesList()
{
    QStringList recentFiles = recentFilesManager.recentFiles();
    qDebug() << "Recent Files: " << recentFiles;  // 添加这行调试输出

    // 创建一个 QMenu 用于显示最近打开文件列表
    QMenu *recentFilesMenu = new QMenu(this);  // 设置父对象为当前窗口

    for (const QString &filePath : recentFiles)
    {
        QAction *fileAction = recentFilesMenu->addAction(filePath);
        connect(fileAction, &QAction::triggered, this, [this, filePath]()
        {

            onOpenRecentFile(filePath);
        });

    }


    recentFilesAction->setMenu(recentFilesMenu);


    QMainWindow *mainWin = qobject_cast<QMainWindow *>(ui->menubar->parentWidget());

    if (mainWin)
    {
        QPoint pos = mainWin->mapToGlobal(ui->menubar->geometry().bottomLeft());
        recentFilesAction->menu()->popup(pos);
    }
}

// 处理打开最近文件的操作
void MainWindow::onOpenRecentFile(const QString &filePath)
{
    if (!userEditConfirmed())
    {
        return;
    }

    QFile file(filePath);

    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "..", "打开文件失败");
        return;
    }

    ui->textEdit->clear();  // 清空文本编辑器内容
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->insertPlainText(text);
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    textchanged = false;

    // 添加最近文件
    recentFilesManager.addRecentFile(filePath);
}


void MainWindow::on_actionAbout_triggered()
{
    AboutDialog dlg;
    dlg.exec();
}

void MainWindow::on_actionFind_triggered()
{
    SearchDialog dlg(this, ui->textEdit);
    dlg.exec();
}

void MainWindow::on_actionReplace_triggered()
{
    ReplaceDialog dlg(this, ui->textEdit);
    dlg.exec();
}
void MainWindow::on_actionNew_triggered()
{
    if (!userEditConfirmed())
    {
        return;
    }
    filePath = "";
    ui->textEdit->clear();
    this->setWindowTitle(tr("新建文本文件 - 编辑器"));
    textchanged = false;
    // 添加最近文件
    recentFilesManager.addRecentFile(filePath);
}


void MainWindow::on_actionOpen_triggered()
{
    if (!userEditConfirmed())
    {
        return;
    }
    QString filename = QFileDialog::getOpenFileName(this, "打开文件", ".", tr("Text files (*.txt) ;; All (*.*)"));
    QFile file(filename);


    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, "..", "打开文件失败");
        return;

    }
    filePath = filename;
    QTextStream in(&file);
    QString text = in.readAll();
    ui->textEdit->insertPlainText(text);
    file.close();
    this->setWindowTitle(QFileInfo(filename).absoluteFilePath());
    textchanged = false;

    recentFilesManager.addRecentFile(filePath);
}


void MainWindow::on_actionSave_triggered()
{
    if (filePath == "")
    {
        QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files(*.txt)"));
        QFile file(filename);
        if (!file.open(QFile::WriteOnly | QFile::Text))
        {
            QMessageBox::warning(this, "..", "打开保存文件失败");
            return;

        }
        file.close();
        filePath = filename;
    }
    QFile file(filePath);


    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "..", "打开保存文件失败");
        return;

    }
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
    textchanged = false;
}

void MainWindow::on_actionSaveAs_triggered()
{

    QString filename = QFileDialog::getSaveFileName(this, "保存文件", ".", tr("Text files(*.txt)"));
    QFile file(filename);
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, "..", "打开保存文件失败");
        return;

    }

    filePath = filename;
    QTextStream out(&file);
    QString text = ui->textEdit->toPlainText();
    out << text;
    file.flush();
    file.close();
    this->setWindowTitle(QFileInfo(filePath).absoluteFilePath());
}


void MainWindow::on_textEdit_textChanged()
{
    if (!textchanged)
    {
        this->setWindowTitle("*z" + this->windowTitle());
        textchanged = true;
    }
    statusLabel.setText("length: " + QString::number(ui->textEdit->toPlainText().length()) + "  lines:" +
                        QString::number(ui->textEdit->document()->lineCount()));
}

bool MainWindow::userEditConfirmed()
{
    if (textchanged)
    {
        QString path = (filePath != "") ? filePath : "无标题.txt";
        QMessageBox msg(this);
        msg.setIcon(QMessageBox::Question);
        msg.setWindowTitle("...");
        msg.setWindowFlag(Qt ::Drawer);
        msg.setText(QString("是否将更改保存到\n") + """+ path + ”?");
        msg.setStandardButtons(QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);
        int r = msg.exec();
        switch ( r )
        {
            case QMessageBox::Yes :
                on_actionSave_triggered();
                break;
            case QMessageBox::No:
                textchanged = false;
                break;
            case QMessageBox::Cancel:
                return false;
        }
    }
    return true;
}








void MainWindow::on_actionUndo_triggered()
{
    ui->textEdit->undo();
}

void MainWindow::on_actionCut_triggered()
{
    ui->textEdit->cut();
    ui->actionPaste->setEnabled(true);
}

void MainWindow::on_actionCopy_triggered()
{
    ui->textEdit->copy();
    ui->actionPaste->setEnabled(true);
}


void MainWindow::on_actionPaste_triggered()
{
    ui->textEdit->paste();
}



void MainWindow::on_action_redo_triggered()
{
    ui->textEdit->redo();
}

void MainWindow::on_textEdit_undoAvailable(bool b)
{
    ui->actionUndo->setEnabled(b);
}
void MainWindow::on_textEdit_copyAvailable(bool b)
{
    ui->actionCopy->setEnabled(b);
    ui->actionCut->setEnabled(b);

}


void MainWindow::on_textEdit_redoAvailable(bool b)
{
    ui->actionPaste->setEnabled(b);
}

// 实现槽函数
void MainWindow::on_actionFontcolor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if (color.isValid())
    {
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit { color: %1; }").arg(color.name()));
    }
    else
    {
        // 处理错误，例如向用户显示消息
    }
}

void MainWindow::on_actionbackcolor_triggered()
{
    QColor color = QColorDialog::getColor(Qt::black, this, "选择颜色");
    if (color.isValid())
    {
        ui->textEdit->setStyleSheet(QString("QPlainTextEdit { background-color: %1; }").arg(color.name()));
    }
    else
    {
        // 处理错误，例如向用户显示消息
    }
}


void MainWindow::on_actionfgbclolor_triggered()
{

}


void MainWindow::on_actionWrap_triggered()
{
    QPlainTextEdit::LineWrapMode mode = ui->textEdit->lineWrapMode();
    if ( mode == QTextEdit::NoWrap )
    {
        ui->textEdit->setLineWrapMode(QPlainTextEdit::WidgetWidth);
        ui->actionWrap->setChecked(true);
    }
    else
    {

        ui->textEdit->setLineWrapMode(QPlainTextEdit::NoWrap);
        ui->actionWrap->setChecked(false);
    }
}


void MainWindow::on_actionFont_triggered()
{
    bool ok = false;
    QFont font = QFontDialog::getFont(&ok, this);
    if (ok)
    {
        ui->textEdit->setFont(font);
    }
}


void MainWindow::on_actionToolBar_triggered()
{
    bool visible = ui->toolBar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionToolBar->setChecked(!visible);
}



void MainWindow::on_actionStatusBar_triggered()
{
    bool visible = ui->statusbar->isVisible();
    ui->toolBar->setVisible(!visible);
    ui->actionStatusBar->setChecked(!visible);
}


void MainWindow::on_actionSelectAll_triggered()
{
    ui->textEdit->selectAll();
}


void MainWindow::on_actionExit_triggered()
{
    if (userEditConfirmed())
    {
        exit(0);
    }
}


void MainWindow::on_textEdit_cursorPositionChanged()
{
    int col = 0;
    int ln = 0;
    int flg = -1;
    int pos = ui->textEdit->textCursor().position();
    QString text = ui->textEdit->toPlainText();
    for (int i = 0; i < pos; i++)
    {
        if ( text[i] == '\n' )
        {
            ln ++;
            flg = i;
        }
    }
    flg ++;
    col = pos - flg;
    statusCursorLabel.setText("length: " + QString::number(ln + 1) + "  lines:" + QString::number(col + 1));
}




void MainWindow::on_actionListNumber_triggered(bool checked)
{
    ui->textEdit->hideLineNumberArea(!checked);
}

