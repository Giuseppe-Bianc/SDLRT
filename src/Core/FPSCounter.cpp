//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "SDLRT/FPSCounter.hpp"
#include "SDLRT/timer/TimeStep.hpp"

FPSCounter::FPSCounter(SDL_Window *window, std::string_view title) noexcept
  : last_time(clock::now()), frames(0), fps(0.0L), ms_per_frame(0.0L), m_window(window), m_title(title) {}

void FPSCounter::frame() {
    updateFPS();
    LINFO("FPS: {:.3Lf} | ms per frame: {:.3Lf}", fps, ms_per_frame);
}

void FPSCounter::frameInTitle() {
    updateFPS();
    SDL_SetWindowTitle(m_window, std::format("{} - {:.3LF} fps/{:.3LF} ms", m_title, fps, ms_per_frame).c_str());
}

void FPSCounter::updateFPS() noexcept {
    frames++;
    const auto current_time = clock::now();
    const auto time_step = Timestep(current_time - last_time);
    if(const auto time_steps = time_step.GetSeconds(); time_steps >= 1.0L) {
        last_time = current_time;
        fps = C_LD(frames) / time_steps;
        ms_per_frame = time_step.GetMilliseconds() / C_LD(frames);
        frames = 0;
    }
}

long double FPSCounter::getFPS() const noexcept { return fps; }

long double FPSCounter::getMsPerFrame() const noexcept { return ms_per_frame; }
// NOLINTEND(*-include-cleaner)
