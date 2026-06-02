#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTimer>
#include <QPushButton>
#include <QLabel>
#include "Simulacion.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget {
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

protected:
    // Qt llama a paintEvent cada vez que hay que redibujar
    void paintEvent(QPaintEvent *event) override;

private slots:
    // Se ejecuta en cada tick del QTimer
    void avanzarSimulacion();

    // Botones
    void iniciarSimulacion();
    void pausarSimulacion();
    void reiniciarSimulacion();
    void guardarResultados();

private:
    Ui::Widget    *ui;
    Simulacion    *sim;       // Motor de física
    QTimer        *timer;     // Controla los pasos de tiempo

    // Parámetros de la simulación
    static constexpr double DT          = 0.05;   // paso de tiempo (segundos)
    static constexpr int    INTERVALO   = 30;     // ms entre frames (~33 fps)
    static constexpr double ANCHO_CAJA  = 760.0;
    static constexpr double ALTO_CAJA   = 520.0;

    // Colores de las partículas (uno por índice)
    static const QColor COLORES_PARTICULAS[];

    // Botones de control (los creamos en código, no en el .ui)
    QPushButton *botonIniciar;
    QPushButton *botonPausar;
    QPushButton *botonReiniciar;
    QPushButton *botonGuardar;
    QLabel      *etiquetaTiempo;
    QLabel      *etiquetaEventos;

    // Offset para centrar la caja de simulación
    static constexpr int OFFSET_X = 20;
    static constexpr int OFFSET_Y = 60;

    void configurarSimulacion();
    void configurarUI();

    // Convierte coordenadas de simulación → coordenadas de pantalla
    int simX(double x) const { return static_cast<int>(x) + OFFSET_X; }
    int simY(double y) const { return static_cast<int>(y) + OFFSET_Y; }
};

#endif // WIDGET_H
