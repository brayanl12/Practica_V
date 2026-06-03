#include "gamescene.h"
#include <cmath>
#include <QDebug>

GameScene::GameScene(QObject *parent)
    : QGraphicsScene(parent),
    m_proyectil(nullptr),
    m_jugadorActual(1),
    m_simulando(false),
    m_juegoTerminado(false),
    m_ganador(0)
{
    setSceneRect(0, 0, ANCHO_ESCENA, ALTO_ESCENA);

    m_timer = new QTimer(this);
    m_timer->setInterval(16);
    connect(m_timer, &QTimer::timeout, this, &GameScene::pasoSimulacion);

    configurarEscena();
}

GameScene::~GameScene()
{
    if (m_timer) {
        m_timer->stop();
    }
}

void GameScene::configurarEscena()
{
    // Limpiar cualquier cosa existente primero
    clear();

    QGraphicsRectItem *fondo = addRect(0, 0, ANCHO_ESCENA, ALTO_ESCENA,
                                       QPen(Qt::black, 3),
                                       QBrush(QColor(30, 35, 45)));
    fondo->setZValue(-10);

    QGraphicsRectItem *lineaMedia = addRect(ANCHO_ESCENA / 2 - 1, 0, 2, ALTO_ESCENA,
                                            QPen(Qt::NoPen),
                                            QBrush(QColor(80, 80, 80, 120)));
    lineaMedia->setZValue(-5);

    // Obstáculos Jugador 1 (izquierda)
    Obstaculo *o1 = new Obstaculo(60,   200, 100, 30,  200, 1);
    Obstaculo *o2 = new Obstaculo(60,   350, 100, 30,  200, 1);
    Obstaculo *o3 = new Obstaculo(120,  270, 80,  30,  100, 1);

    // Obstáculos Jugador 2 (derecha)
    Obstaculo *o4 = new Obstaculo(740,  200, 100, 30,  200, 2);
    Obstaculo *o5 = new Obstaculo(740,  350, 100, 30,  200, 2);
    Obstaculo *o6 = new Obstaculo(700,  270, 80,  30,  100, 2);

    m_obstaculos.clear();
    m_obstaculos << o1 << o2 << o3 << o4 << o5 << o6;

    for (auto *o : m_obstaculos) {
        if (o) addItem(o);
    }

    // Jugadores
    Jugador *j1 = new Jugador(60, 290, 1);
    Jugador *j2 = new Jugador(840, 290, 2);

    m_jugadores.clear();
    m_jugadores << j1 << j2;

    for (auto *j : m_jugadores) {
        if (j) addItem(j);
    }

    // Etiquetas
    auto crearEtiqueta = [&](const QString &texto, double x, double y, QColor color) {
        QGraphicsTextItem *t = addText(texto);
        if (t) {
            t->setDefaultTextColor(color);
            QFont f;
            f.setBold(true);
            f.setPointSize(11);
            t->setFont(f);
            t->setPos(x, y);
            t->setZValue(20);
        }
    };
    crearEtiqueta("Jugador 1", 20, 10, QColor(220, 90, 90));
    crearEtiqueta("Jugador 2", 760, 10, QColor(90, 110, 220));
}

void GameScene::lanzarProyectil(double anguloGrados, double velocidad, double masa)
{
    if (m_simulando || m_juegoTerminado) return;

    // Limpiar proyectil anterior
    if (m_proyectil) {
        removeItem(m_proyectil);
        delete m_proyectil;
        m_proyectil = nullptr;
    }

    // Limpiar estela
    for (auto *t : m_estelaItems) {
        if (t) {
            removeItem(t);
            delete t;
        }
    }
    m_estelaItems.clear();

    // Posición de inicio - SALE DESDE EL JUGADOR CON UN OFFSET
    double inicioX, inicioY;
    double offset = 25.0;  // Distancia desde el centro del jugador

    for (auto *j : m_jugadores) {
        if (j && j->getId() == m_jugadorActual) {
            inicioX = j->getX();
            inicioY = j->getY();

            // Agregar offset en la dirección del disparo
            if (m_jugadorActual == 1) {
                inicioX += offset;  // Hacia la derecha
            } else {
                inicioX -= offset;  // Hacia la izquierda
            }
            break;
        }
    }

    double rad = anguloGrados * M_PI / 180.0;
    double vx = velocidad * std::cos(rad);
    double vy = velocidad * std::sin(rad);

    // Dirección según el jugador
    if (m_jugadorActual == 2) {
        vx = -std::abs(vx);
    } else {
        vx = std::abs(vx);
    }

    m_proyectil = new Proyectil(inicioX, inicioY, vx, vy, masa, m_jugadorActual);

    if (m_proyectil) {
        addItem(m_proyectil);
        m_simulando = true;
        m_timer->start();
    }
}

