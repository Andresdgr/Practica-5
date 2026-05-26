#include "particula.h"

Particula::Particula(double x, double y, double vx, double vy, double m, double r, QGraphicsItem *parent)
    : QGraphicsEllipseItem(parent), posicion(x, y), velocidad(vx, vy), masa(m), radio(r)
{
    setRect(-radio, -radio, radio * 2, radio * 2); // Define el tamaño de la elipse
    setPos(x, y);                                  // Posición inicial en la escena
    setBrush(Qt::red);                             // Color de relleno
    setPen(QPen(Qt::black));                       // Color del borde
}

void Particula::actualizarPosicion(double dt) {
    // Actualiza la lógica matemática
    posicion.setX(posicion.getX() + velocidad.getX() * dt);
    posicion.setY(posicion.getY() + velocidad.getY() * dt);

    // Actualiza la posición visual en la interfaz de Qt
    setPos(posicion.getX(), posicion.getY());
}

Vector2D Particula::getPosicion() const { return posicion; }
Vector2D Particula::getVelocidad() const { return velocidad; }
double Particula::getMasa() const { return masa; }
double Particula::getRadio() const { return radio; }

void Particula::setVelocidad(double vx, double vy) {
    velocidad.setX(vx);
    velocidad.setY(vy);
}
