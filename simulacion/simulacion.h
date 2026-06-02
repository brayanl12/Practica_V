#ifndef SIMULACION_H
#define SIMULACION_H

#include "Particula.h"
#include "Obstaculo.h"
#include <vector>
#include <string>

class Simulacion {
public:
    double anchoCaja;
    double altoCaja;
    double coefRestitucion;  // para colisión partícula-obstáculo

    std::vector<Particula> particulas;
    std::vector<Obstaculo> obstaculos;

    // Evento registrado al detectar una colisión
    struct EventoColision {
        double      tiempo;
        std::string descripcion;
    };
    std::vector<EventoColision> eventos;

    // Constructor
    Simulacion(double ancho, double alto, double e = 0.6);

    void agregarParticula(double x, double y, double vx, double vy,
                          double masa, double radio);
    void agregarObstaculo(double x, double y, double lado);

    // Avanza UN solo paso de tiempo (lo llama el QTimer del Widget)
    void avanzarPaso(double dt);

    // Guarda trayectorias completas en archivo de texto
    void guardarEnArchivo(const std::string& nombreArchivo) const;

    double tiempoActual() const { return tiempo; }

private:
    double tiempo;
    int    siguienteIdParticula;
    int    siguienteIdObstaculo;

    // Punto de trayectoria guardado en cada paso
    struct PuntoTrayectoria {
        double tiempo;
        int    idParticula;
        double x, y;
    };
    std::vector<PuntoTrayectoria> trayectoria;

    // Los tres tipos de colisión
    void manejarColisionPared      (Particula& p);
    void manejarColisionObstaculo  (Particula& p);
    void manejarColisionParticulas (int i, int j);
};

#endif // SIMULACION_H
