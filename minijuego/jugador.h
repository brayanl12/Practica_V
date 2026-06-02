#ifndef JUGADOR_H
#define JUGADOR_H

#include <QString>
#include <QPointF>

// -------------------------------------------------------
// Jugador
// Almacena el estado de cada jugador:
//   - posición del cañón
//   - parámetros del próximo disparo (ángulo, rapidez, masa)
//   - si sigue vivo
// -------------------------------------------------------
class Jugador
{
public:
    Jugador(int id, const QString &nombre, QPointF posCanion);

    int     id()          const { return m_id; }
    QString nombre()      const { return m_nombre; }
    QPointF posCanion()   const { return m_posCanion; }

    double  angulo()      const { return m_angulo; }   // grados
    double  rapidez()     const { return m_rapidez; }  // px/s
    double  masa()        const { return m_masa; }     // kg

    void    setAngulo (double a) { m_angulo  = a; }
    void    setRapidez(double r) { m_rapidez = r; }
    void    setMasa   (double m) { m_masa    = m; }

    bool    estaVivo()    const  { return m_vivo; }
    void    eliminar()           { m_vivo = false; }

    // Descompone rapidez y ángulo en (vx, vy)
    // direccion: +1 → dispara hacia la derecha (J1)
    //            -1 → dispara hacia la izquierda (J2)
    void calcularVelocidad(int direccion, double &vx, double &vy) const;

private:
    int     m_id;
    QString m_nombre;
    QPointF m_posCanion;
    double  m_angulo;
    double  m_rapidez;
    double  m_masa;
    bool    m_vivo;
};

#endif // JUGADOR_H
