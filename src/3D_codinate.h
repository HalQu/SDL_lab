#pragma once

#include "config.h"

class Camera; // 前向声明 Camera 类

class Point2D {
public:
    float x, y;

    Point2D() : x(0), y(0) {}
    Point2D(float x_, float y_) : x(x_), y(y_) {}
};

class direction3D {
public:
    float x, y, z;

    direction3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}
};

class Point3D {
public:
    Point3D() : x(0), y(0), z(0) {}
    Point3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    float getX() const { return x; }
    float getY() const { return y; }
    float getZ() const { return z; }

    void setX(float x_) {x=x_;}
    void setY(float y_) {y=y_;}
    void setZ(float z_) {z=z_;}

    Point2D To2D(const Camera& camera); // 声明 To2D 函数

private:
    float x, y, z;
};


