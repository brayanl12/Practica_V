#ifndef VECTOR2D_H
#define VECTOR2D_H

#include <cmath>

class Vector2D {
public:
    double x, y;

    Vector2D(double x = 0, double y = 0);
    Vector2D operator+(const Vector2D& otro) const;
    Vector2D operator-(const Vector2D& otro) const;
    Vector2D operator*(double escalar) const;
    double magnitud() const;
    double productoPunto(const Vector2D& otro) const;
};

#endif
