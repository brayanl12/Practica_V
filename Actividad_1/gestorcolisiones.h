#ifndef GESTORCOLISIONES_H
#define GESTORCOLISIONES_H

#include "Particula.h"
#include "Obstaculo.h"
#include <vector>

class GestorColisiones {
public:
    static bool manejarColisionPared(Particula& p,
                                     double xMin, double xMax,
                                     double yMin, double yMax);

    static bool manejarColisionObstaculo(Particula& p, const Obstaculo& obs);

    static bool manejarColisionParticulaParticula(Particula& p1, Particula& p2,
                                                  std::vector<Particula>& particulas);
};

#endif // GESTORCOLISIONES_H
