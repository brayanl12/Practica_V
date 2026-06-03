#include "widget.h"
#include "ui_widget.h"

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QFont>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QFileDialog>

// ══════════════════════════════════════════
// Colores para cada partícula (por índice)
// ══════════════════════════════════════════
const QColor Widget::COLORES_PARTICULAS[] = {
    QColor(231,  76,  60),   // rojo      – Partícula 1
    QColor( 52, 152, 219),   // azul      – Partícula 2
    QColor( 46, 204, 113),   // verde     – Partícula 3
    QColor(243, 156,  18),   // naranja   – Partícula 4
    QColor(155,  89, 182),   // morado    – Partícula 5
    QColor( 26, 188, 156),   // turquesa  – Partícula 6
};

// ══════════════════════════════════════════
// Constructor
// ══════════════════════════════════════════
Widget::Widget(QWidget *parent)
    : QWidget(parent), ui(new Ui::Widget), sim(nullptr)
{
    ui->setupUi(this);

    // Tamaño fijo de la ventana: caja + panel de botones
    setFixedSize(static_cast<int>(ANCHO_CAJA) + OFFSET_X * 2,
                 static_cast<int>(ALTO_CAJA)  + OFFSET_Y + 10);
    setWindowTitle("Simulacion de Colisiones Multiples — Practica 5");

    // Construir UI de botones
    configurarUI();

    // Crear simulación y poblarla
    sim = new Simulacion(ANCHO_CAJA, ALTO_CAJA, 0.6);
    configurarSimulacion();

    // Timer: cada INTERVALO ms llama a avanzarSimulacion()
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &Widget::avanzarSimulacion);
    // No arranca automáticamente: el usuario presiona "Iniciar"
}

Widget::~Widget() {
    delete ui;
    delete sim;
}

// ══════════════════════════════════════════
// CONFIGURAR PARTÍCULAS Y OBSTÁCULOS
// Modifica este método para cambiar la escena
// ══════════════════════════════════════════
void Widget::configurarSimulacion() {
    // -- Partículas: agregarParticula(x, y, vx, vy, masa, radio) --
    sim->agregarParticula(100, 150,  120,  80, 1.0, 12);  // P1
    sim->agregarParticula(660, 100, -100,  60, 2.0, 15);  // P2
    sim->agregarParticula(380, 480,   50, -90, 3.0, 18);  // P3
    sim->agregarParticula(200, 380,  -70, -50, 1.5, 13);  // P4

    // -- Obstáculos: agregarObstaculo(x, y, lado) --
    sim->agregarObstaculo(140, 190, 60);   // Obs 1
    sim->agregarObstaculo(530, 140, 60);   // Obs 2
    sim->agregarObstaculo(280, 360, 60);   // Obs 3
    sim->agregarObstaculo(520, 390, 60);   // Obs 4
}

// ══════════════════════════════════════════
// CREAR BOTONES Y ETIQUETAS EN LA PARTE
// SUPERIOR DEL WIDGET
// ══════════════════════════════════════════
void Widget::configurarUI() {
    // Fondo del widget
    setStyleSheet("background-color: #1e1e2e;");

    int bx = OFFSET_X;
    int by = 8;
    int bw = 110;
    int bh = 32;
    int gap = 8;

    auto estilo = [](const QString& color) {
        return QString(
                   "QPushButton { background: %1; color: white; border: none; "
                   "border-radius: 6px; font-size: 12px; font-weight: bold; }"
                   "QPushButton:hover { opacity: 0.85; }"
                   "QPushButton:disabled { background: #444; color: #888; }"
                   ).arg(color);
    };

    botonIniciar = new QPushButton("▶  Iniciar", this);
    botonIniciar->setGeometry(bx, by, bw, bh);
    botonIniciar->setStyleSheet(estilo("#27ae60"));
    connect(botonIniciar, &QPushButton::clicked, this, &Widget::iniciarSimulacion);

    bx += bw + gap;
    botonPausar = new QPushButton("⏸  Pausar", this);
    botonPausar->setGeometry(bx, by, bw, bh);
    botonPausar->setStyleSheet(estilo("#e67e22"));
    botonPausar->setEnabled(false);
    connect(botonPausar, &QPushButton::clicked, this, &Widget::pausarSimulacion);

    bx += bw + gap;
    botonReiniciar = new QPushButton("↺  Reiniciar", this);
    botonReiniciar->setGeometry(bx, by, bw, bh);
    botonReiniciar->setStyleSheet(estilo("#c0392b"));
    connect(botonReiniciar, &QPushButton::clicked, this, &Widget::reiniciarSimulacion);

    bx += bw + gap;
    botonGuardar = new QPushButton("💾  Guardar", this);
    botonGuardar->setGeometry(bx, by, bw, bh);
    botonGuardar->setStyleSheet(estilo("#2980b9"));
    connect(botonGuardar, &QPushButton::clicked, this, &Widget::guardarResultados);

    // Etiquetas de información (derecha)
    etiquetaTiempo = new QLabel("t = 0.00 s", this);
    etiquetaTiempo->setGeometry(580, by, 120, bh / 2 - 2);
    etiquetaTiempo->setStyleSheet("color: #a0cfe8; font-size: 12px;");

    etiquetaEventos = new QLabel("Eventos: 0", this);
    etiquetaEventos->setGeometry(580, by + bh / 2, 160, bh / 2 - 2);
    etiquetaEventos->setStyleSheet("color: #f0a070; font-size: 12px;");
}

// ══════════════════════════════════════════
// SLOTS DE BOTONES
// ══════════════════════════════════════════
void Widget::iniciarSimulacion() {
    timer->start(INTERVALO);
    botonIniciar->setEnabled(false);
    botonPausar->setEnabled(true);
}

