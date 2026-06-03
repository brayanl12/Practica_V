#include "Particula.h"
#include <cmath>

Particula::Particula(int id, double x, double y,
                     double vx, double vy,
                     double masa, double radio)
    : id(id), x(x), y(y), vx(vx), vy(vy),
    masa(masa), radio(radio), activa(true) {}

void Particula::actualizar(double dt) {
    x += vx * dt;
    y += vy * dt;
}

bool Particula::colisionaCon(const Particula& otra) const {
    if (!activa || !otra.activa) return false;
    return distanciaA(otra) <= (radio + otra.radio);
}

double Particula::distanciaA(const Particula& otra) const {
    double dx = x - otra.x;
    double dy = y - otra.y;
    return std::sqrt(dx * dx + dy * dy);
}
