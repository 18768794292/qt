
#include "recentfilesmanager.h"
#include <QSettings>

RecentFilesManager::RecentFilesManager(QObject *parent)
    : QObject(parent)
{

    QSettings settings("MyCompany", "MyApp");
    recentFilesList = settings.value("recentFiles").toStringList();
}
void RecentFilesManager::addRecentFile(const QString &filePath)
{

    recentFilesList.removeAll(filePath);
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
