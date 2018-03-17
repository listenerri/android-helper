#ifndef SPLITLINE_H
#define SPLITLINE_H

#include <QFrame>

class SplitLine : public QFrame
{
public:
    enum SplitLineType {Vertical, Horizontal};
    SplitLine(SplitLineType type = Vertical, QWidget *parent = nullptr);
};

#endif // SPLITLINE_H
