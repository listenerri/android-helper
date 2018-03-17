#ifndef MUSICPAGENAV_H
#define MUSICPAGENAV_H

#include <QWidget>
#include <QButtonGroup>

class MusicPageNav : public QWidget
{
    Q_OBJECT
public:
    explicit MusicPageNav(QWidget *parent = nullptr);

signals:
    void currentIndexChanged(int index);

private slots:
    void onButtonClicked(int id);

private:
    QButtonGroup *m_buttonGroup;

};

#endif // MUSICPAGENAV_H
