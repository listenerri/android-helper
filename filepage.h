#ifndef FILEPAGE_H
#define FILEPAGE_H

#include <QWidget>
#include "toolsbar.h"
#include "filepagefiletree.h"
#include "filepagefiletreesd.h"

class FilePage : public QWidget
{
    Q_OBJECT
public:
    explicit FilePage(QWidget *parent = nullptr);

signals:

private:
    ToolsBar *m_toolsBar;
    FilePageFileTree *m_fileTree;
    FilePageFileTreeSD *m_fileTreeSD;

private slots:
    void resetPagesConnect(int currentPageIndex);
};

#endif // FILEPAGE_H
