#ifndef SIMULACION_H
#define SIMULACION_H

#include "Particula.h"
#include "Obstaculo.h"
#include <vector>
#include <fstream>
#include <string>

class Simulacion {
private:
    std::vector<Particula> particulas;
    std::vector<Obstaculo> obstaculos;
    double anchoMundo, altoMundo;
    double deltaTiempo;
    double tiempoActual;
    std::ofstream archivoSalida;

    void registrarPosiciones();
    void registrarColision(const std::string& tipo);

public:
    Simulacion(double ancho, double alto, double dt);
    ~Simulacion();
    void agregarParticula(const Particula& p);
    void agregarObstaculo(const Obstaculo& obs);
    void ejecutar(double duracion);
};

#endif
