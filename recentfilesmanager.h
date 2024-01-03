// recentfilesmanager.h
#ifndef RECENTFILESMANAGER_H
#define RECENTFILESMANAGER_H

#include <QObject>
#include <QStringList>

class RecentFilesManager : public QObject
{
    Q_OBJECT
public:
    RecentFilesManager(QObject *parent = nullptr);

    void addRecentFile(const QString &filePath);
    QStringList recentFiles() const;

signals:
    void fileSelected(const QString &filePath);
    void recentFilesChanged();  // 添加的信号
private:
    QStringList recentFilesList;
    const int maxRecentFiles = 100; // 最大记录文件数
};

#endif // RECENTFILESMANAGER_H
