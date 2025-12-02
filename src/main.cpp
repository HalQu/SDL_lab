#include <SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include "3D_codinate.h"
#include "Cube.h"
#include "Camera.h"
#include "config.h"
#include "player.h"

int main(int /*argc*/, char* /*argv*/[]) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return 1;
    }

    // 创建窗口
    SDL_Window* window = SDL_CreateWindow("3D Cube and Camera Demo",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          SCREEN_WIDTH, SCREEN_HEIGHT,
                                          SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if (!window) {
        SDL_Log("Could not create window: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    // 创建渲染器
    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        SDL_Log("Could not create renderer: %s", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    // 初始化立方体和摄像机
    Cube cube(Point3D(0.0f, 0.0f, 0.0f), 100.0f); // 立方体中心在原点，边长为 100

    Player master(Point3D(0.0f,0.0f,-300.0f),direction3D(0.0f,0.0f,1.0));

    Camera camera(Point3D(50.0f, 0.0f, -300.0f),  // 摄像机位置在 z = -500
                  direction3D(0.0f, 0.0f, 1.0f), // 摄像机朝向 z 轴正方向
                  direction3D(0.0f, 1.0f, 0.0f), // 摄像机的上方向为 y 轴正方向
                  (float)SCREEN_WIDTH / SCREEN_HEIGHT, 
                  1.4f ,//视野
                  0.1f, //近裁剪面
                  1000.0f//远裁剪面
                );

    bool running = true;
    SDL_Event event;

    // 游戏主循环
    while (running) {
        // 事件处理
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }
            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE: // 按下 ESC 键退出
                        running = false;
                        break;
                    default:
                        break;
                }

            }
            master.input(event);
            camera.input(event);
            
        }

        //数据更新逻辑
        master.update();
        camera.update_move(master);
        camera.update_fov();

        // 渲染逻辑
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // 设置背景颜色为黑色
        SDL_RenderClear(renderer); // 清空屏幕

        // 绘制立方体
        cube.draw(renderer, camera);


        SDL_RenderPresent(renderer); // 更新屏幕
        SDL_Delay(16); // 大约 60 FPS
    }

    // 清理资源
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}