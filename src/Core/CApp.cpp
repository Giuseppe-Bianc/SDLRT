//
// Created by gbian on 03/07/2024.
//

#include "SDLRT/CApp.hpp"

CApp::CApp() : isRunning(true), pWindow(nullptr), pRenderer(nullptr) {}

bool CApp::OnInit() {
    vnd::Timer inittimer("init SDL");
    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        LERROR("SDL_Init Error: {}", SDL_GetError());
        return false;
    }
    LINFO("{}", inittimer);
    vnd::Timer initwtimer("init SDL_window");
    pWindow = SDL_CreateWindow(wtile.data(), wwidth, wheight, SDL_WINDOW_RESIZABLE);
    LINFO("{}", initwtimer);

    if(pWindow != nullptr) {
        // Initialise the renderer.
        vnd::Timer initrtimer("init SDL_Renderer");
        pRenderer = SDL_CreateRenderer(pWindow, nullptr);
        LINFO("{}", initrtimer);
        if(pRenderer == nullptr) {
            LERROR("SDL_CreateRenderer Error: {}", SDL_GetError());
            SDL_DestroyWindow(pWindow);
            SDL_Quit();
            return false;
        }

        // Initialise the qbImage instance.
        m_image.Initialize(wwidth, wheight, pRenderer);
        qbRT::Camera testCamera;
        testCamera.SetPosition(glm::dvec3{0.0, 0.0, 0.0});
        testCamera.SetLookAt(glm::dvec3{0.0, 2.0, 0.0});
        testCamera.SetUp(glm::dvec3{0.0, 0.0, 1.0});
        testCamera.SetLength(1.0);
        testCamera.SetHorzSize(1.0);
        testCamera.SetAspect(1.0);
        testCamera.UpdateCameraGeometry();

        // Get the screen centre and U,V vectors and display.
        auto screenCentre = testCamera.GetScreenCentre();
        auto screenU = testCamera.GetU();
        auto screenV = testCamera.GetV();

        // And display to the terminal.
        LINFO("\nCamera screen centre: {}\nCamera U vector: {}\nCamera V vector:{}", screenCentre, screenU, screenV);
    } else {
        LERROR("SDL_CreateWindow Error: {}", SDL_GetError());
        return false;
    }

    return true;
}

int CApp::OnExecute() {
    SDL_Event event;

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

void CApp::OnEvent(SDL_Event *event) {
    switch(event->type) {
    case SDL_EVENT_QUIT:
        isRunning = false;
        break;
    case SDL_EVENT_KEY_DOWN:
        isRunning = false;
        break;
    }
}

void CApp::OnLoop() {}

void CApp::OnRender() {
    // Set the background colour to white.
    SDL_SetRenderDrawColor(pRenderer, 255, 255, 255, 255);
    SDL_RenderClear(pRenderer);

    m_scene.Render(m_image);
    // Display the image.
    m_image.Display();

    // Show the result.
    SDL_RenderPresent(pRenderer);
}

void CApp::OnExit() {
    // Tidy up SDL2 stuff.
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    pWindow = NULL;
    SDL_Quit();
}
