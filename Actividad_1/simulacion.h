#ifndef SIMULACION_H
#define SIMULACION_H

#include "Particula.h"
#include "Obstaculo.h"
#include <vector>
#include <fstream>
#include <string>
#include <iomanip>

class Simulacion {
public:
    Simulacion(double ancho, double alto, double dt);
    ~Simulacion();

    void agregarParticula(const Particula& p);
    void agregarObstaculo(const Obstaculo& obs);
    void ejecutar(double duracion);

private:
    double anchoMundo, altoMundo, deltaTiempo, tiempoActual;
    std::vector<Particula> particulas;
    std::vector<Obstaculo> obstaculos;
    std::ofstream archivoSalida;

    int cntPared, cntObstaculo, cntParticula;

    void registrarPosiciones();
    void registrarColision(const std::string& tipo);
};

#endif // SIMULACION_H
