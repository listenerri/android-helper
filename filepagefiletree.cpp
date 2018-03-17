#include "filepagefiletree.h"
#include <QDebug>
#include <ddialog.h>
#include <QMenu>
#include <QInputDialog>
#include "tools/mtptool.h"
#include "currentdevicemanager.h"

DWIDGET_USE_NAMESPACE

FilePageFileTree::FilePageFileTree(QWidget *parent) : QWidget(parent)
{
    mainVBLayout = new QVBoxLayout(this);
    treeView = new QTreeView(this);
    fileSystemModel = new FileSystemModelFixed;

    fileSystemModel->setRootPath(MtpTool::MountPoint.absolutePath());
    treeView->setModel(fileSystemModel);
    treeView->setRootIndex(fileSystemModel->index(CurrentDeviceManager::getInstance()->getCurrentDevice()->getStoragePath()));

    treeView->setColumnWidth(0, 450);
    treeView->setIconSize(QSize(30, 30));
    treeView->setSelectionMode(QTreeView::ExtendedSelection);

    treeView->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(treeView, &QTreeView::customContextMenuRequested, this, &FilePageFileTree::showContextMenu);

    mainVBLayout->setContentsMargins(0, 0, 0, 0);
    mainVBLayout->addWidget(treeView);
    this->setLayout(mainVBLayout);
}

void FilePageFileTree::deleteSelected()
{
    // TODO: 删除音频文件时通知CurrentDevice
    // TODO: 如果文件和文件的父目录同时被选中,只删除父目录
    QModelIndexList selectedIndexes = treeView->selectionModel()->selectedRows();
    if (selectedIndexes.count() != 0) {
        QString delFilesName;
        foreach (QModelIndex modelIndex, selectedIndexes) {
            delFilesName += fileSystemModel->fileInfo(modelIndex).fileName() + "\n";
        }
        DDialog delDialog("Delete:", delFilesName, this);
        delDialog.addButton("Cancel");
        delDialog.addButton("Delete", true, DDialog::ButtonWarning);
        if (delDialog.exec() == DDialog::Accepted) {
            foreach (QModelIndex modelIndex, selectedIndexes) {
                QFileInfo fileInfo = fileSystemModel->fileInfo(modelIndex);
                if (fileInfo.isDir()) {
                    // 删除目录
                    if (fileSystemModel->rmdir(modelIndex)) {
                        qDebug() << "(FilePageFileTree::deleteSelected) dir deleted:" << fileInfo.absoluteFilePath();
                    } else {
                        qDebug() << "(FilePageFileTree::deleteSelected) dir delete failed:" << fileInfo.absoluteFilePath();
                    }
                } else if (fileInfo.isFile()) {
                    // 删除文件
                    if (fileSystemModel->remove(modelIndex)) {
                        qDebug() << "(FilePageFileTree::deleteSelected) file deleted:" << fileInfo.absoluteFilePath();
                    } else {
                        qDebug() << "(FilePageFileTree::deleteSelected) file delete failed:" << fileInfo.absoluteFilePath();
                    }
                }
            }
        } else {
            qDebug() << "(FilePageFileTree::deleteSelected) cancel delete selection.";
        }
    } else {
        qDebug() << "(FilePageFileTree::deleteSelected) no selected items.";
    }
}

void FilePageFileTree::showContextMenu()
{
    QMenu contextMenu(this);
    contextMenu.addAction("New dir");
    contextMenu.addAction("New dir in subdirectory");
    contextMenu.addSeparator();
    contextMenu.addAction("Delete");
    QAction *selectedAction = contextMenu.exec(QCursor::pos());
    if (selectedAction != 0) {
        if (selectedAction->text() == "New dir") {
            newDir(false);
        } else if (selectedAction->text() == "New dir in subdirectory") {
            newDir(true);
        } else if (selectedAction->text() == "Delete") {
            deleteSelected();
        }
    }
}

// 新建文件夹并滚动到其位置
void FilePageFileTree::newDir(bool inSub)
{
    QString text = QInputDialog::getText(this, "New dir", "input the name of new dir");
    if (text.count() != 0) {
        qDebug() << "(FilePageFileTree::showContextMenu) new dir name:" << text;
        QModelIndexList selectedIndexes = treeView->selectionModel()->selectedRows();
        if (selectedIndexes.count() != 0) {
            // 如果当前有选中项则取其第一个作为新建目录的index
            QModelIndex index = selectedIndexes.first();
            if (inSub) {
                // 新建选中项的子文件夹
                if (fileSystemModel->isDir(index)) {
                    treeView->scrollTo(fileSystemModel->mkdir(index,text));
                }
            } else {
                // 新建选中项的同级文件夹
                treeView->scrollTo(fileSystemModel->mkdir(index.parent(),text));
            }
        } else {
            // 如果当前没有选中项则使用treeview的root作为新建目录的index
            treeView->scrollTo(fileSystemModel->mkdir(treeView->rootIndex(),text));
        }
    }
}
