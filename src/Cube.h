#pragma once

#include "3D_codinate.h"
#include <SDL.h>
#include "Camera.h"
#include <vector>





class Cube {
public:
    Cube(const Point3D& centerPoint, float cubeSize)
        : center(centerPoint), size(cubeSize) {
        float halfSize = size / 2.0f;
        // 定义立方体的8个顶点
        vertices[0] = Point3D(center.getX() - halfSize, center.getY() - halfSize, center.getZ() - halfSize);
        vertices[1] = Point3D(center.getX() + halfSize, center.getY() - halfSize, center.getZ() - halfSize);
        vertices[2] = Point3D(center.getX() + halfSize, center.getY() + halfSize, center.getZ() - halfSize);
        vertices[3] = Point3D(center.getX() - halfSize, center.getY() + halfSize, center.getZ() - halfSize);
        vertices[4] = Point3D(center.getX() - halfSize, center.getY() - halfSize, center.getZ() + halfSize);
        vertices[5] = Point3D(center.getX() + halfSize, center.getY() - halfSize, center.getZ() + halfSize);
        vertices[6] = Point3D(center.getX() + halfSize, center.getY() + halfSize, center.getZ() + halfSize);
        vertices[7] = Point3D(center.getX() - halfSize, center.getY() + halfSize, center.getZ() + halfSize);
    }

    ~Cube() {}

    Point3D getCenter() const { return center; }
    float getSize() const { return size; }

    void setCenter(const Point3D& centerPoint) { center = centerPoint; }
    void setSize(float cubeSize) { size = cubeSize; }

    // 绘制立方体
    void draw(SDL_Renderer* renderer, const Camera& camera) {
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        int edges[12][2] = {
            {0, 1}, {1, 2}, {2, 3}, {3, 0},
            {4, 5}, {5, 6}, {6, 7}, {7, 4},
            {0, 4}, {1, 5}, {2, 6}, {3, 7}
        };
        const float INVISIBLE = 1e5f;
        for (int i = 0; i < 12; ++i) {
            Point2D p1 = vertices[edges[i][0]].To2D(camera);
            Point2D p2 = vertices[edges[i][1]].To2D(camera);
            if ((std::fabs(p1.x) > INVISIBLE && std::fabs(p1.y) > INVISIBLE) ||
                (std::fabs(p2.x) > INVISIBLE && std::fabs(p2.y) > INVISIBLE))
                continue;
            SDL_RenderDrawLine(renderer, static_cast<int>(p1.x), static_cast<int>(p1.y),
                               static_cast<int>(p2.x), static_cast<int>(p2.y));
        }
    }

private:
    Point3D center; // 立方体中心点
    float size;     // 立方体边长
    Point3D vertices[8]; // 立方体的8个顶点
};

