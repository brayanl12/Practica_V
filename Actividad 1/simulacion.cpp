#include "Simulacion.h"
#include <fstream>
#include <cmath>
#include <sstream>
#include <algorithm>

// ══════════════════════════════════════════
// Constructor
// ══════════════════════════════════════════
Simulacion::Simulacion(double ancho, double alto, double e)
    : anchoCaja(ancho), altoCaja(alto), coefRestitucion(e),
    tiempo(0.0), siguienteIdParticula(1), siguienteIdObstaculo(1) {}

void Simulacion::agregarParticula(double x, double y, double vx, double vy,
                                  double masa, double radio) {
    particulas.emplace_back(siguienteIdParticula++, x, y, vx, vy, masa, radio);
}

void Simulacion::agregarObstaculo(double x, double y, double lado) {
    obstaculos.emplace_back(siguienteIdObstaculo++, x, y, lado);
}

// ══════════════════════════════════════════
// COLISIÓN TIPO 1: Partícula – Pared
// Perfectamente elástica: se invierte la componente
// perpendicular a la pared (vx o vy según corresponda).
// ══════════════════════════════════════════
void Simulacion::manejarColisionPared(Particula& p) {
    bool hubo = false;

    if (p.x - p.radio < 0) {
        p.x  = p.radio;
        p.vx = std::abs(p.vx);
        hubo = true;
    } else if (p.x + p.radio > anchoCaja) {
        p.x  = anchoCaja - p.radio;
        p.vx = -std::abs(p.vx);
        hubo = true;
    }

    if (p.y - p.radio < 0) {
        p.y  = p.radio;
        p.vy = std::abs(p.vy);
        hubo = true;
    } else if (p.y + p.radio > altoCaja) {
        p.y  = altoCaja - p.radio;
        p.vy = -std::abs(p.vy);
        hubo = true;
    }

    if (hubo) {
        std::ostringstream ss;
        ss << "t=" << tiempo
           << " | PARED | Particula_" << p.id
           << " | pos=(" << p.x << ", " << p.y << ")"
           << " | vel_post=(" << p.vx << ", " << p.vy << ")";
        eventos.push_back({tiempo, ss.str()});
    }
}

// ══════════════════════════════════════════
// COLISIÓN TIPO 2: Partícula – Obstáculo
// Inelástica con coeficiente de restitución e:
//   v'⊥ = -e · v⊥   (perpendicular al plano de impacto)
//   v'∥ = v∥         (paralela queda intacta)
// ══════════════════════════════════════════
void Simulacion::manejarColisionObstaculo(Particula& p) {
    for (const auto& obs : obstaculos) {
        if (!obs.colisionaConParticula(p.x, p.y, p.radio)) continue;

        double nx = 0, ny = 0;
        obs.obtenerLadoColision(p.x, p.y, nx, ny);

        // Proyectar velocidad en componente perpendicular
        double velPerp = p.vx * nx + p.vy * ny;
        double vPerpX  = velPerp * nx;
        double vPerpY  = velPerp * ny;

        // Componente paralela = velocidad total - perpendicular
        double vParX = p.vx - vPerpX;
        double vParY = p.vy - vPerpY;

        // Aplicar restitución a la perpendicular
        p.vx = vParX + (-coefRestitucion) * vPerpX;
        p.vy = vParY + (-coefRestitucion) * vPerpY;

        // Sacar la partícula del obstáculo para evitar re-colisión
        p.x += nx * p.radio * 0.1;
        p.y += ny * p.radio * 0.1;

        std::ostringstream ss;
        ss << "t=" << tiempo
           << " | OBSTACULO_" << obs.id
           << " | Particula_" << p.id
           << " | normal=(" << nx << ", " << ny << ")"
           << " | vel_post=(" << p.vx << ", " << p.vy << ")";
        eventos.push_back({tiempo, ss.str()});
        break;
    }
}

// ══════════════════════════════════════════
// COLISIÓN TIPO 3: Partícula – Partícula
// Completamente inelástica (fusión):
//   m1·v1 + m2·v2 = (m1+m2)·v'
//   Posición: centro de masa
//   Radio: √(r1² + r2²)  — conserva área total
// ══════════════════════════════════════════
void Simulacion::manejarColisionParticulas(int i, int j) {
    Particula& a = particulas[i];
    Particula& b = particulas[j];

    if (!a.activa || !b.activa) return;
    if (!a.colisionaCon(b))     return;

    double M = a.masa + b.masa;

    double nuevaVx = (a.masa * a.vx + b.masa * b.vx) / M;
    double nuevaVy = (a.masa * a.vy + b.masa * b.vy) / M;
    double nuevaX  = (a.masa * a.x  + b.masa * b.x)  / M;
    double nuevaY  = (a.masa * a.y  + b.masa * b.y)  / M;
    double nuevoR  = std::sqrt(a.radio * a.radio + b.radio * b.radio);

    // 'a' absorbe a 'b'
    a.x     = nuevaX;
    a.y     = nuevaY;
    a.vx    = nuevaVx;
    a.vy    = nuevaVy;
    a.masa  = M;
    a.radio = nuevoR;
    b.activa = false;

    std::ostringstream ss;
    ss << "t=" << tiempo
       << " | FUSION | Particula_" << a.id << " + Particula_" << b.id
       << " -> masa=" << M
       << " | pos_cm=(" << nuevaX << ", " << nuevaY << ")"
       << " | vel=(" << nuevaVx << ", " << nuevaVy << ")"
       << " | radio=" << nuevoR;
    eventos.push_back({tiempo, ss.str()});
}

// ══════════════════════════════════════════
// AVANZAR UN SOLO PASO DE TIEMPO
// Este método es llamado repetidamente por
// el QTimer del Widget en cada tick.
// ══════════════════════════════════════════
void Simulacion::avanzarPaso(double dt) {
    tiempo += dt;

    // Guardar posición actual de todas las partículas activas
    for (const auto& p : particulas)
        if (p.activa)
            trayectoria.push_back({tiempo, p.id, p.x, p.y});

    // 1. Mover partículas
    for (auto& p : particulas)
        if (p.activa) p.actualizar(dt);

    // 2. Colisiones con paredes (elástica)
    for (auto& p : particulas)
        if (p.activa) manejarColisionPared(p);

    // 3. Colisiones con obstáculos (inelástica)
    for (auto& p : particulas)
        if (p.activa) manejarColisionObstaculo(p);

    // 4. Colisiones entre partículas (completamente inelástica)
    for (int i = 0; i < (int)particulas.size(); ++i)
        for (int j = i + 1; j < (int)particulas.size(); ++j)
            manejarColisionParticulas(i, j);
}

// ══════════════════════════════════════════
// GUARDAR RESULTADOS EN ARCHIVO DE TEXTO
// ══════════════════════════════════════════
void Simulacion::guardarEnArchivo(const std::string& nombreArchivo) const {
    std::ofstream archivo(nombreArchivo);
    if (!archivo.is_open()) return;

    archivo << "# OBSTACULOS\n# id  x  y  lado\n";
    for (const auto& obs : obstaculos)
        archivo << obs.id << " " << obs.x << " " << obs.y << " " << obs.lado << "\n";

    archivo << "\n# TRAYECTORIAS\n# tiempo  id_particula  x  y\n";
    for (const auto& pt : trayectoria)
        archivo << pt.tiempo << " " << pt.idParticula << " " << pt.x << " " << pt.y << "\n";

    archivo << "\n# COLISIONES\n";
    for (const auto& ev : eventos)
        archivo << ev.descripcion << "\n";

    archivo.close();
}
