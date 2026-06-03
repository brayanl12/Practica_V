#include "widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent),
    m_angulo(0.0), m_velocidad(300.0), m_masa(5.0)
{
    configurarUI();

    m_escena = new GameScene(this);
    m_vista->setScene(m_escena);
    m_vista->setRenderHint(QPainter::Antialiasing, true);
    m_vista->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_vista->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    m_vista->fitInView(m_escena->sceneRect(), Qt::KeepAspectRatio);

    connect(m_escena, &GameScene::turnoCambiado,
            this,    &Widget::alCambiarTurno);
    connect(m_escena, &GameScene::juegoTerminadoSignal,
            this,    &Widget::alTerminarJuego);
    connect(m_escena, &GameScene::infoProyectilActualizada,
            this,    &Widget::actualizarInfoProyectil);
    connect(m_botonLanzar, &QPushButton::clicked, this, &Widget::alLanzar);
    connect(m_botonReiniciar,  &QPushButton::clicked, this, &Widget::alReiniciar);
    connect(m_sliderAngulo, &QSlider::valueChanged, this, &Widget::alCambiarAngulo);
    connect(m_spinVelocidad, QOverload<double>::of(&QDoubleSpinBox::valueChanged),
            this, &Widget::alCambiarVelocidad);

    alCambiarTurno(1);
}

Widget::~Widget() {}

void Widget::configurarUI()
{
    setWindowTitle("Minijuego de Estrategia por Turnos - Física Computacional");
    setMinimumSize(1000, 720);

    QVBoxLayout *layoutPrincipal = new QVBoxLayout(this);
    layoutPrincipal->setContentsMargins(8, 8, 8, 8);
    layoutPrincipal->setSpacing(6);

    m_labelTurno = new QLabel("Turno del Jugador 1");
    m_labelTurno->setAlignment(Qt::AlignCenter);
    QFont tf; tf.setBold(true); tf.setPointSize(14);
    m_labelTurno->setFont(tf);
    m_labelTurno->setStyleSheet(
        "QLabel { background: #1e2a3a; color: #e05050; "
        "border-radius: 6px; padding: 6px; }");
    layoutPrincipal->addWidget(m_labelTurno);

    m_vista = new QGraphicsView();
    m_vista->setMinimumSize(900, 600);
    m_vista->setStyleSheet("border: 2px solid #555; border-radius: 4px;");
    layoutPrincipal->addWidget(m_vista, 1);

    QHBoxLayout *filaControles = new QHBoxLayout();

    QGroupBox *grupoAngulo = new QGroupBox("Ángulo de lanzamiento");
    QVBoxLayout *layAngulo = new QVBoxLayout(grupoAngulo);
    m_sliderAngulo = new QSlider(Qt::Horizontal);
    m_sliderAngulo->setRange(-89, 89);
    m_sliderAngulo->setValue(0);
    m_labelAngulo  = new QLabel("0°");
    m_labelAngulo->setAlignment(Qt::AlignCenter);
    layAngulo->addWidget(m_sliderAngulo);
    layAngulo->addWidget(m_labelAngulo);
    filaControles->addWidget(grupoAngulo);

    QGroupBox *grupoFisica = new QGroupBox("Parámetros del proyectil");
    QHBoxLayout *layFisica = new QHBoxLayout(grupoFisica);
    layFisica->addWidget(new QLabel("Velocidad (px/s):"));
    m_spinVelocidad = new QDoubleSpinBox();
    m_spinVelocidad->setRange(50, 800);
    m_spinVelocidad->setValue(300);
    m_spinVelocidad->setSingleStep(10);
    layFisica->addWidget(m_spinVelocidad);
    layFisica->addSpacing(10);
    layFisica->addWidget(new QLabel("Masa (kg):"));
    m_spinMasa = new QDoubleSpinBox();
    m_spinMasa->setRange(1, 50);
    m_spinMasa->setValue(5);
    m_spinMasa->setSingleStep(1);
    layFisica->addWidget(m_spinMasa);
    filaControles->addWidget(grupoFisica);

    QVBoxLayout *layBotones = new QVBoxLayout();
    m_botonLanzar = new QPushButton("🚀 Lanzar");
    m_botonLanzar->setMinimumHeight(40);
    m_botonLanzar->setStyleSheet(
        "QPushButton { background: #2a6030; color: white; font-weight: bold; "
        "border-radius: 6px; font-size: 13px; }"
        "QPushButton:hover { background: #3a8040; }"
        "QPushButton:disabled { background: #555; color: #999; }");
    m_botonReiniciar = new QPushButton("↺ Reiniciar");
    m_botonReiniciar->setMinimumHeight(35);
    m_botonReiniciar->setStyleSheet(
        "QPushButton { background: #5a3020; color: white; font-weight: bold; "
        "border-radius: 6px; }"
        "QPushButton:hover { background: #7a4030; }");
    layBotones->addWidget(m_botonLanzar);
    layBotones->addWidget(m_botonReiniciar);
    filaControles->addLayout(layBotones);

    layoutPrincipal->addLayout(filaControles);

    m_labelInfo = new QLabel(
        "J1 ataca los obstáculos de J2 | J2 ataca los obstáculos de J1\n"
        "Destruye la defensa del rival y golpea al Rival para ganar.");
    m_labelInfo->setAlignment(Qt::AlignCenter);
    m_labelInfo->setStyleSheet("color: #aaa; font-size: 11px;");
    layoutPrincipal->addWidget(m_labelInfo);

    m_labelFisica = new QLabel("vx: --  vy: --  |v|: --");
    m_labelFisica->setAlignment(Qt::AlignCenter);
    m_labelFisica->setStyleSheet("color: #88aaff; font-family: monospace; font-size: 11px;");
    layoutPrincipal->addWidget(m_labelFisica);

    setStyleSheet(
        "QWidget { background: #1a1e26; color: #ddd; }"
        "QGroupBox { border: 1px solid #444; border-radius: 4px; "
        "margin-top: 6px; padding-top: 8px; }"
        "QGroupBox::title { subcontrol-origin: margin; left: 8px; color: #aaa; }"
        "QSlider::groove:horizontal { height: 6px; background: #444; border-radius: 3px; }"
        "QSlider::handle:horizontal { background: #4488ff; width: 14px; height: 14px; "
        "margin: -4px 0; border-radius: 7px; }"
        "QDoubleSpinBox { background: #2a2e3a; border: 1px solid #555; "
        "border-radius: 3px; padding: 2px; min-width: 70px; }"
        );
}

