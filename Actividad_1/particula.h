#ifndef PARTICULA_H
#define PARTICULA_H

#include "Vector2D.h"

class Particula {
public:
    Vector2D posicion;
    Vector2D velocidad;
    double masa;
    double radio;
    bool activa;  // true si no ha sido absorbida

    Particula(double x, double y, double vx, double vy, double masa, double radio);
    void actualizar(double dt);
};

#endif
