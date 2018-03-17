#include "taglibtool.h"
#include <fileref.h>
#include <mpegfile.h>
#include <id3v2tag.h>
#include <attachedpictureframe.h>
#include <QDebug>
#include <QPixmap>

TaglibTool::TaglibTool(QObject *parent) : QObject(parent)
{

}

// 返回由path提供的音频文件的元数据组成的列表, 获取失败返回空列表
// TODO: 返回map更合适
QStringList TaglibTool::getMetaData(QString path)
{
    QStringList metaData;
    bool failed = false;
    TagLib::FileRef fileRef(path.toStdString().c_str());
    if (!fileRef.isNull()) {
        if (fileRef.tag()) {
            TagLib::Tag *tag = fileRef.tag();
            QString title, artist, album, year;
            title = QString::fromStdWString(tag->title().toWString());
            artist = QString::fromStdWString(tag->artist().toWString());
            album = QString::fromStdWString(tag->album().toWString());
            year = QString::number(tag->year());
            metaData << title << artist << album << year;
        } else {
            qDebug() << "(TaglibTool::getMetaData) get tag failed!!";
            failed = true;
        }
        if(fileRef.audioProperties()) {
            TagLib::AudioProperties *properties = fileRef.audioProperties();
            int seconds = properties->length() % 60;
            int minutes = (properties->length() - seconds) / 60;
            QString time = QString::number(minutes) + ":" + QString::number(seconds);
            metaData << time;
        } else {
            qDebug() << "(TaglibTool::getMetaData) get audio properties failed!!";
            failed = true;
        }
    } else {
        qDebug() << "(TaglibTool::getMetaData) FileRef is invalid!!";
    }

    if (failed) {
        metaData.clear();
        return metaData;
    } else {
        return metaData;
    }
}

QPixmap TaglibTool::getAlbumPicture(QString path)
{
    QPixmap albumPic;
    TagLib::MPEG::File file(path.toStdString().c_str());
    if (file.isValid()) {
        TagLib::ID3v2::FrameList frameList = file.ID3v2Tag()->frameListMap()["APIC"];
        if (!frameList.isEmpty()) {
            TagLib::ID3v2::AttachedPictureFrame *picFrame = static_cast<TagLib::ID3v2::AttachedPictureFrame *> (frameList.front());
            if (albumPic.loadFromData(QByteArray::fromRawData(picFrame->picture().data(), picFrame->picture().size()))) {
                return albumPic;
            } else {
                qDebug() << "(TaglibTool::getAlbumPicture) pixmap load from data failed!!" << path;
            }
        } else {
            qDebug() << "(TaglibTool::getAlbumPicture) no picture frames (APIC)!!" << path;
        }
    } else {
        qDebug() << "(TaglibTool::getAlbumPicture) file is invalid!!" << path;
    }
    return albumPic;
}
