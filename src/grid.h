#pragma once
#include "3D_codinate.h"
#include "Camera.h"
#include <cmath>
#include <SDL.h>
#include <vector>
#include "config.h"
#include <iostream>

class Grid 
{
public:
    // gridSize: 格子边长（世界单位）
    // gridCount: 每边格线数（应为奇数以便中心对齐）
    // planeY: 网格位于世界 y 平面高度
    Grid(float gridSize = 1.0f, int gridCount = 41, float planeY = 0.0f)
        : size(gridSize), count((gridCount > 1) ? gridCount : 41), y(planeY)
    {
        if (count % 2 == 0) ++count; // 保证奇数，便于居中
    }

    ~Grid() {}

    void draw(SDL_Renderer* renderer, const Camera& camera)
    {
        const Point3D camPos = camera.getPosition();

        float cx = camPos.getX();
        float cz = camPos.getZ();
        float cy = camPos.getY();

        int centerXIdx = static_cast<int>(std::round(cx / size));
        int centerZIdx = static_cast<int>(std::round(cz / size));

        int half = count / 2;
        float startX = (centerXIdx - half) * size;
        float endX   = (centerXIdx + half) * size;
        float startZ = (centerZIdx - half) * size;
        float endZ   = (centerZIdx + half) * size;

        SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);

        // helper: 计算点到相机前向轴的深度 cz
        auto camDepth = [&](const Point3D &p)->float {
            auto f = camera.get_Direction();
            return (p.getX() - camPos.getX())*f.x + (p.getY() - camPos.getY())*f.y + (p.getZ() - camPos.getZ())*f.z;
        };

        // helper: 把线段裁剪到 nearClip（如果整段被裁掉返回 false）
        auto clipToNear = [&](const Point3D &a, const Point3D &b, Point3D &outA, Point3D &outB)->bool {
            float nearC = camera.getNearClip();
            float czA = camDepth(a);
            float czB = camDepth(b);
            // 全在相机后面 / 在近裁剪面之内 -> 不可见
            if (czA <= nearC && czB <= nearC) return false;

            outA = a; outB = b;
            if (czA <= nearC && czB > nearC) {
                float t = (nearC - czA) / (czB - czA);
                outA.setX(a.getX() + (b.getX() - a.getX()) * t);
                outA.setY(a.getY() + (b.getY() - a.getY()) * t);
                outA.setZ(a.getZ() + (b.getZ() - a.getZ()) * t);
            } else if (czB <= nearC && czA > nearC) {
                float t = (nearC - czA) / (czB - czA);
                outB.setX(a.getX() + (b.getX() - a.getX()) * t);
                outB.setY(a.getY() + (b.getY() - a.getY()) * t);
                outB.setZ(a.getZ() + (b.getZ() - a.getZ()) * t);
            }
            return true;
        };

        // 绘制平行 X 轴的线（Z 方向变化）
        for (int i = 0; i <= count; ++i) {
            float zPos = startZ + i * size;
            Point3D p1(startX, y, zPos);
            Point3D p2(endX,   y, zPos);

            Point3D c1, c2;
            if (!clipToNear(p1, p2, c1, c2)) continue;

            Point2D s = c1.To2D_forback(camera);
            Point2D e = c2.To2D_forback(camera);

            if (!std::isfinite(s.x) || !std::isfinite(s.y) || !std::isfinite(e.x) || !std::isfinite(e.y)) continue;

            float dist = std::fabs(zPos - cz);
            uint8_t alpha = static_cast<uint8_t>(std::max(40.0f, 255.0f * std::max(0.0f, 1.0f - dist / (half * size))));
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, alpha);
            SDL_RenderDrawLine(renderer, static_cast<int>(s.x), static_cast<int>(s.y),
                                         static_cast<int>(e.x), static_cast<int>(e.y));
        }

        // 绘制平行 Z 轴的线（X 方向变化）
        for (int i = 0; i <= count; ++i) {
            float xPos = startX + i * size;
            Point3D p1(xPos, y, startZ);
            Point3D p2(xPos, y, endZ);

            Point3D c1, c2;
            if (!clipToNear(p1, p2, c1, c2)) continue;

            Point2D s = c1.To2D_forback(camera);
            Point2D e = c2.To2D_forback(camera);

            if (!std::isfinite(s.x) || !std::isfinite(s.y) || !std::isfinite(e.x) || !std::isfinite(e.y)) continue;

            float dist = std::fabs(xPos - cx);
            uint8_t alpha = static_cast<uint8_t>(std::max(40.0f, 255.0f * std::max(0.0f, 1.0f - dist / (half * size))));
            SDL_SetRenderDrawColor(renderer, 200, 200, 200, alpha);
            SDL_RenderDrawLine(renderer, static_cast<int>(s.x), static_cast<int>(s.y),
                                         static_cast<int>(e.x), static_cast<int>(e.y));
        }

        // 中心轴高亮（裁剪后绘制）
        {
            Point3D a(cx, y, startZ), b(cx, y, endZ), ca, cb;
            if (clipToNear(a, b, ca, cb)) {
                Point2D sa = ca.To2D_forback(camera), sb = cb.To2D_forback(camera);
                if (std::isfinite(sa.x) && std::isfinite(sb.x)) {
                    SDL_SetRenderDrawColor(renderer, 220, 80, 80, 255);
                    SDL_RenderDrawLine(renderer, (int)sa.x, (int)sa.y, (int)sb.x, (int)sb.y);
                }
            }
        }
        {
            Point3D a(startX, y, cz), b(endX, y, cz), ca, cb;
            if (clipToNear(a, b, ca, cb)) {
                Point2D sa = ca.To2D_forback(camera), sb = cb.To2D_forback(camera);
                if (std::isfinite(sa.x) && std::isfinite(sb.x)) {
                    SDL_SetRenderDrawColor(renderer, 80, 160, 220, 255);
                    SDL_RenderDrawLine(renderer, (int)sa.x, (int)sa.y, (int)sb.x, (int)sb.y);
                }
            }
        }
    }

private:
    float size;   // 单个格子的大小
    int count;    // 网格线数量
    float y;      // 网格所在的Y平面高度

    
};