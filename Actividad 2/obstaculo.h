#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QString>
#include <QBrush>
#include <QPen>
#include <QFont>
#include <QColor>

class Obstaculo : public QGraphicsRectItem
{
public:
    explicit Obstaculo(double x, double y, double w, double h,
                       int resistencia, int dueno,
                       QGraphicsItem *parent = nullptr);

    void aplicarDano(double dano);

    double getX()      const { return m_x; }
    double getY()      const { return m_y; }
    double getW()      const { return m_w; }
    double getH()      const { return m_h; }
    int    getResistencia() const { return m_resistencia; }
    bool   estaDestruido()   const { return m_resistencia <= 0; }
    int    getDueno() const { return m_dueno; }

    void actualizarVisual();

    QGraphicsTextItem *m_etiqueta;

private:
    double m_x, m_y, m_w, m_h;
    int    m_resistencia;
    int    m_dueno;
    int    m_resistenciaInicial;
};

#endif // OBSTACULO_H
