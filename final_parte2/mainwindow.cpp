#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QGraphicsRectItem>
#include <QMessageBox>
#include <cmath>
#include <QRandomGenerator>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    scene = new QGraphicsScene(0, 0, 800, 600, this);
    ui->graphicsView->setScene(scene);

    // Crear un rectángulo que represente los bordes de la escena
    QGraphicsRectItem* borde = new QGraphicsRectItem(0, 0, scene->width(), scene->height());
    QPen pen(Qt::black);
    pen.setWidth(4); // grosor del borde
    borde->setPen(pen);
    borde->setBrush(Qt::NoBrush); // transparente por dentro
    scene->addItem(borde);

    // Crear cañones
    Cannon* c1 = new Cannon(15, 180);
    Cannon* c2 = new Cannon(735, 180);
    scene->addItem(c1);
    scene->addItem(c2);

    // Crear jugadores
    jugadores[0] = new Jugador("Jugador 1", c1);
    jugadores[1] = new Jugador("Jugador 2", c2);

    //Crear personaje 1
    QGraphicsPixmapItem* personaje1 = new QGraphicsPixmapItem();
    personaje1->setPixmap(QPixmap(":/imagen/p1.png").scaled(80, 80));
    personaje1->setPos(165, 510);
    scene->addItem(personaje1);
    jugadores[0]->setPersonaje(personaje1);

    //Crear personaje 2
    QGraphicsPixmapItem* personaje2 = new QGraphicsPixmapItem();
    personaje2->setPixmap(QPixmap(":/imagen/p2.png").scaled(80, 80));
    personaje2->setPos(575, 510);
    scene->addItem(personaje2);
    jugadores[1]->setPersonaje(personaje2);

    //Soporte cannon 1
    Bloque* b0 = new Bloque(0, 210, 80, 30, 100);
    b0->setBrush(QBrush(Qt::black));
    scene->addItem(b0);

    //Soporte cannon 2
    Bloque* b01 = new Bloque(720, 210, 80, 30, 100);
    b01->setBrush(QBrush(Qt::black));
    scene->addItem(b01);

    // Crear bloques para cada jugador
    //Jugador 1
    Bloque* b1 = new Bloque(130, 450, 150, 40, 100);
    scene->addItem(b1);
    jugadores[0]->bloques.append(b1);

    Bloque* b2 = new Bloque(130, 490, 40, 150, 100);
    scene->addItem(b2);
    jugadores[0]->bloques.append(b2);

    Bloque* b3 = new Bloque(240, 490, 40, 150, 100);
    scene->addItem(b3);
    jugadores[0]->bloques.append(b3);

    //Jugador 2
    Bloque* b4 = new Bloque(540, 450, 150, 40, 100);
    scene->addItem(b4);
    jugadores[1]->bloques.append(b4);

    Bloque* b5 = new Bloque(540, 490, 40, 150, 100);
    scene->addItem(b5);
    jugadores[1]->bloques.append(b5);

    Bloque* b6 = new Bloque(650, 490, 40, 150, 100);
    scene->addItem(b6);
    jugadores[1]->bloques.append(b6);

    turnoActual = 0;
    proyectilActivo = nullptr;

    // Timer
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateSimulation);
    timer->start(20);

    // Botón disparar
    connect(ui->dispararButton, &QPushButton::clicked, this, &MainWindow::disparar);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::disparar()
{
    if (proyectilActivo) return; // Ya hay un proyectil en vuelo

    Jugador* j = jugadores[turnoActual];
    qreal ang = ui->anguloSpin->value() * M_PI / 180.0;
    qreal vel = ui->velocidadSpin->value();

    qreal vx = vel * qCos(ang);
    qreal vy = -vel * qSin(ang); // hacia arriba

    // Invertir X si es el jugador 2 para que dispare hacia la izquierda
    if (turnoActual == 1) vx = -vx;

    proyectilActivo = new Proyectil(j->cannon->x(), j->cannon->y(), vx, vy, 5.0);
    scene->addItem(proyectilActivo);
}

