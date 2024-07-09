//
// Created by gbian on 03/07/2024.
//
// NOLINTBEGIN(*-include-cleaner)
#include "SDLRT/FPSCounter.hpp"
#include "SDLRT/timer/TimeStep.hpp"

DISABLE_WARNINGS_PUSH(26447)

FPSCounter::FPSCounter(SDL_Window *window, std::string_view title) noexcept
  : last_time(clock::now()), frames(0), fps(0.0L), ms_per_frame(0.0L), m_window(window), m_title(title) {}

std::string FPSCounter::transformTime(long double inputTimeMilli) const noexcept {
    using namespace std::chrono;

    const duration<long double, std::milli> durationmils(inputTimeMilli);

    auto durationMs = duration_cast<milliseconds>(durationmils);
    auto durationUs = duration_cast<microseconds>(durationmils - durationMs);
    auto durationNs = duration_cast<nanoseconds>(durationmils - durationMs - durationUs);

    return FORMAT("{}ms,{}us,{}ns", C_LD(durationMs.count()), C_LD(durationUs.count()), C_LD(durationNs.count()));
}

void FPSCounter::frame() {
    updateFPS();
    LINFO("{:.3LF} fps/{}", fps, ms_per_frame);
}

void FPSCounter::frameInTitle() {
    updateFPS();
    SDL_SetWindowTitle(m_window, FORMAT("{} - {:.3LF} fps/{}", m_title, fps, ms_per_frameComposition).c_str());
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
    ms_per_frameComposition = transformTime(ms_per_frame);
}

long double FPSCounter::getFPS() const noexcept { return fps; }

long double FPSCounter::getMsPerFrame() const noexcept { return ms_per_frame; }

DISABLE_WARNINGS_POP()
// NOLINTEND(*-include-cleaner)
