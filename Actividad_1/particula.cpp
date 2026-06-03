#include "Particula.h"

Particula::Particula(double x, double y, double vx, double vy, double masa, double radio)
    : posicion(x, y), velocidad(vx, vy), masa(masa), radio(radio), activa(true) {}

void Particula::actualizar(double dt) {
    posicion = posicion + velocidad * dt;
}
