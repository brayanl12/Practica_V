#include "Obstaculo.h"
#include <cmath>
#include <algorithm>

Obstaculo::Obstaculo(int id, double x, double y, double lado)
    : id(id), x(x), y(y), lado(lado) {}

bool Obstaculo::colisionaConParticula(double px, double py, double radio) const {
    double cx = std::max(izquierda(), std::min(px, derecha()));
    double cy = std::max(arriba(),    std::min(py, abajo()));
    double dx = px - cx;
    double dy = py - cy;
    return (dx * dx + dy * dy) <= (radio * radio);
}

int Obstaculo::obtenerLadoColision(double px, double py,
                                   double& nx, double& ny) const {
    double dIzq  = std::abs(px - izquierda());
    double dDer  = std::abs(px - derecha());
    double dArr  = std::abs(py - arriba());
    double dAbaj = std::abs(py - abajo());

    double minD = std::min({dIzq, dDer, dArr, dAbaj});

    if (minD == dIzq)  { nx = -1; ny =  0; return 0; }
    if (minD == dDer)  { nx =  1; ny =  0; return 1; }
    if (minD == dArr)  { nx =  0; ny = -1; return 2; }
    /* dAbaj */          nx =  0; ny =  1; return 3;
}
