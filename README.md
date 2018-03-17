## 简介
基于c++/qt, 用于管理Android手机上的文件(多媒体文件).

## 依赖
``` bash
# deepin linux
apt install adb go-mtpfs libdtkwidget2 libdtkwidget-dev libtag1-dev libtag1v5
```

## 依赖介绍
- adb程序检测手机的插入和拔出
- go-mtpfs挂载设备存储
- fusermount卸载设备存储
- taglib获取歌曲信息

## 使用
手机端需要进入开发者选项中开启USB调试, 然后使用qt-creator导入项目后运行即可

## 待完善
- 界面美化
- device页面中添加设备详细信息
- 加载文件的速度(多线程和mtp工具的性能)
- 文件管理页面删除音乐文件时同步移除音乐页面中的数据

## 待增加功能
- 图片浏览
- 下载目录
