//
// Created by gbian on 03/07/2024.
//

#include "SDLRT/CApp.hpp"

CApp::CApp() noexcept : isRunning(true), pWindow(nullptr), pRenderer(nullptr) {}

bool CApp::OnInit() {
    vnd::Timer inittimer("init SDL");
    if(SDL_Init(SDL_INIT_VIDEO) < 0) [[unlikely]] {
        LERROR("SDL_Init Error: {}", SDL_GetError());
        return false;
    }
    LINFO("{}", inittimer);
    vnd::Timer initwtimer("init SDL_window");
    pWindow = SDL_CreateWindow(wtile.data(), wwidth, wheight, SDL_WINDOW_RESIZABLE);
    LINFO("{}", initwtimer);

    if(pWindow != nullptr) {
        LINFO("creazione SDL window di dimensioni(w: {}, h: {})", wwidth, wheight);
        // Initialise the renderer.
        vnd::Timer initrtimer("init SDL_Renderer");
        pRenderer = SDL_CreateRenderer(pWindow, nullptr);
        LINFO("{}", initrtimer);
        if(pRenderer == nullptr) [[unlikely]] {
            LERROR("SDL_CreateRenderer Error: {}", SDL_GetError());
            SDL_DestroyWindow(pWindow);
            SDL_Quit();
            return false;
        }

        m_image.Initialize(wwidth, wheight, pRenderer);

        // Render the scene.
        m_scene.Render(m_image);
        m_pixels = m_image.ArrangePixels();
    } else [[unlikely]] {
        LERROR("SDL_CreateWindow Error: {}", SDL_GetError());
        return false;
    }

    return true;
}

int CApp::OnExecute() {
    SDL_Event event{};

    if(OnInit() == false) { return -1; }
    FPSCounter counter(pWindow, wtile.data());
    while(isRunning) {
        while(SDL_PollEvent(&event) != 0) { OnEvent(&event); }
        counter.frameInTitle();
        OnLoop();
        OnRender();
    }

    OnExit();
    return 0;
}

void CApp::OnEvent(const SDL_Event *event) noexcept {
    switch(event->type) {
    case SDL_EVENT_QUIT:
        isRunning = false;
        break;
    case SDL_EVENT_KEY_DOWN:
        switch(event->key.key) {
        case SDLK_ESCAPE:
            isRunning = false;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
}

void CApp::OnLoop() const noexcept {}

void CApp::OnRender() noexcept {
    // Set the background colour to white.
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(pRenderer);

    // Display the image.
    m_image.Display(m_pixels);

    // Show the result.
    SDL_RenderPresent(pRenderer);
}

void CApp::OnExit() {
    vnd::AutoTimer timer{"OnExit"};
    // Tidy up SDL2 stuff.
    if(pRenderer != nullptr) [[likely]] {
        SDL_DestroyRenderer(pRenderer);
        pRenderer = nullptr;
    }
    if(pWindow != nullptr) [[likely]] {
        SDL_DestroyWindow(pWindow);
        pWindow = nullptr;
    }
    SDL_Quit();
}
