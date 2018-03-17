#ifndef FILESYSTEMMODELFIXED_H
#define FILESYSTEMMODELFIXED_H

#include <QFileSystemModel>

class FileSystemModelFixed : public QFileSystemModel
{

    // QAbstractItemModel interface
public:
    QVariant data(const QModelIndex &index, int role) const;
};

#endif // FILESYSTEMMODELFIXED_H
