#ifndef JUGADOR_H
#define JUGADOR_H

#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QString>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QColor>

class Jugador : public QGraphicsEllipseItem
{
public:
    explicit Jugador(double x, double y, int idJugador,
                     QGraphicsItem *parent = nullptr);

    int    getId()        const { return m_idJugador; }
    double getX()         const { return m_x; }
    double getY()         const { return m_y; }
    bool   estaVivo()      const { return m_vivo; }
    void   setVivo(bool v);
    double getRadio()    const { return m_radio; }

private:
    double m_x, m_y;
    int    m_idJugador;
    bool   m_vivo;
    double m_radio;
    QGraphicsTextItem *m_etiqueta;
};

#endif // JUGADOR_H