void Widget::alLanzar()
{
    m_angulo = m_sliderAngulo->value();
    m_velocidad = m_spinVelocidad->value();
    m_masa  = m_spinMasa->value();

    m_escena->lanzarProyectil(m_angulo, m_velocidad, m_masa);
    m_botonLanzar->setEnabled(false);
}

void Widget::alReiniciar()
{
    m_escena->reiniciarJuego();
    m_labelFisica->setText("vx: --  vy: --  |v|: --");
    actualizarControles();
}

void Widget::alCambiarTurno(int jugador)
{
    if (jugador == 1) {
        m_labelTurno->setText("🎯  Turno del Jugador 1  (Rojo)");
        m_labelTurno->setStyleSheet(
            "QLabel { background: #2a1515; color: #e06060; "
            "border-radius: 6px; padding: 6px; font-size: 14px; font-weight: bold; }");
    } else {
        m_labelTurno->setText("🎯  Turno del Jugador 2  (Azul)");
        m_labelTurno->setStyleSheet(
            "QLabel { background: #151528; color: #6080e0; "
            "border-radius: 6px; padding: 6px; font-size: 14px; font-weight: bold; }");
    }
    actualizarControles();
}

void Widget::alTerminarJuego(int ganador)
{
    m_botonLanzar->setEnabled(false);
    QString msg = QString("¡El Jugador %1 ha ganado la batalla!").arg(ganador);
    m_labelTurno->setText("🏆  " + msg);
    m_labelTurno->setStyleSheet(
        "QLabel { background: #1a2a10; color: #80e060; "
        "border-radius: 6px; padding: 6px; font-size: 14px; font-weight: bold; }");
    QMessageBox::information(this, "Juego terminado", msg);
}

void Widget::actualizarInfoProyectil(double vx, double vy, double velocidad)
{
    m_labelFisica->setText(
        QString("vx: %1  vy: %2  |v|: %3 px/s")
            .arg(vx, 6, 'f', 1)
            .arg(vy, 6, 'f', 1)
            .arg(velocidad, 6, 'f', 1));
}

void Widget::alCambiarAngulo(int valor)
{
    m_labelAngulo->setText(QString("%1°").arg(valor));
}

void Widget::alCambiarVelocidad(double /*valor*/) {}

void Widget::actualizarControles()
{
    bool puedeLanzar = !m_escena->estaSimulando() && !m_escena->juegoTerminado();
    m_botonLanzar->setEnabled(puedeLanzar);
}