void GameScene::pasoSimulacion()
{
    // VERIFICACIÓN CRÍTICA DE SEGURIDAD
    if (!m_proyectil) {
        terminarSimulacion();
        return;
    }

    if (!m_proyectil->estaActivo()) {
        terminarSimulacion();
        return;
    }

    // Actualizar posición
    m_proyectil->actualizar(DT);

    // Agregar punto de estela (con protección)
    QGraphicsEllipseItem *punto = addEllipse(
        m_proyectil->getX() - 2,
        m_proyectil->getY() - 2,
        4, 4,
        QPen(Qt::NoPen),
        QBrush(m_jugadorActual == 1 ? QColor(220, 80, 80, 120) : QColor(80, 100, 220, 120)));

    if (punto) {
        punto->setZValue(8);
        m_estelaItems.append(punto);
    }

    // Limitar estela
    while (m_estelaItems.size() > 80) {
        QGraphicsItem *viejo = m_estelaItems.takeFirst();
        if (viejo) {
            removeItem(viejo);
            delete viejo;
        }
    }

    // Emitir información (solo si proyectil existe)
    if (m_proyectil) {
        emit infoProyectilActualizada(m_proyectil->getVx(),
                                      m_proyectil->getVy(),
                                      m_proyectil->velocidad());
    }

    verificarColisiones();
}

void GameScene::verificarColisiones()
{
    if (!m_proyectil) return;
    if (!m_proyectil->estaActivo()) return;

    verificarColisionPared(m_proyectil);

    // Verificar obstáculos
    for (auto *obs : m_obstaculos) {
        if (obs && !obs->estaDestruido()) {
            verificarColisionObstaculo(m_proyectil, obs);
            if (!m_proyectil->estaActivo()) break;
        }
    }

    // Verificar jugadores
    if (m_proyectil && m_proyectil->estaActivo()) {
        for (auto *j : m_jugadores) {
            if (j && j->estaVivo()) {
                verificarColisionJugador(m_proyectil, j);
                if (!m_proyectil->estaActivo()) break;
            }
        }
    }
}

void GameScene::verificarColisionPared(Proyectil *p)
{
    if (!p) return;

    double r = p->getRadio();
    double px = p->getX();
    double py = p->getY();

    if (px - r <= 0) {
        p->setPosicion(r, py);
        p->setVx(std::abs(p->getVx()));
    } else if (px + r >= ANCHO_ESCENA) {
        p->setPosicion(ANCHO_ESCENA - r, py);
        p->setVx(-std::abs(p->getVx()));
    }

    if (py - r <= 0) {
        p->setPosicion(p->getX(), r);
        p->setVy(std::abs(p->getVy()));
    } else if (py + r >= ALTO_ESCENA) {
        p->setPosicion(p->getX(), ALTO_ESCENA - r);
        p->setVy(-std::abs(p->getVy()));
    }
}

