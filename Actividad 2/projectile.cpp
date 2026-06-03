#include "projectile.h"

Proyectil::Proyectil(double x, double y, double vx, double vy,
                     double masa, int idJugador, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent),
    m_x(x), m_y(y), m_vx(vx), m_vy(vy),
    m_masa(masa), m_idJugador(idJugador),
    m_activo(true), m_radio(10.0)
{
    setRect(-m_radio, -m_radio, m_radio * 2, m_radio * 2);
    QGraphicsEllipseItem::setPos(m_x, m_y);

    if (idJugador == 1)
        setBrush(QBrush(QColor(220, 50, 50)));
    else
        setBrush(QBrush(QColor(50, 100, 220)));

    setPen(QPen(Qt::black, 1));
    setZValue(10);
}

void Proyectil::actualizar(double dt)
{
    if (!m_activo) return;
    m_x += m_vx * dt;
    m_y += m_vy * dt;
    QGraphicsEllipseItem::setPos(m_x, m_y);
}

void Proyectil::setPosicion(double x, double y)
{
    m_x = x;
    m_y = y;
    QGraphicsEllipseItem::setPos(m_x, m_y);
}

double Proyectil::velocidad() const
{
    return std::sqrt(m_vx * m_vx + m_vy * m_vy);
}
