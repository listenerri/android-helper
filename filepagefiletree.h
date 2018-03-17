#ifndef FILEPAGEFILELIST_H
#define FILEPAGEFILELIST_H

#include <QWidget>
#include <QVBoxLayout>
#include <QTreeView>
#include "filesystemmodelfixed.h"

class FilePageFileTree : public QWidget
{
    Q_OBJECT

public:
    FilePageFileTree(QWidget *parent = nullptr);

public slots:
    void deleteSelected();

protected:
    QVBoxLayout *mainVBLayout;
    QTreeView *treeView;
    FileSystemModelFixed *fileSystemModel;

private slots:
    void showContextMenu();

private:
    void newDir(bool inSub);
};

#endif // FILEPAGEFILELIST_H
