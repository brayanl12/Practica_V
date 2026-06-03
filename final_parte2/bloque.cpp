#include "bloque.h"
#include <QGraphicsScene>
#include <QColor>

Bloque::Bloque(qreal x, qreal y, qreal ancho, qreal alto, qreal res, qreal coef)
{
    setRect(0, 0, ancho, alto);
    setPos(x, y);
    resistencia = res;
    coefRestitucion = coef;
    destruido = false;
    actualizarColor();
}

void Bloque::recibirDaño(qreal daño)
{
    if (destruido) return;  // ya está destruido

    resistencia -= daño;

    if (resistencia <= 0) {
        destruido = true;
        resistencia = 0;
    }

    actualizarColor();
}

void Bloque::actualizarColor()
{
    // Cambiar color según la resistencia
    if (resistencia <= 0) {
        setBrush(QBrush(Qt::gray));  // bloque destruido
    } else if (resistencia < 20) {
        setBrush(QBrush(Qt::red));
    } else if (resistencia < 50) {
        setBrush(QBrush(Qt::yellow));
    } else {
        setBrush(QBrush(Qt::green));
    }
}
