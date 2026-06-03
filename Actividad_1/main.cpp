#include "Simulacion.h"
#include "Obstaculo.h"
#include <iostream>

int main() {
    // Crear la simulación: ancho=800, alto=600, delta tiempo=0.01 segundos
    Simulacion simulacion(800, 600, 0.01);

    // Agregar al menos 4 partículas con diferentes masas, velocidades y tamaños
    simulacion.agregarParticula(Particula(100, 100,  50,  30, 1.0, 10));
    simulacion.agregarParticula(Particula(200, 150, -20,  40, 1.5, 12));
    simulacion.agregarParticula(Particula(300, 200,  60, -10, 1.2, 11));
    simulacion.agregarParticula(Particula(400, 300, -30, -20, 1.8, 13));

    // Agregar 4 obstáculos cuadrados estáticos
    // (x, y, ancho, alto, coeficiente de restitución)
    simulacion.agregarObstaculo(Obstaculo(200, 200, 50, 50, 0.6));
    simulacion.agregarObstaculo(Obstaculo(500, 400, 60, 60, 0.5));
    simulacion.agregarObstaculo(Obstaculo(100, 500, 40, 40, 0.7));
    simulacion.agregarObstaculo(Obstaculo(600, 100, 55, 55, 0.4));

    // Ejecutar simulación por 10 segundos
    simulacion.ejecutar(10.0);

    return 0;
}