void MainWindow::updateSimulation()
{
    if (!proyectilActivo) return;

    proyectilActivo->avanzar();

    // 1. Revisar colisión con PERSONAJE primero
    Jugador* objetivo = jugadores[1 - turnoActual];

    if (proyectilActivo->collidesWithItem(objetivo->personaje)) {

        objetivo->recibirImpacto();

        QMessageBox::information(this, "¡Victoria!", jugadores[turnoActual]->nombre + " ha ganado!");

        timer->stop();

        // Eliminar proyectiles activos
        if (proyectilActivo) {
            scene->removeItem(proyectilActivo);
            delete proyectilActivo;
            proyectilActivo = nullptr;
        }

        // Limpiar bloques
        for (int i = 0; i < 2; ++i) {
            auto& bloques = jugadores[i]->bloques;
            for (Bloque* b : bloques) {
                if (b->scene()) scene->removeItem(b);
                delete b;
            }
            bloques.clear();
        }

        // Eliminar jugadores
        for (int i = 0; i < 2; ++i) {
            delete jugadores[i];
            jugadores[i] = nullptr;
        }

        qApp->quit();
    }

    // Colisiones con paredes
    colisionProyectilPared(proyectilActivo);

    // Colisiones con bloques
    for (Bloque* b : jugadores[1-turnoActual]->bloques) {
        colisionProyectilBloque(proyectilActivo, b);
    }

    // Revisar si el proyectil toca el piso
    qreal piso = scene->height() - proyectilActivo->rect().height();
    if (proyectilActivo->y() >= piso) {
        proyectilActivo->setY(piso);  // Asegurar que quede justo en el piso
        scene->removeItem(proyectilActivo);
        delete proyectilActivo;
        proyectilActivo = nullptr;
        siguienteTurno();  // Pasar el turno
    }

    // Limpiar bloques destruidos de la escena y la lista
    auto& bloques = jugadores[1 - turnoActual]->bloques;
    for (int i = bloques.size() - 1; i >= 0; --i) {
        if (bloques[i]->destruido) {
            if (bloques[i]->scene())
                bloques[i]->scene()->removeItem(bloques[i]);
            delete bloques[i];
            bloques.removeAt(i);
        }
    }
}

void MainWindow::colisionProyectilPared(Proyectil* p)
{
    QRectF r = p->sceneBoundingRect();

    if (r.left() < 0 || r.right() > scene->width()) {
        p->velocidad.setX(-p->velocidad.x());
    }
    if (r.top() < 0 || r.bottom() > scene->height()) {
        p->velocidad.setY(-p->velocidad.y());
    }
}

void MainWindow::colisionProyectilBloque(Proyectil* p, Bloque* b)
{
    if (!p->collidesWithItem(b)) return;

    qreal velocidadMag = qSqrt(p->velocidad.x()*p->velocidad.x() + p->velocidad.y()*p->velocidad.y());
    qreal dano = FACTOR_DANO * p->masa * velocidadMag;

    b->recibirDaño(dano);

    // Rebote inelástico simple
    QRectF pb = p->sceneBoundingRect();
    QRectF ob = b->sceneBoundingRect();

    qreal distLeft   = qAbs(pb.right() - ob.left());
    qreal distRight  = qAbs(ob.right() - pb.left());
    qreal distTop    = qAbs(pb.bottom() - ob.top());
    qreal distBottom = qAbs(ob.bottom() - pb.top());

    qreal minDist = std::min({distLeft, distRight, distTop, distBottom});

    if (minDist == distLeft || minDist == distRight)
        p->velocidad.setX(-p->velocidad.x() * b->coefRestitucion);
    else
        p->velocidad.setY(-p->velocidad.y() * b->coefRestitucion);
}

void MainWindow::siguienteTurno()
{
    turnoActual = 1 - turnoActual;
}

