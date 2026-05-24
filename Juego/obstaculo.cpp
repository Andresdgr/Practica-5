#include "obstaculo.h"

Obstaculo::Obstaculo(double x, double y, double l, double coef_e, QGraphicsItem *parent)
    : QGraphicsRectItem(parent), posicion(x, y), lado(l), e(coef_e)
{
    // Dimensiones del cuadrado visual (x local 0, y local 0, ancho, alto)
    setRect(0, 0, lado, lado);

    // Coordenadas globales de la escena
    setPos(x, y);

    // Aspecto visual
    setBrush(Qt::darkGray);
    setPen(QPen(Qt::black));
}

Vector2D Obstaculo::getPosicion() const { return posicion; }
double Obstaculo::getLado() const { return lado; }
double Obstaculo::getE() const { return e; }
