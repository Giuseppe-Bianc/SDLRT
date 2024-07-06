//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#pragma once

#include "./timer/Timer.hpp"
#include "FPSCounter.hpp"
#include "headersSDL.hpp"
#include "rayTracing/Camera.hpp"
#include "rayTracing/Scene.hpp"
#include "rayTracing/qbImage.hpp"
class CApp {
public:
    CApp() noexcept;

    int OnExecute();
    bool OnInit();
    void OnEvent(const SDL_Event *event) noexcept;
    void OnLoop() noexcept;
    void OnRender() noexcept;
    void OnExit();

private:
    qbImage m_image;
    qbRT::Scene m_scene;
    std::vector<SDL_Color> m_pixels{};
    // SDL2 stuff.
    bool isRunning;
    SDL_Window *pWindow;
    SDL_Renderer *pRenderer;
};

// NOLINTEND(*-include-cleaner)