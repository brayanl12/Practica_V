#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QTimer>
#include "jugador.h"
#include "proyectil.h"
#include "bloque.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void updateSimulation();
    void disparar();

private:
    Ui::MainWindow *ui;
    QGraphicsScene* scene;
    Jugador* jugadores[2];
    int turnoActual;
    Proyectil* proyectilActivo;
    QTimer* timer;
    const qreal FACTOR_DANO = 0.4;
    void colisionProyectilBloque(Proyectil* p, Bloque* b);
    void colisionProyectilPared(Proyectil* p);
    void siguienteTurno();
};

#endif // MAINWINDOW_H
