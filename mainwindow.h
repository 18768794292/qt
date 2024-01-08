#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include<Qlabel>
#include <QTabWidget>
#include <QTextEdit>
#include"recentfilesmanager.h"
#include "customtextedit.h"
#include <QTextBrowser>  // 替换为 QTextBrowser
#include <QTextCursor>
QT_BEGIN_NAMESPACE
struct Bookmark
{
    int lineNumber;
    int columnNumber;
};
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionAbout_triggered();

    void on_actionNew_triggered();

    void on_actionOpen_triggered();

    void on_actionSave_triggered();

    void on_actionSaveAs_triggered();

    void on_textEdit_textChanged();

    void on_actionFind_triggered();

    void on_actionReplace_triggered();

    void on_actionUndo_triggered();

    void on_actionCopy_triggered();

    void on_actionPaste_triggered();

    void on_actionCut_triggered();

    void on_action_redo_triggered();

    void on_textEdit_copyAvailable(bool b);

    void on_textEdit_undoAvailable(bool b);

    void on_textEdit_redoAvailable(bool b);

    void on_actionbackcolor_triggered();

    void on_actionFontcolor_triggered();

    void on_actionfgbclolor_triggered();

    void on_actionWrap_triggered();

    void on_actionFont_triggered();

    void on_actionToolBar_triggered();

    void on_actionStatusBar_triggered();

    void on_actionSelectAll_triggered();

    void on_actionExit_triggered();

    void on_textEdit_cursorPositionChanged();



    void on_actionListNumber_triggered(bool checked);
    void showRecentFilesList();
    void onOpenRecentFile(const QString &filePath);
    void updateRecentFilesMenu();
    void onOpenHyperlink(const QString &link);


    void on_actionAddBookmark_triggered();
    void on_actionShowBookmarks_triggered();
    void scrollToBookmark(const Bookmark &bookmark);
private:
    Ui::MainWindow *ui;
    QLabel statusCursorLabel;
    QLabel statusLabel;
    QString filePath;
    bool textchanged;
    bool userEditConfirmed();
    QTabWidget *tabWidget;  // 添加 QTabWidget 成员变
    RecentFilesManager recentFilesManager;
    QAction *recentFilesAction;  // 新添加的 QAction
    CustomTextEdit *customTextEdit;



    QList<Bookmark> bookmarks;

};
#endif // MAINWINDOW_H
