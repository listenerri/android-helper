#include "splitline.h"

SplitLine::SplitLine(SplitLine::SplitLineType type, QWidget *parent) : QFrame(parent)
{
    if (type == Vertical) {
        this->setFrameStyle(VLine | Raised);
        this->setLineWidth(1);
    } else {
        this->setFrameStyle(HLine | Raised);
        this->setLineWidth(1);
    }
    this->setContentsMargins(0, 0, 0, 0);
}
