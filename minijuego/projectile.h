#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <QGraphicsEllipseItem>
#include <QPointF>

// -------------------------------------------------------
// Proyectil
// Representa la bala lanzada por cada jugador.
// Usa movimiento parabólico en tiempo discreto:
//   vx = constante (MRU)
//   vy += g * dt   (caída libre, eje Y hacia abajo en Qt)
//   x  += vx * dt
//   y  += vy * dt
// -------------------------------------------------------
class Projectile : public QGraphicsEllipseItem
{
public:
    static constexpr double GRAVEDAD = 980.0; // px/s^2
    static constexpr double DT       = 0.016; // ~60 fps
    static constexpr double RADIO    = 10.0;  // px

    Projectile(double masa,
               double x0, double y0,
               double vx0, double vy0,
               int jugadorDuenio);

    // Avanza un paso de simulación
    bool avanzar();

    // Getters
    double  vx()       const { return m_vx; }
    double  vy()       const { return m_vy; }
    double  masa()     const { return m_masa; }
    double  rapidez()  const;
    int     duenio()   const { return m_duenio; }
    QPointF centro()   const;

    // Rebote elástico en paredes (invierte componente)
    void rebotarX() { m_vx = -m_vx; }
    void rebotarY() { m_vy = -m_vy; }

    // Restitución inelástica en obstáculos
    void aplicarRestitucionX(double e) { m_vx = -e * m_vx; }
    void aplicarRestitucionY(double e) { m_vy = -e * m_vy; }

    void desactivar()     { m_activo = false; }
    bool estaActivo() const { return m_activo; }

private:
    double m_masa;
    double m_vx, m_vy;
    double m_x,  m_y;
    int    m_duenio;
    bool   m_activo;

    void sincronizarPosicion();
};

#endif // PROJECTILE_H
