#include "GestorColisiones.h"
#include <cmath>
#include <algorithm>

// Tipo 1: Elastica con paredes — retorna true si hubo rebote
bool GestorColisiones::manejarColisionPared(Particula& p,
                                            double xMin, double xMax,
                                            double yMin, double yMax) {
    bool colision = false;
    if (p.posicion.x - p.radio <= xMin) {
        p.posicion.x = xMin + p.radio;
        p.velocidad.x = -p.velocidad.x;
        colision = true;
    }
    if (p.posicion.x + p.radio >= xMax) {
        p.posicion.x = xMax - p.radio;
        p.velocidad.x = -p.velocidad.x;
        colision = true;
    }
    if (p.posicion.y - p.radio <= yMin) {
        p.posicion.y = yMin + p.radio;
        p.velocidad.y = -p.velocidad.y;
        colision = true;
    }
    if (p.posicion.y + p.radio >= yMax) {
        p.posicion.y = yMax - p.radio;
        p.velocidad.y = -p.velocidad.y;
        colision = true;
    }
    return colision;
}

// Tipo 2: Inelastica parcial con obstaculo — retorna true si hubo colision
bool GestorColisiones::manejarColisionObstaculo(Particula& p, const Obstaculo& obs) {
    Vector2D normal;
    if (obs.verificarColisionConParticula(p, normal)) {
        double vn = p.velocidad.productoPunto(normal);
        if (vn < 0) {
            double vt = p.velocidad.productoPunto(Vector2D(-normal.y, normal.x));
            p.velocidad = normal * (-obs.coeficienteRestitucion * vn)
                        + Vector2D(-normal.y, normal.x) * vt;
        }
        return true;
    }
    return false;
}

// Tipo 3: Completamente inelastica (fusion) — retorna true si hubo colision
bool GestorColisiones::manejarColisionParticulaParticula(Particula& p1, Particula& p2,
                                                         std::vector<Particula>& particulas) {
    Vector2D delta = p1.posicion - p2.posicion;
    double distancia    = delta.magnitud();
    double distanciaMin = p1.radio + p2.radio;

    if (distancia < distanciaMin && distancia > 1e-9) {
        double nuevaMasa = p1.masa + p2.masa;
        Vector2D nuevaVel = (p1.velocidad * p1.masa + p2.velocidad * p2.masa)
                            * (1.0 / nuevaMasa);
        Vector2D nuevaPos = (p1.posicion  * p1.masa + p2.posicion  * p2.masa)
                            * (1.0 / nuevaMasa);
        double nuevoRadio = std::sqrt(p1.radio * p1.radio + p2.radio * p2.radio);

        p1.activa = false;
        p2.activa = false;

        particulas.push_back(Particula(nuevaPos.x, nuevaPos.y,
                                       nuevaVel.x, nuevaVel.y,
                                       nuevaMasa, nuevoRadio));
        return true;
    }
    return false;
}
