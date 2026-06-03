#include "proyectil.h"
#include <QBrush>

Proyectil::Proyectil(qreal x, qreal y, qreal vx, qreal vy, qreal m)
{
    setRect(0, 0, 14, 14);
    setBrush(QBrush(Qt::red));

    setPos(x, y);

    velocidad = QPointF(vx, vy);
    masa = m;
    activa = true;
}

void Proyectil::aplicarGravedad()
{
    // gravedad constante
    velocidad.setY(velocidad.y() + 0.3);
}

void Proyectil::avanzar()
{
    aplicarGravedad();
    setX(x() + velocidad.x());
    setY(y() + velocidad.y());
}
