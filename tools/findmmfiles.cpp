#include "findmmfiles.h"
#include <QDirIterator>
#include <QThread>
#include <QDebug>

bool FindMMFiles::CancelFlag = false;
QStringList FindMMFiles::MusicSuffix = QStringList() << "mp3";
QStringList FindMMFiles::DirBlacklist = QStringList() << "Android" << "alipay" << "gifshow"\
                                                      << "MicroMsg" << "MobileQQ" << "msflogs"\
                                                      << "autonavi";

QStringList FindMMFiles::DirRecursion = QStringList() << "tencent";

FindMMFiles::FindMMFiles(QObject *parent) : QObject(parent)
{
    m_allMMFilesCount = 0;
    m_threadCount = 0;
    m_threadCountLeft = 0;
    m_waitOnce = false;
}

// 每当遍历到path中的一个子目录就启动一个新的线程去遍历这个目录
void FindMMFiles::doFindMMFiles(QString path)
{
    QFileInfoList fileInfoList = QDir(path).entryInfoList(QDir::AllEntries | QDir::NoDotAndDotDot);
    foreach (auto fileInfo, fileInfoList) {
        if (fileInfo.isFile()) {
            if (isMusicFile(fileInfo)) {
                m_allMMFilesCount++;
                m_musicFiles << fileInfo;
            }
        } else if (fileInfo.isDir()){
            QString dirName = fileInfo.fileName();
            QString dirPath = fileInfo.absoluteFilePath();
            // 递归遍历某些目录
            if (DirRecursion.contains(dirName, Qt::CaseInsensitive)) {
                qDebug() << "(FindMMFiles::doFindFiles) recursion dir:" << dirPath;
                m_waitOnce = true;
                doFindMMFiles(dirPath);
                continue;
            }
            // 跳过那些文件量巨大且不包含需要的多媒体文件的目录
            if (DirBlacklist.contains(dirName, Qt::CaseInsensitive)) {
                qDebug() << "(FindMMFiles::doFindFiles) skip dir:" << dirPath;
                continue;
            }
            qDebug() << "(FindMMFiles::doFindFiles) new work thread for " << dirPath;
            FindMMFilesWorker *newWorker = new FindMMFilesWorker;
            workers.append(newWorker);
            newWorker->setPath(dirPath);
            QThread *newThread = new QThread;
            connect(this, &FindMMFiles::startWork, newWorker, &FindMMFilesWorker::doWork);
            connect(newWorker, &FindMMFilesWorker::workDone, this, &FindMMFiles::onWorkDone);
            connect(newWorker, &FindMMFilesWorker::workDone, newThread, &QThread::quit);
            connect(newWorker, &FindMMFilesWorker::workCanceled, this, &FindMMFiles::onWorkCanceled);
            connect(newWorker, &FindMMFilesWorker::workCanceled, newThread, &QThread::quit);
            connect(newThread, &QThread::finished, newThread, &QThread::deleteLater);
            newWorker->moveToThread(newThread);
            newThread->start();
            m_threadCount++;
        }
    }
    if (m_waitOnce) {
        m_waitOnce = false;
        qDebug() << "(FindMMFiles::doFindFiles) wait for dir:" << path;
    } else {
        m_threadCountLeft = m_threadCount;
        emit startWork();
        qDebug() << "(FindMMFiles::doFindFiles) work thread count:" << m_threadCountLeft;
    }
}

void FindMMFiles::onWorkDone(int allMMFilesCount, QFileInfoList musicFiles)
{
    m_allMMFilesCount = m_allMMFilesCount + allMMFilesCount;
    m_musicFiles << musicFiles;
    m_threadCountLeft--;
    qDebug() << "(FindMMFiles::onWorkDone) left work thread count:" << m_threadCountLeft;
    if (m_threadCountLeft == 0) {
        m_threadCount = 0;
        deleteWorkers();
        if (!CancelFlag) {
            qDebug() << "(FindMMFiles::onWorkDone) all work thread finished!!";
            emit findMMFilesFinished(m_allMMFilesCount, m_musicFiles);
        } else {
            qDebug() << "(FindMMFiles::onWorkDone) all work thread finished, but find files is canceled!!";
            emit findMMFilesCanceled();
        }
    }
}

void FindMMFiles::onWorkCanceled()
{
    m_threadCountLeft--;
    qDebug() << "(FindMMFiles::onWorkCanceled) left work thread count:" << m_threadCountLeft;
    if (m_threadCountLeft == 0) {
        m_threadCount = 0;
        deleteWorkers();
        qDebug() << "(FindMMFiles::onWorkCanceled) all work thread canceled!!";
        emit findMMFilesCanceled();
    }
}

bool FindMMFiles::isMusicFile(QFileInfo fileInfo)
{
    return MusicSuffix.contains(fileInfo.suffix(), Qt::CaseInsensitive);
}

void FindMMFiles::deleteWorkers()
{
    qDebug() << "(FindMMFiles::deleteWorkers) delete workers count:" << workers.count();
    foreach (auto worker, workers) {
        delete worker;
    }
    workers.clear();
}
