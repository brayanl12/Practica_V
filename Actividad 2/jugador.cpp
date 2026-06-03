#include "jugador.h"

Jugador::Jugador(double x, double y, int idJugador, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent),
    m_x(x), m_y(y), m_idJugador(idJugador),
    m_vivo(true), m_radio(20.0)  // Radio un poco más grande
{
    setRect(m_x - m_radio, m_y - m_radio, m_radio * 2, m_radio * 2);

    if (idJugador == 1) {
        setBrush(QBrush(QColor(220, 80, 80)));
        // Agregar un borde más visible
        setPen(QPen(QColor(255, 150, 150), 3));
    } else {
        setBrush(QBrush(QColor(80, 100, 220)));
        setPen(QPen(QColor(150, 150, 255), 3));
    }

    setZValue(5);

    m_etiqueta = new QGraphicsTextItem("Rival", this);
    m_etiqueta->setDefaultTextColor(Qt::white);
    QFont f;
    f.setBold(true);
    f.setPointSize(8);
    m_etiqueta->setFont(f);
    m_etiqueta->setPos(m_x - m_etiqueta->boundingRect().width() / 2.0,
                       m_y + m_radio + 5);

    // Agregar un texto con el número del jugador
    QGraphicsTextItem *numero = new QGraphicsTextItem(QString::number(idJugador), this);
    numero->setDefaultTextColor(Qt::white);
    QFont fn;
    fn.setBold(true);
    fn.setPointSize(14);
    numero->setFont(fn);
    numero->setPos(m_x - 7, m_y - 12);
}

void Jugador::setVivo(bool v)
{
    m_vivo = v;
    if (!m_vivo) {
        setBrush(QBrush(QColor(80, 80, 80)));
        setPen(QPen(QColor(60, 60, 60), 3));
        m_etiqueta->setPlainText("MUERTO");
    }
}
