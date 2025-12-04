#pragma once
#include <SDL.h>
#include <cmath>
#include "3D_codinate.h"

class Camera; // 前向声明

class Player {
public:
    Player(const Point3D& pos, const direction3D& dir)
        : position(pos), direction(dir) {}
    ~Player() {}

    Point3D getPosition() const { return position; }
    direction3D getDirection() const { return direction; }
    
    void input(const SDL_Event& event)
    {
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_w:
                    is_walkforward=true;
                    break;
                case SDLK_LEFT:
                    is_turnleft=true;
                    break;
                case SDLK_RIGHT:
                    is_turnright=true;
                    break;
                default:
                    break;
            }
        }
        if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_w:
                    is_walkforward=false;
                    break;
                case SDLK_LEFT:
                    is_turnleft=false;
                    break;
                case SDLK_RIGHT:
                    is_turnright=false;
                    break;
                default:
                    break;
            }
        }
    }

    void update()
    {
        // 使用球坐标 theta 统一处理左右转向，并保证同步与归一
        if (is_turnleft) {
            float angle = -5.0f * PI / 180.0f; // 每帧左转 5°
            direction.setSpherical(direction.theta + angle, direction.phi);
        }
        if (is_turnright) {
            float angle = 5.0f * PI / 180.0f; // 每帧右转 5°
            direction.setSpherical(direction.theta + angle, direction.phi);
        }

        if (is_walkforward)
        {
            position.setX(position.getX()+velocity*direction.x);
            position.setY(position.getY()+velocity*direction.y);
            position.setZ(position.getZ()+velocity*direction.z);
        }
    }
private:
    Point3D position; // 玩家在3D空间中的位置
    direction3D direction; // 玩家面朝的方向
    bool is_walkforward = false;
    bool is_turnleft = false;
    bool is_turnright = false;
    float velocity = 5.0f;
};