#ifndef PROYECTIL_H
#define PROYECTIL_H

#include <QGraphicsEllipseItem>
#include <QPointF>

class Proyectil : public QGraphicsEllipseItem
{
public:
    Proyectil(qreal x, qreal y, qreal vx, qreal vy, qreal masa);
    QPointF velocidad;
    qreal masa;
    bool activa;
    void avanzar();
    void aplicarGravedad();
};

#endif // PROYECTIL_H
