#ifndef GAMESCENE_H
#define GAMESCENE_H

#include <QGraphicsScene>
#include <QTimer>
#include <QVector>
#include <QObject>
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>
#include <QColor>
#include <QFont>
#include <QGraphicsTextItem>

#include "projectile.h"
#include "obstaculo.h"
#include "jugador.h"

static constexpr double ANCHO_ESCENA = 900.0;
static constexpr double ALTO_ESCENA = 600.0;
static constexpr double E_OBSTACULO = 0.4;
static constexpr double FACTOR_DANO = 0.05;
static constexpr double DT = 0.016;

class GameScene : public QGraphicsScene
{
    Q_OBJECT

public:
    explicit GameScene(QObject *parent = nullptr);
    ~GameScene();

    void lanzarProyectil(double anguloGrados, double velocidad, double masa);
    int  jugadorActual() const { return m_jugadorActual; }
    bool estaSimulando()  const { return m_simulando; }
    bool juegoTerminado()    const { return m_juegoTerminado; }
    int  ganador()        const { return m_ganador; }

    void reiniciarJuego();

signals:
    void turnoCambiado(int jugador);
    void juegoTerminadoSignal(int ganador);
    void infoProyectilActualizada(double vx, double vy, double velocidad);

private slots:
    void pasoSimulacion();

private:
    void configurarEscena();
    void verificarColisiones();
    void verificarColisionPared(Proyectil *p);
    void verificarColisionObstaculo(Proyectil *p, Obstaculo *obs);
    void verificarColisionJugador(Proyectil *p, Jugador *j);
    bool todosObstaculosDestruidosPara(int idJugador);
    void terminarSimulacion();

    QTimer        *m_timer;
    Proyectil    *m_proyectil;

    QVector<Obstaculo *> m_obstaculos;
    QVector<Jugador   *> m_jugadores;

    int  m_jugadorActual;
    bool m_simulando;
    bool m_juegoTerminado;
    int  m_ganador;

    QVector<QGraphicsItem *> m_estelaItems;
};

#endif // GAMESCENE_H
