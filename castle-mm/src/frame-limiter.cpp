#include "frame-limiter.h"
#include <thread>

void FrameLimiter::init(const double maxDeltaTime) {
    a = sc::now();
    b = sc::now();
    this->maxDeltaTime = maxDeltaTime;
}

void FrameLimiter::update() {
    a = sc::now();
    std::chrono::duration<double, std::milli> work_time = a - b;

    if (work_time.count() < maxDeltaTime)
    {
        std::chrono::duration<double, std::milli> delta_ms(maxDeltaTime - work_time.count());
        auto delta_ms_duration = std::chrono::duration_cast<std::chrono::milliseconds>(delta_ms);
        std::this_thread::sleep_for(std::chrono::milliseconds(delta_ms_duration.count()));
    }

    b = sc::now();
    std::chrono::duration<double, std::milli> sleep_time = b - a;
}
