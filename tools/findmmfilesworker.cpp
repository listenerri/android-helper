#include "findmmfilesworker.h"
#include "findmmfiles.h"
#include <QDirIterator>
#include <QDebug>

FindMMFilesWorker::FindMMFilesWorker(QObject *parent) : QObject(parent)
{

}

void FindMMFilesWorker::doWork()
{
    int allMMFilesCount = 0;
    QFileInfoList musicFiles;
    QDirIterator dirIterator(m_path, QDir::AllEntries | QDir::NoDotAndDotDot, QDirIterator::Subdirectories);
    while (dirIterator.hasNext()) {
        if (FindMMFiles::CancelFlag) {
            qDebug() << "(FindMMFilesWorker::doWork) work thread for " << m_path << " canceled!!";
            emit workCanceled();
            return;
        }
        dirIterator.next();
        QFileInfo fileInfo = dirIterator.fileInfo();
        if (fileInfo.isFile()) {
            if (FindMMFiles::isMusicFile(fileInfo)) {
                allMMFilesCount++;
                musicFiles << fileInfo;
                qDebug() << "(FindMMFilesWorker::doWork) find music file:" << dirIterator.filePath();
            }
        }
    }
    qDebug() << "(FindMMFilesWorker::doWork) work thread for " << m_path << " done!!";
    emit workDone(allMMFilesCount, musicFiles);
}

void FindMMFilesWorker::setPath(const QString &path)
{
    m_path = path;
}
