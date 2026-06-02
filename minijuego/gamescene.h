#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QList>
#include "projectile.h"
#include "obstaculo.h"
#include "jugador.h"

// -------------------------------------------------------
// EscenaJuego (GameScene)
// Núcleo del juego. Hereda QGraphicsScene y gestiona:
//   - Loop de simulación por QTimer
//   - Detección y resolución de los 3 tipos de colisión
//   - Turnos y condición de victoria
// -------------------------------------------------------
class GameScene : public QGraphicsScene
{
    Q_OBJECT
public:
    static constexpr double ANCHO_ESCENA  = 900.0;
    static constexpr double ALTO_ESCENA   = 500.0;
    static constexpr double E_OBSTACULO   = 0.5;  // coef. de restitución

    explicit GameScene(QObject *parent = nullptr);
    ~GameScene();

    void    iniciarJuego();
    void    disparar(double angulo, double rapidez, double masa);

    Jugador* jugador(int idx)    { return m_jugadores[idx]; }
    int      turnoActual()  const { return m_turnoActual; }
    bool     juegoTerminado() const { return m_juegoTerminado; }

signals:
    void turnoChanged(int nuevoTurno);
    void obstaculoGolpeado(int duenio, double resistenciaRestante);
    void rivalEliminado(int jugadorPerdedor);
    void proyectilTermino();
    void mensajeEstado(const QString &msg);

private slots:
    void pasarSimulacion();

private:
    // Colisiones
    void verificarColisionParedes();
    void verificarColisionObstaculos();
    void verificarColisionRivales();
    void resolverImpactoObstaculo(Obstaculo *obs);

    // Escena
    void construirEscena();
    void siguienteTurno();

    QTimer      *m_temporizador;
    Projectile  *m_proyectil;        // nullptr si no hay proyectil activo
    Jugador     *m_jugadores[2];
    QList<Obstaculo*> m_obstaculos;
    QGraphicsItemGroup *m_rivales[2];

    int  m_turnoActual;
    bool m_juegoTerminado;
};

#endif // GAMESCENE_H
