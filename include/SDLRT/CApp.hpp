//
// Created by gbian on 03/07/2024.
//

#pragma once

#include "./timer/Timer.hpp"
#include "FPSCounter.hpp"
#include "headers.hpp"
#include "rayTracing/Camera.hpp"
#include "rayTracing/Scene.hpp"
#include "rayTracing/qbImage.hpp"
#include <SDL3/SDL.h>
class CApp {
public:
    CApp();

    int OnExecute();
    bool OnInit();
    void OnEvent(SDL_Event *event);
    void OnLoop();
    void OnRender();
    void OnExit();

private:
    qbImage m_image;
    qbRT::Scene m_scene;
    // SDL2 stuff.
    bool isRunning;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
};
