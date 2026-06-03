#include "cannon.h"
#include <QPixmap>

Cannon::Cannon(qreal x, qreal y)
{
    setPixmap(QPixmap(":/imagen/cannon.png").scaled(40, 40));
    setPos(x, y);
}


