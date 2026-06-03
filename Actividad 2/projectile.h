#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <cmath>

class Proyectil : public QGraphicsEllipseItem
{
public:
    explicit Proyectil(double x, double y, double vx, double vy,
                       double masa, int idJugador, QGraphicsItem *parent = nullptr);
    ~Proyectil() = default;

    void actualizar(double dt);

    double getX()    const { return m_x; }
    double getY()    const { return m_y; }
    double getVx()   const { return m_vx; }
    double getVy()   const { return m_vy; }
    double getMasa() const { return m_masa; }
    int    getIdJugador() const { return m_idJugador; }
    bool   estaActivo() const { return m_activo; }
    double getRadio() const { return m_radio; }

    void setVx(double vx) { m_vx = vx; }
    void setVy(double vy) { m_vy = vy; }
    void setActivo(bool a) { m_activo = a; }
    void setPosicion(double x, double y);

    double velocidad() const;

private:
    double m_x, m_y;
    double m_vx, m_vy;
    double m_masa;
    int    m_idJugador;
    bool   m_activo;
    double m_radio;

    static constexpr double GRAVEDAD = 0.0;
};

#endif // PROJECTILE_H
