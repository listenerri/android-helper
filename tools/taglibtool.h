#ifndef TAGLIBTOOL_H
#define TAGLIBTOOL_H

#include <QObject>

class TaglibTool : public QObject
{
    Q_OBJECT
public:
    explicit TaglibTool(QObject *parent = nullptr);

    static QStringList getMetaData(QString path);
    static QPixmap getAlbumPicture(QString path);

signals:

public slots:
};

#endif // TAGLIBTOOL_H
