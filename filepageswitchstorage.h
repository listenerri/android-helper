#ifndef FILEPAGESWITCHSTORAGE_H
#define FILEPAGESWITCHSTORAGE_H

#include <QWidget>

class FilePageSwitchStorage : public QWidget
{
    Q_OBJECT
public:
    explicit FilePageSwitchStorage(QWidget *parent = nullptr);

signals:
    void switchStoragePage(int index);

private slots:
    void onButtonClicked(int id);
};

#endif // FILEPAGESWITCHSTORAGE_H
