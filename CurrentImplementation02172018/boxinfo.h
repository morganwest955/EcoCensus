#ifndef BOXINFO_H
#define BOXINFO_H

#include <QColor>

struct BoxInfo {
    QColor pencolor;
    int penwidth = 1;
    int x = 0;
    int y = 0;
    int w = 0;
    int h = 0;
};

#endif // BOXINFO_H
