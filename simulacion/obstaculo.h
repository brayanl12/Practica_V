#ifndef OBSTACULO_H
#define OBSTACULO_H

class Obstaculo {
public:
    int    id;
    double x, y;  // Esquina superior izquierda
    double lado;

    Obstaculo(int id, double x, double y, double lado);

    double izquierda() const { return x; }
    double derecha()   const { return x + lado; }
    double arriba()    const { return y; }
    double abajo()     const { return y + lado; }

    // True si la partícula circular solapa con este obstáculo
    bool colisionaConParticula(double px, double py, double radio) const;

    // Rellena (nx,ny) con la normal exterior del lado más cercano
    // Retorna: 0=izq, 1=der, 2=arr, 3=abaj
    int obtenerLadoColision(double px, double py,
                            double& nx, double& ny) const;
};

#endif // OBSTACULO_H
