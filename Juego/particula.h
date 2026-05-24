#ifndef PARTICULA_H
#define PARTICULA_H

#include "vector2d.h"
#include <QGraphicsEllipseItem>
#include <QBrush>
#include <QPen>

class Particula : public QGraphicsEllipseItem {
private:
    Vector2D posicion;
    Vector2D velocidad;
    double masa;
    double radio;

public:
    Particula(double x, double y, double vx, double vy, double m, double r, QGraphicsItem *parent = nullptr);

    void actualizarPosicion(double dt);

    Vector2D getPosicion() const;
    Vector2D getVelocidad() const;
    double getMasa() const;
    double getRadio() const;

    void setVelocidad(double vx, double vy);
};

#endif // PARTICULA_H
