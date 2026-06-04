#include "Simulacion.h"
#include "Obstaculo.h"
#include <iostream>

int main() {
    // Mundo 800x600, dt=0.01 s
    Simulacion simulacion(800, 600, 0.01);

    // 6 particulas: (x, y, vx, vy, masa, radio)
    // Velocidades moderadas para reducir colisiones con paredes
    simulacion.agregarParticula(Particula( 80, 100,  35,  20, 1.0, 10));
    simulacion.agregarParticula(Particula(720, 100, -65,  20, 1.5, 12));
    simulacion.agregarParticula(Particula( 80, 500,  80, -25, 1.2, 11));
    simulacion.agregarParticula(Particula(720, 500, -35, -25, 1.8, 13));
    simulacion.agregarParticula(Particula(400,  80,  40,  30, 1.1, 10));
    simulacion.agregarParticula(Particula(0, 520, -15, -30, 1.3, 11));

    // 4 obstaculos cuadrados estaticos: (x, y, ancho, alto, coef_restitucion)
    simulacion.agregarObstaculo(Obstaculo(200, 200, 50, 50, 0.6));
    simulacion.agregarObstaculo(Obstaculo(500, 400, 60, 60, 0.5));
    simulacion.agregarObstaculo(Obstaculo(100, 450, 40, 40, 0.7));
    simulacion.agregarObstaculo(Obstaculo(600, 100, 55, 55, 0.4));

    // Simular 10 segundos
    simulacion.ejecutar(10.0);

    return 0;
}
