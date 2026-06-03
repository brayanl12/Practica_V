#ifndef BLOQUE_H
#define BLOQUE_H

#include <QGraphicsRectItem>
#include <QBrush>

class Bloque : public QGraphicsRectItem
{
public:
    Bloque(qreal x, qreal y, qreal ancho, qreal alto, qreal resistencia, qreal coefRestitucion = 0.7);
    qreal resistencia;
    qreal coefRestitucion;
    void recibirDaño(qreal daño);
    bool destruido;
    void actualizarColor();
};

#endif // BLOQUE_H
