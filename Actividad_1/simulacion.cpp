#include "Simulacion.h"
#include "GestorColisiones.h"
#include <iostream>
#include <algorithm>

Simulacion::Simulacion(double ancho, double alto, double dt)
    : anchoMundo(ancho), altoMundo(alto), deltaTiempo(dt), tiempoActual(0) {
    archivoSalida.open("simulacion_resultados.txt");
    if (archivoSalida.is_open()) {
        archivoSalida << "tiempo,id_particula,posicion_x,posicion_y\n";
    } else {
        std::cerr << "Error: No se pudo abrir el archivo de salida.\n";
    }
}

Simulacion::~Simulacion() {
    if (archivoSalida.is_open()) {
        archivoSalida.close();
    }
}

void Simulacion::agregarParticula(const Particula& p) {
    particulas.push_back(p);
}

void Simulacion::agregarObstaculo(const Obstaculo& obs) {
    obstaculos.push_back(obs);
}

void Simulacion::registrarPosiciones() {
    for (size_t i = 0; i < particulas.size(); ++i) {
        if (particulas[i].activa) {
            archivoSalida << tiempoActual << "," << i << ","
                          << particulas[i].posicion.x << ","
                          << particulas[i].posicion.y << "\n";
        }
    }
}

void Simulacion::registrarColision(const std::string& tipo) {
    archivoSalida << tiempoActual << ",COLISION," << tipo << "\n";
}

void Simulacion::ejecutar(double duracion) {
    int pasos = static_cast<int>(duracion / deltaTiempo);

    for (int paso = 0; paso <= pasos; ++paso) {
        tiempoActual = paso * deltaTiempo;

        // 1. Actualizar posiciones
        for (auto& p : particulas) {
            if (p.activa) p.actualizar(deltaTiempo);
        }

        // 2. Colisiones con paredes (elásticas)
        for (auto& p : particulas) {
            if (p.activa) {
                GestorColisiones::manejarColisionPared(p, 0, anchoMundo, 0, altoMundo);
            }
        }

        // 3. Colisiones con obstáculos (inelásticas)
        for (auto& p : particulas) {
            if (!p.activa) continue;
            for (auto& obs : obstaculos) {
                GestorColisiones::manejarColisionObstaculo(p, obs);
            }
        }

        // 4. Colisiones entre partículas (completamente inelásticas)
        bool huboColision = false;
        for (size_t i = 0; i < particulas.size(); ++i) {
            if (!particulas[i].activa) continue;
            for (size_t j = i + 1; j < particulas.size(); ++j) {
                if (!particulas[j].activa) continue;
                if (GestorColisiones::manejarColisionParticulaParticula(particulas[i], particulas[j], particulas)) {
                    registrarColision("particula-particula");
                    huboColision = true;
                }
            }
        }

        // Si hubo colisiones, limpiamos las partículas inactivas
        if (huboColision) {
            particulas.erase(std::remove_if(particulas.begin(), particulas.end(),
                                            [](const Particula& p) { return !p.activa; }),
                             particulas.end());
        }

        // 5. Registrar posiciones actuales
        registrarPosiciones();
    }

    std::cout << "Simulación finalizada. Datos guardados en 'simulacion_resultados.txt'\n";
}
