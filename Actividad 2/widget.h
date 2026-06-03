#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QSlider>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QSpinBox>
#include <QMessageBox>
#include <QPainter>

#include "gamescene.h"

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void alLanzar();
    void alReiniciar();
    void alCambiarTurno(int jugador);
    void alTerminarJuego(int ganador);
    void actualizarInfoProyectil(double vx, double vy, double velocidad);
    void alCambiarAngulo(int valor);
    void alCambiarVelocidad(double valor);

private:
    void configurarUI();
    void actualizarControles();

    GameScene      *m_escena;
    QGraphicsView  *m_vista;

    QSlider        *m_sliderAngulo;
    QLabel         *m_labelAngulo;
    QDoubleSpinBox *m_spinVelocidad;
    QDoubleSpinBox *m_spinMasa;
    QPushButton    *m_botonLanzar;
    QPushButton    *m_botonReiniciar;

    QLabel *m_labelTurno;
    QLabel *m_labelInfo;
    QLabel *m_labelFisica;

    double m_angulo;
    double m_velocidad;
    double m_masa;
};

#endif // WIDGET_H