void GameScene::verificarColisionObstaculo(Proyectil *p, Obstaculo *obs)
{
    if (!p || !obs) return;
    if (!p->estaActivo()) return;

    double px = p->getX();
    double py = p->getY();
    double r = p->getRadio();

    double ox = obs->getX();
    double oy = obs->getY();
    double ow = obs->getW();
    double oh = obs->getH();

    // Verificar colisión AABB
    if (px + r < ox || px - r > ox + ow) return;
    if (py + r < oy || py - r > oy + oh) return;

    // Calcular solapamiento
    double solapeIzq = (px + r) - ox;
    double solapeDer = (ox + ow) - (px - r);
    double solapeArr = (py + r) - oy;
    double solapeAba = (oy + oh) - (py - r);

    double minSolapeX = std::min(solapeIzq, solapeDer);
    double minSolapeY = std::min(solapeArr, solapeAba);

    // Calcular daño
    double vImpacto = p->velocidad();
    double dano = FACTOR_DANO * p->getMasa() * vImpacto;
    obs->aplicarDano(dano);

    // Aplicar rebote según la cara
    if (minSolapeX < minSolapeY) {
        double nuevaVx = -E_OBSTACULO * p->getVx();
        p->setVx(nuevaVx);
        if (solapeIzq < solapeDer) {
            p->setPosicion(ox - r - 0.5, py);
        } else {
            p->setPosicion(ox + ow + r + 0.5, py);
        }
    } else {
        double nuevaVy = -E_OBSTACULO * p->getVy();
        p->setVy(nuevaVy);
        if (solapeArr < solapeAba) {
            p->setPosicion(px, oy - r - 0.5);
        } else {
            p->setPosicion(px, oy + oh + r + 0.5);
        }
    }
}

void GameScene::verificarColisionJugador(Proyectil *p, Jugador *j)
{
    if (!p || !j) return;
    if (j->getId() == m_jugadorActual) return;

    double dx = p->getX() - j->getX();
    double dy = p->getY() - j->getY();
    double distancia = std::sqrt(dx * dx + dy * dy);

    if (distancia < p->getRadio() + j->getRadio()) {
        j->setVivo(false);
        p->setActivo(false);

        m_juegoTerminado = true;
        m_ganador = m_jugadorActual;

        if (m_timer) m_timer->stop();

        emit juegoTerminadoSignal(m_ganador);

        // Limpiar proyectil inmediatamente
        if (m_proyectil) {
            removeItem(m_proyectil);
            delete m_proyectil;
            m_proyectil = nullptr;
        }
    }
}

bool GameScene::todosObstaculosDestruidosPara(int idJugador)
{
    for (auto *o : m_obstaculos) {
        if (o && o->getDueno() == idJugador && !o->estaDestruido()) {
            return false;
        }
    }
    return true;
}

void GameScene::terminarSimulacion()
{
    if (m_timer) {
        m_timer->stop();
    }

    m_simulando = false;

    if (m_proyectil) {
        removeItem(m_proyectil);
        delete m_proyectil;
        m_proyectil = nullptr;
    }

    if (!m_juegoTerminado) {
        m_jugadorActual = (m_jugadorActual == 1) ? 2 : 1;
        emit turnoCambiado(m_jugadorActual);
    }
}

void GameScene::reiniciarJuego()
{
    // Detener todo
    if (m_timer) {
        m_timer->stop();
    }

    m_simulando = false;
    m_juegoTerminado = false;
    m_ganador = 0;
    m_jugadorActual = 1;

    // Limpiar proyectil
    if (m_proyectil) {
        removeItem(m_proyectil);
        delete m_proyectil;
        m_proyectil = nullptr;
    }

    // Limpiar estela
    for (auto *t : m_estelaItems) {
        if (t) {
            removeItem(t);
            delete t;
        }
    }
    m_estelaItems.clear();

    // Limpiar obstáculos
    for (auto *o : m_obstaculos) {
        if (o) {
            removeItem(o);
            delete o;
        }
    }
    m_obstaculos.clear();

    // Limpiar jugadores
    for (auto *j : m_jugadores) {
        if (j) {
            removeItem(j);
            delete j;
        }
    }
    m_jugadores.clear();

    // Limpiar toda la escena
    clear();

    // Reconfigurar
    configurarEscena();

    emit turnoCambiado(m_jugadorActual);
}
