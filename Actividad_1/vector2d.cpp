#include "Vector2D.h"

Vector2D::Vector2D(double x, double y) : x(x), y(y) {}

Vector2D Vector2D::operator+(const Vector2D& otro) const {
    return Vector2D(x + otro.x, y + otro.y);
}

Vector2D Vector2D::operator-(const Vector2D& otro) const {
    return Vector2D(x - otro.x, y - otro.y);
}

Vector2D Vector2D::operator*(double escalar) const {
    return Vector2D(x * escalar, y * escalar);
}

double Vector2D::magnitud() const {
    return std::sqrt(x * x + y * y);
}

double Vector2D::productoPunto(const Vector2D& otro) const {
    return x * otro.x + y * otro.y;
}
