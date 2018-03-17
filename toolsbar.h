#ifndef TOOLSBAR_H
#define TOOLSBAR_H

#include <QWidget>
#include <QGridLayout>
#include <QHBoxLayout>

class ToolsBar : public QWidget
{
    Q_OBJECT
public:
    explicit ToolsBar(QWidget *parent = nullptr);

    void addLeftWidget(QWidget *leftWidget);
    void addCenterWidget(QWidget *centerWidget);

signals:
    void deleteButtonClicked();

private:
    QGridLayout *mainGridLayout;

private slots:
    void onDeleteButtonClicked();
};

#endif // TOOLSBAR_H
