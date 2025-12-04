#pragma once

#include <iostream>
#include "3D_codinate.h"
#include "player.h"

class Camera
{
public:
    Camera(const Point3D& pos, const direction3D& dir, const direction3D& upDir,
           float aspect, float fov, float nearC, float farC)
        : position(pos), aspectRatio(aspect), FOV(fov), nearClip(nearC), farClip(farC)
    {
        // 使用 direction、up 构造右向量，再正交化保证三个基向量为正交单位向量
        direction = normalize(dir);
        up = normalize(upDir);
        direction3D r = normalize(cross(direction, up));  // right = forward x up
        up = normalize(cross(r, direction));               // re-orthonormalize up = right x forward
    }
    ~Camera() {}

    void setPosition(const Point3D& pos) { position = pos; }
    Point3D getPosition() const { return position; }

    // 设置方向/上向量时保证正交单位化
    void setDirection(const direction3D& dir) {
        direction = normalize(dir);
        direction3D r = normalize(cross(direction, up));
        up = normalize(cross(r, direction));
    }
    direction3D get_Direction() const { return direction; }

    void setUp(const direction3D& upDir) {
        up = normalize(upDir);
        direction3D r = normalize(cross(direction, up));
        up = normalize(cross(r, direction));
    }
    direction3D get_Up() const { return up; }

    float getAspectRatio() const { return aspectRatio; }
    void setAspectRatio(float aspect) { aspectRatio = aspect; }

    float getFOV() const { return FOV; }
    void setFOV(float fov) { FOV = fov; }

    float getNearClip() const { return nearClip; }
    void setNearClip(float nearC) { nearClip = nearC; }

    float getFarClip() const { return farClip; }
    void setFarClip(float farC) { farClip = farC; }

    direction3D get_TxG() const
    {
        // right = forward x up (保证返回单位向量)
        return normalize(cross(direction, up));
    }


    void input(const SDL_Event& event)
    {
        if(event.type == SDL_KEYDOWN)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_q:
                    fov_resize=-1;
                    break;
                case SDLK_e:
                    fov_resize=1;
                    break;
                case SDLK_UP:
                    aspect_resize=1;
                    break;
                case SDLK_DOWN:
                    aspect_resize=-1;
                    break;
                default:
                    break;
            }
        }
        if(event.type == SDL_KEYUP)
        {
            switch(event.key.keysym.sym)
            {
                case SDLK_q:
                    fov_resize=0;
                    break;
                case SDLK_e:
                    fov_resize=0;
                    break;
                case SDLK_UP:
                    aspect_resize=0;
                    break;
                case SDLK_DOWN:
                    aspect_resize=0;
                    break;
                default:
                    break;
            }
        }
        
    }


    void update_move(const Player& master)
    {
        setPosition(master.getPosition());
        setDirection(master.getDirection());
    }
    void update_fov()
    {
        if(fov_resize==1)
        {
            FOV+=0.02f;
            if(FOV>1.57f)
                FOV=1.57f;
            std::cout<<FOV<<std::endl;
        }
        if(fov_resize==-1)
        {
            FOV-=0.02f;
            if(FOV<0.1f)
                FOV=0.1f;
            std::cout<<FOV<<std::endl;
        }
    }

    void update_pitch()
    {
        if(pitch_resize==1)
        {
              
        }
    }

    

private:
    // 小工具：叉积与归一化
    static direction3D cross(const direction3D& a, const direction3D& b) 
    {
        return direction3D(
            a.y * b.z - a.z * b.y,
            a.z * b.x - a.x * b.z,
            a.x * b.y - a.y * b.x
        );
    }
    static direction3D normalize(const direction3D& v) 
    { 
        float length = std::sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
        if (length <= 1e-8f) return direction3D(0, 0, 1);
        return direction3D(v.x / length, v.y / length, v.z / length);
    }

private:
    Point3D position; // 摄像机位置
    direction3D direction; // 摄像机方向（单位化）
    direction3D up; // 摄像机向上方向（单位化、正交）
    int pitch_resize=false;
    float aspectRatio; // 宽高比
    int aspect_resize=false;
    float FOV; // 视野（弧度）
    int fov_resize = false;
    float nearClip; // 近裁剪平面
    float farClip; // 远裁剪平面
};



