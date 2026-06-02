#ifndef PARTICULA_H
#define PARTICULA_H

class Particula {
public:
    int    id;
    double x, y;    // Posición
    double vx, vy;  // Velocidad
    double masa;
    double radio;
    bool   activa;  // false si fue absorbida

    Particula(int id, double x, double y,
              double vx, double vy,
              double masa, double radio);

    // Avanza la posición un paso dt
    void actualizar(double dt);

    // True si los círculos se solapan
    bool colisionaCon(const Particula& otra) const;

    double distanciaA(const Particula& otra) const;
};

#endif // PARTICULA_H
