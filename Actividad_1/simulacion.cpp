#include "Simulacion.h"
#include "GestorColisiones.h"
#include <iostream>
#include <algorithm>

Simulacion::Simulacion(double ancho, double alto, double dt)
    : anchoMundo(ancho), altoMundo(alto), deltaTiempo(dt), tiempoActual(0),
      cntPared(0), cntObstaculo(0), cntParticula(0) {
    archivoSalida.open("simulacion_resultados.txt");
    if (archivoSalida.is_open()) {
        archivoSalida << "tiempo,id_particula,posicion_x,posicion_y\n";
    } else {
        std::cerr << "Error: No se pudo abrir el archivo de salida.\n";
    }
}

Simulacion::~Simulacion() {
    if (archivoSalida.is_open()) archivoSalida.close();
}

void Simulacion::agregarParticula(const Particula& p) { particulas.push_back(p); }
void Simulacion::agregarObstaculo(const Obstaculo& obs) { obstaculos.push_back(obs); }

void Simulacion::registrarPosiciones() {
    for (size_t i = 0; i < particulas.size(); ++i) {
        if (particulas[i].activa) {
            archivoSalida << tiempoActual << ","
                          << i                     << ","
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
        for (auto& p : particulas)
            if (p.activa) p.actualizar(deltaTiempo);

        // 2. Tipo 1: colisiones con paredes (elasticas)
        for (auto& p : particulas) {
            if (!p.activa) continue;
            if (GestorColisiones::manejarColisionPared(p, 0, anchoMundo, 0, altoMundo)) {
                registrarColision("pared");
                ++cntPared;
            }
        }

        // 3. Tipo 2: colisiones con obstaculos (inelasticas parciales)
        for (auto& p : particulas) {
            if (!p.activa) continue;
            for (const auto& obs : obstaculos) {
                if (GestorColisiones::manejarColisionObstaculo(p, obs)) {
                    registrarColision("obstaculo");
                    ++cntObstaculo;
                }
            }
        }

        // 4. Tipo 3: colisiones particula-particula (completamente inelasticas)
        bool huboFusion = false;
        size_t n = particulas.size();
        for (size_t i = 0; i < n; ++i) {
            if (!particulas[i].activa) continue;
            for (size_t j = i + 1; j < n; ++j) {
                if (!particulas[j].activa) continue;
                if (GestorColisiones::manejarColisionParticulaParticula(
                        particulas[i], particulas[j], particulas)) {
                    registrarColision("particula-particula");
                    ++cntParticula;
                    huboFusion = true;
                }
            }
        }

        // Limpiar fusionadas DESPUES del paso completo
        if (huboFusion) {
            particulas.erase(
                std::remove_if(particulas.begin(), particulas.end(),
                               [](const Particula& p){ return !p.activa; }),
                particulas.end());
        }

        // 5. Registrar posiciones
        registrarPosiciones();
    }

    // Resumen en consola
    int total = cntPared + cntObstaculo + cntParticula;

    std::cout <<   "       RESUMEN DE COLISIONES          \n";

    std::cout <<   " Tipo 1 - Pared           : " << std::setw(6) << cntPared     << "   \n";
    std::cout <<   " Tipo 2 - Obstaculo       : " << std::setw(6) << cntObstaculo << "   \n";
    std::cout <<   " Tipo 3 - Particula-Part. : " << std::setw(6) << cntParticula << "   \n";

    std::cout <<   "  TOTAL                    : " << std::setw(6) << total        << "  \n";
    std::cout << "Datos guardados en: simulacion_resultados.txt\n";
}
