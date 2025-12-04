#pragma once

#define SCREEN_WIDTH 1200
#define SCREEN_HEIGHT 800
#define PI 3.14159265358979323846f

#include <cmath>

inline double wrapToPi(double a) {
    constexpr double TWO_PI = 2.0 * PI;
    // 把 a 移到以 -PI 为基准的区间，然后取模，再修正到 [ -PI, PI )
    a = std::fmod(a + PI, TWO_PI);    // 结果在 (-2π, 2π)
    if (a < 0.0) a += TWO_PI;         // 结果在 [0, 2π)
    return a - PI;                    // 结果在 [-π, π)
}

inline float wrapToPiF(float a) {
    constexpr float TWO_PI = 2.0f * PI;
    a = std::fmod(a + PI, TWO_PI);
    if (a < 0.0f) a += TWO_PI;
    return a - PI;
}