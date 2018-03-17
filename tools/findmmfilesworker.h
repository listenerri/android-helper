#ifndef FINDFILESWORKER_H
#define FINDFILESWORKER_H

#include <QObject>
#include <QFileInfoList>

class FindMMFilesWorker : public QObject
{
    Q_OBJECT
public:
    FindMMFilesWorker(QObject *parent = nullptr);

public:
    void setPath(const QString &path);

signals:
    void workCanceled();
    void workDone(int allMMFilesCount, QFileInfoList musicFiles);

public slots:
    void doWork();

private:
    QString m_path;

};

#endif // FINDFILESWORKER_H
