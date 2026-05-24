#ifndef VECTOR2D_H
#define VECTOR2D_H

class Vector2D {
private:
    double x;
    double y;

public:
    Vector2D(double x = 0.0, double y = 0.0);

    double getX() const;
    double getY() const;

    void setX(double x);
    void setY(double y);
};

#endif // VECTOR2D_H
