#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QGraphicsView>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QPushButton>
#include <QGroupBox>
#include "gamescene.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

// -------------------------------------------------------
// Widget
// Ventana principal: contiene la vista de la escena
// y el panel de control (ángulo, rapidez, masa, disparo).
// -------------------------------------------------------
class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:
    void alDisparar();
    void alCambiarTurno(int turno);
    void alRecibirMensaje(const QString &msg);
    void alEliminarRival(int jugadorPerdedor);
    void alTerminarProyectil();

private:
    void construirInterfaz();
    void actualizarEtiquetaTurno();

    Ui::Widget     *ui;
    GameScene      *m_escena;
    QGraphicsView  *m_vista;

    QGroupBox      *m_panelControl;
    QLabel         *m_etiquetaTurno;
    QLabel         *m_etiquetaEstado;
    QDoubleSpinBox *m_spinAngulo;
    QDoubleSpinBox *m_spinRapidez;
    QDoubleSpinBox *m_spinMasa;
    QPushButton    *m_botonDisparar;
};

#endif // WIDGET_H
