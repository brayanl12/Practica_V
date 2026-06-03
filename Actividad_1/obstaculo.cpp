#include "Obstaculo.h"
#include <cmath>

Obstaculo::Obstaculo(double x, double y, double ancho, double alto, double coeficienteRestitucion)
    : esquinaSuperiorIzquierda(x, y), ancho(ancho), alto(alto), coeficienteRestitucion(coeficienteRestitucion) {}

bool Obstaculo::verificarColisionConParticula(const Particula& p, Vector2D& normal) const {
    double cx = p.posicion.x;
    double cy = p.posicion.y;
    double r = p.radio;

    double izquierda = esquinaSuperiorIzquierda.x;
    double derecha = esquinaSuperiorIzquierda.x + ancho;
    double arriba = esquinaSuperiorIzquierda.y;
    double abajo = esquinaSuperiorIzquierda.y + alto;

    double closestX = std::max(izquierda, std::min(cx, derecha));
    double closestY = std::max(arriba, std::min(cy, abajo));

    double dx = cx - closestX;
    double dy = cy - closestY;

    if (std::sqrt(dx*dx + dy*dy) < r) {
        // Determinar la normal (lado del cuadrado)
        if (std::abs(dx) > std::abs(dy)) {
            normal = Vector2D(dx > 0 ? 1 : -1, 0);
        } else {
            normal = Vector2D(0, dy > 0 ? 1 : -1);
        }
        return true;
    }
    return false;
}
