// recentfilesmanager.cpp
#include "recentfilesmanager.h"
#include <QSettings>

RecentFilesManager::RecentFilesManager(QObject *parent)
    : QObject(parent)
{
    // 从设置中读取最近文件列表
    QSettings settings("MyCompany", "MyApp");
    recentFilesList = settings.value("recentFiles").toStringList();
}
void RecentFilesManager::addRecentFile(const QString &filePath)
{
    // 添加最近文件
    recentFilesList.removeAll(filePath); // 避免重复
    recentFilesList.prepend(filePath);
    while (recentFilesList.size() > maxRecentFiles)
    {
        recentFilesList.removeLast();
    }
    QSettings settings("MyCompany", "MyApp");
    settings.setValue("recentFiles", recentFilesList);

    emit fileSelected(filePath);
    emit recentFilesChanged();

}

QStringList RecentFilesManager::recentFiles() const
{
    return recentFilesList;
}
