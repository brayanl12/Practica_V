#include "GestorColisiones.h"
#include <cmath>
#include <algorithm>

void GestorColisiones::manejarColisionPared(Particula& p, double xMin, double xMax, double yMin, double yMax) {
    // Pared izquierda
    if (p.posicion.x - p.radio <= xMin) {
        p.posicion.x = xMin + p.radio;
        p.velocidad.x = -p.velocidad.x;
    }
    // Pared derecha
    if (p.posicion.x + p.radio >= xMax) {
        p.posicion.x = xMax - p.radio;
        p.velocidad.x = -p.velocidad.x;
    }
    // Pared superior
    if (p.posicion.y - p.radio <= yMin) {
        p.posicion.y = yMin + p.radio;
        p.velocidad.y = -p.velocidad.y;
    }
    // Pared inferior
    if (p.posicion.y + p.radio >= yMax) {
        p.posicion.y = yMax - p.radio;
        p.velocidad.y = -p.velocidad.y;
    }
}

void GestorColisiones::manejarColisionObstaculo(Particula& p, const Obstaculo& obs) {
    Vector2D normal;
    if (obs.verificarColisionConParticula(p, normal)) {
        double vn = p.velocidad.productoPunto(normal);
        if (vn < 0) {
            // Velocidad paralela al obstáculo
            double vt = p.velocidad.productoPunto(Vector2D(-normal.y, normal.x));
            // Aplicar coeficiente de restitución solo a la componente perpendicular
            p.velocidad = normal * (-obs.coeficienteRestitucion * vn) + Vector2D(-normal.y, normal.x) * vt;
        }
    }
}

bool GestorColisiones::manejarColisionParticulaParticula(Particula& p1, Particula& p2, std::vector<Particula>& particulas) {
    Vector2D delta = p1.posicion - p2.posicion;
    double distancia = delta.magnitud();
    double distanciaMinima = p1.radio + p2.radio;

    if (distancia < distanciaMinima) {
        // Colisión completamente inelástica: se unen
        double nuevaMasa = p1.masa + p2.masa;
        Vector2D nuevaVelocidad = (p1.velocidad * p1.masa + p2.velocidad * p2.masa) * (1.0 / nuevaMasa);
        Vector2D nuevaPosicion = (p1.posicion * p1.masa + p2.posicion * p2.masa) * (1.0 / nuevaMasa);
        double nuevoRadio = std::sqrt(p1.radio * p1.radio + p2.radio * p2.radio); // aproximación por área

        // Desactivar las dos partículas originales
        p1.activa = false;
        p2.activa = false;

        // Crear nueva partícula
        particulas.push_back(Particula(nuevaPosicion.x, nuevaPosicion.y,
                                       nuevaVelocidad.x, nuevaVelocidad.y,
                                       nuevaMasa, nuevoRadio));
        return true;  // Hubo colisión
    }
    return false;
}
