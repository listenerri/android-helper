#include "filesystemmodelfixed.h"

QVariant FileSystemModelFixed::data(const QModelIndex &index, int role) const
{
    if (role == Qt::TextAlignmentRole && index.column() == 1) {
        // size列垂直居中
        return QVariant(Qt::AlignLeft | Qt::AlignVCenter);
    }
    return QFileSystemModel::data(index, role);
}
