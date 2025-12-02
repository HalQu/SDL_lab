#pragma once

#include "3D_codinate.h"
#include "player.h"


class Camera
{
public:
    Camera(const Point3D& pos, const direction3D& dir, const direction3D& upDir,
           float aspect, float fov, float nearC, float farC)
        : position(pos), direction(dir), up(upDir),
          aspectRatio(aspect), FOV(fov), nearClip(nearC), farClip(farC) {}
    ~Camera() {}


    void setPosition(const Point3D& pos) { position = pos; }
    Point3D getPosition() const { return position; }

    void setDirection(const direction3D& dir) { direction = dir; }
    direction3D get_Direction() const { return direction; }

    void setUp(const direction3D& upDir) { up = upDir; }
    direction3D get_Up() const { return up; }

    void setAspectRatio(float aspect) { aspectRatio = aspect; }
    float getAspectRatio() const { return aspectRatio; }

    void setFOV(float fov) { FOV = fov; }
    float getFOV() const { return FOV; }

    void setNearClip(float nearC) { nearClip = nearC; }
    float getNearClip() const { return nearClip; }

    void setFarClip(float farC) { farClip = farC; }
    float getFarClip() const { return farClip; }

    direction3D get_TxG() const
    {
        return direction3D(
            direction.z * up.y - direction.y * up.z ,
            direction.x * up.z - direction.z * up.x ,
            direction.y * up.x - direction.x * up.y 
        );
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
        }
        if(fov_resize==-1)
        {
            FOV-=0.02f;
            if(FOV<0.1f)
                FOV=0.1f;
        }
    }

    




private:
    Point3D position; // 摄像机位置
    direction3D direction; // 摄像机方向
    direction3D up; // 摄像机向上方向
    float aspectRatio; // 宽高比
    int aspect_resize=false;
    float FOV; // 视野范围(0~1.57 radians )
    int fov_resize = false;
    float nearClip; // 近裁剪平面
    float farClip; // 远裁剪平面
};



