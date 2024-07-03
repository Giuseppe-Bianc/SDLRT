//
// Created by gbian on 28/06/2024.
//

#pragma once

#include "../cast/casts.hpp"
#include "../headers.hpp"
#include "timeFactors.hpp"

class Timestep {
public:
    explicit Timestep(float time = 0.0F) noexcept : m_Time(C_LD(time)) {}
    explicit Timestep(double time = 0.0) noexcept : m_Time(C_LD(time)) {}
    explicit Timestep(long double time = 0.0L) noexcept : m_Time(time) {}
    explicit Timestep(const std::chrono::duration<long double> &time) noexcept : m_Time(time.count()) {}

    explicit operator float() const noexcept { return C_F(m_Time); }
    explicit operator double() const noexcept { return C_D(m_Time); }
    explicit operator long double() const noexcept { return m_Time; }

    long double GetSeconds() const noexcept { return m_Time; }
    long double GetMilliseconds() const noexcept { return m_Time * 1000.0L; }

private:
    long double m_Time;
};
