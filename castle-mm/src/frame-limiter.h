#pragma once
#include <chrono>

using sc = std::chrono::system_clock;

struct FrameLimiter {
    sc::time_point a = sc::now();
    sc::time_point b = sc::now();
    double maxDeltaTime{ 1.0f / 60.0f };

    inline FrameLimiter(const double maxDeltaTime) { init(maxDeltaTime); }
    void init(const double maxDeltaTime);
    void update();
};
