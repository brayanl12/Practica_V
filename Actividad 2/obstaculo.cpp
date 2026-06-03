#include "obstaculo.h"
#include <algorithm>

Obstaculo::Obstaculo(double x, double y, double w, double h,
                     int resistencia, int dueno,
                     QGraphicsItem *parent)
    : QGraphicsRectItem(x, y, w, h, parent),
    m_x(x), m_y(y), m_w(w), m_h(h),
    m_resistencia(resistencia),
    m_dueno(dueno),
    m_resistenciaInicial(resistencia)
{
    m_etiqueta = new QGraphicsTextItem(QString::number(m_resistencia), this);
    m_etiqueta->setDefaultTextColor(Qt::white);
    QFont f;
    f.setBold(true);
    f.setPointSize(9);
    m_etiqueta->setFont(f);

    actualizarVisual();
}

void Obstaculo::aplicarDano(double dano)
{
    m_resistencia -= static_cast<int>(dano);
    if (m_resistencia < 0) m_resistencia = 0;
    actualizarVisual();
}

void Obstaculo::actualizarVisual()
{
    double ratio = (m_resistenciaInicial > 0)
    ? static_cast<double>(m_resistencia) / m_resistenciaInicial
    : 0.0;

    QColor colorBase = (m_dueno == 1)
                           ? QColor(180, 60, 60)
                           : QColor(60, 80, 180);

    if (estaDestruido()) {
        setBrush(QBrush(QColor(80, 80, 80)));
        setPen(QPen(QColor(60, 60, 60), 2));
    } else {
        int r = static_cast<int>(colorBase.red()   * ratio + 120 * (1.0 - ratio));
        int g = static_cast<int>(colorBase.green() * ratio + 120 * (1.0 - ratio));
        int b = static_cast<int>(colorBase.blue()  * ratio + 120 * (1.0 - ratio));
        setBrush(QBrush(QColor(r, g, b)));
        setPen(QPen(Qt::black, 2));
    }

    m_etiqueta->setPlainText(estaDestruido() ? "X" : QString::number(m_resistencia));
    double lx = m_x + m_w / 2.0 - m_etiqueta->boundingRect().width()  / 2.0;
    double ly = m_y + m_h / 2.0 - m_etiqueta->boundingRect().height() / 2.0;
    m_etiqueta->setPos(lx, ly);
}
