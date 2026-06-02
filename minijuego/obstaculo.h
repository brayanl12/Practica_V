#ifndef OBSTACULO_H
#define OBSTACULO_H

#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QString>

// -------------------------------------------------------
// Obstaculo
// Rectángulo estático con resistencia (HP).
// Daño = FACTOR_DANIO * masa_proyectil * |v_impacto|
// Cambia de color conforme pierde resistencia.
// -------------------------------------------------------
class Obstaculo : public QGraphicsRectItem
{
public:
    static constexpr double FACTOR_DANIO = 0.05; // factor constante definido por el estudiante

    Obstaculo(double x, double y, double ancho, double alto,
              double resistencia, int jugadorDuenio,
              const QString &etiqueta = "");

    // Aplica daño; retorna true si fue destruido
    bool aplicarDanio(double masaProyectil, double rapidezImpacto);

    bool   estaDestruido() const { return m_resistencia <= 0; }
    double resistencia()   const { return m_resistencia; }
    int    duenio()        const { return m_duenio; }

    void actualizarEtiqueta();

private:
    double             m_resistencia;
    int                m_duenio;
    QGraphicsTextItem *m_etiqueta;
};

#endif // OBSTACULO_H