void Widget::pausarSimulacion() {
    if (timer->isActive()) {
        timer->stop();
        botonPausar->setText("▶  Reanudar");
        botonIniciar->setEnabled(false);
        // Reutilizamos botonPausar para reanudar
        disconnect(botonPausar, &QPushButton::clicked, this, &Widget::pausarSimulacion);
        connect(botonPausar, &QPushButton::clicked, this, &Widget::iniciarSimulacion);
    }
}

void Widget::reiniciarSimulacion() {
    timer->stop();
    delete sim;
    sim = new Simulacion(ANCHO_CAJA, ALTO_CAJA, 0.6);
    configurarSimulacion();

    botonIniciar->setEnabled(true);
    botonPausar->setEnabled(false);
    botonPausar->setText("⏸  Pausar");

    disconnect(botonPausar, nullptr, this, nullptr);
    connect(botonPausar, &QPushButton::clicked, this, &Widget::pausarSimulacion);

    etiquetaTiempo->setText("t = 0.00 s");
    etiquetaEventos->setText("Eventos: 0");
    update();
}

void Widget::guardarResultados() {
    QString ruta = QFileDialog::getSaveFileName(
        this, "Guardar resultados", "simulacion_salida.txt",
        "Archivos de texto (*.txt)"
        );
    if (!ruta.isEmpty())
        sim->guardarEnArchivo(ruta.toStdString());
}

// ══════════════════════════════════════════
// AVANZAR UN PASO DE FÍSICA
// Llamado por el QTimer cada INTERVALO ms
// ══════════════════════════════════════════
void Widget::avanzarSimulacion() {
    sim->avanzarPaso(DT);

    // Actualizar etiquetas de estado
    etiquetaTiempo->setText(
        QString("t = %1 s").arg(sim->tiempoActual(), 0, 'f', 2)
        );
    etiquetaEventos->setText(
        QString("Eventos: %1").arg(sim->eventos.size())
        );

    // Pedir redibujo → Qt llama a paintEvent()
    update();
}

// ══════════════════════════════════════════
// DIBUJAR LA ESCENA COMPLETA
// Qt llama a este método automáticamente
// cada vez que update() es invocado.
// ══════════════════════════════════════════
void Widget::paintEvent(QPaintEvent* /*event*/) {
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);

    // ── Fondo de la caja de simulación ──────────
    p.fillRect(OFFSET_X, OFFSET_Y,
               static_cast<int>(ANCHO_CAJA),
               static_cast<int>(ALTO_CAJA),
               QColor(30, 34, 50));

    // ── Borde de la caja ──────────────────────
    p.setPen(QPen(QColor(120, 140, 200), 2));
    p.setBrush(Qt::NoBrush);
    p.drawRect(OFFSET_X, OFFSET_Y,
               static_cast<int>(ANCHO_CAJA),
               static_cast<int>(ALTO_CAJA));

    if (!sim) return;

    // ── Obstáculos (cuadrados grises) ──────────
    p.setPen(QPen(QColor(100, 110, 130), 1));
    p.setBrush(QBrush(QColor(70, 80, 100)));
    for (const auto& obs : sim->obstaculos) {
        p.drawRect(simX(obs.x), simY(obs.y),
                   static_cast<int>(obs.lado),
                   static_cast<int>(obs.lado));

        // Etiqueta del obstáculo
        p.setPen(QColor(180, 190, 210));
        p.setFont(QFont("Arial", 8));
        p.drawText(simX(obs.x + obs.lado / 2) - 10,
                   simY(obs.y + obs.lado / 2) + 4,
                   QString("Obs %1").arg(obs.id));
        p.setPen(QPen(QColor(100, 110, 130), 1));
    }

    // ── Partículas ─────────────────────────────
    for (int i = 0; i < (int)sim->particulas.size(); ++i) {
        const Particula& part = sim->particulas[i];
        if (!part.activa) continue;

        QColor color = COLORES_PARTICULAS[i % 6];

        // Halo exterior semitransparente
        p.setPen(Qt::NoPen);
        p.setBrush(QBrush(QColor(color.red(), color.green(), color.blue(), 50)));
        int radio = static_cast<int>(part.radio);
        p.drawEllipse(simX(part.x) - radio - 3,
                      simY(part.y) - radio - 3,
                      (radio + 3) * 2, (radio + 3) * 2);

        // Cuerpo de la partícula
        p.setPen(QPen(color.darker(150), 1.5));
        p.setBrush(QBrush(color));
        p.drawEllipse(simX(part.x) - radio,
                      simY(part.y) - radio,
                      radio * 2, radio * 2);

        // Etiqueta con ID y masa
        p.setPen(Qt::white);
        p.setFont(QFont("Arial", 7, QFont::Bold));
        p.drawText(simX(part.x) - 6,
                   simY(part.y) + 4,
                   QString("P%1").arg(part.id));
    }

    // ── Últimos eventos colisión (panel inferior) ──
    int yInfo = OFFSET_Y + static_cast<int>(ALTO_CAJA) + 5;
    p.setPen(QColor(160, 160, 180));
    p.setFont(QFont("Courier", 8));

    int maxMostrar = 2;   // cuántos eventos recientes mostrar
    int total = static_cast<int>(sim->eventos.size());
    int inicio = std::max(0, total - maxMostrar);
    for (int k = inicio; k < total; ++k) {
        p.drawText(OFFSET_X, yInfo + (k - inicio) * 13,
                   QString::fromStdString(sim->eventos[k].descripcion));
    }
}
