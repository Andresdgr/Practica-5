#ifndef OBSTACULO_H
#define OBSTACULO_H

#include "vector2d.h"
#include <QGraphicsRectItem>
#include <QBrush>
#include <QPen>

// Heredamos de QGraphicsRectItem
class Obstaculo : public QGraphicsRectItem {
private:
    Vector2D posicion;
    double lado;
    double e;

public:
    Obstaculo(double x, double y, double l, double coef_e, QGraphicsItem *parent = nullptr);

    Vector2D getPosicion() const;
    double getLado() const;
    double getE() const;
};

#endif // OBSTACULO_H
