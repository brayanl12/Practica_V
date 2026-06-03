#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "Vector2D.h"
#include "Particula.h"

class Obstaculo {
public:
    Vector2D esquinaSuperiorIzquierda;
    double ancho, alto;
    double coeficienteRestitucion;  // para colisiones inelásticas

    Obstaculo(double x, double y, double ancho, double alto, double coeficienteRestitucion);
    bool verificarColisionConParticula(const Particula& p, Vector2D& normal) const;
};

#endif
