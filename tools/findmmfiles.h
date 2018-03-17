#ifndef FINDFILES_H
#define FINDFILES_H

#include <QObject>
#include <QFileInfoList>
#include "findmmfilesworker.h"

class FindMMFiles : public QObject
{
    Q_OBJECT
public:
    explicit FindMMFiles(QObject *parent = nullptr);
    static bool CancelFlag;
    static QStringList MusicSuffix;
    static QStringList DirBlacklist;
    static QStringList DirRecursion;

public:
    static bool isMusicFile(QFileInfo fileInfo);

signals:
    void findMMFilesAlmostDone(int allMMFilesCount, QFileInfoList musicFiles);
    void findMMFilesFinished(int allMMFilesCount, QFileInfoList musicFiles);
    void startWork();
    void findMMFilesCanceled();

public slots:
    void doFindMMFiles(QString path);
    void onWorkDone(int allMMFilesCount, QFileInfoList musicFiles);
    void onWorkCanceled();

private:
    QList<FindMMFilesWorker*> workers;
    int m_threadCount;
    int m_threadCountLeft;
    int m_allMMFilesCount;
    QFileInfoList m_musicFiles;
    bool m_waitOnce;

private:
    void deleteWorkers();

};

#endif // FINDFILES_H
