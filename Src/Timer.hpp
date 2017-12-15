#pragma once
#include <chrono>

using hrc = std::chrono::high_resolution_clock;
using ms = std::chrono::milliseconds;

class Timer
{
public:
    Timer::Timer()
    {
        has_paused_ = false;
        stopped_ = false;
    }

    Timer::~Timer()
    {
    }

    void Timer::start()
    {
        reset();
    }

    void Timer::reset()
    {
        start_ = pause_ = resume_ = hrc::now();
        has_paused_ = false;
        stopped_ = false;
    }

    void Timer::stop()
    {
        stop_ = hrc::now();
        stopped_ = true;
    }

    void Timer::pause()
    {
        pause_ = hrc::now();
        has_paused_ = true;
        resume_ = hrc::now();
    }

    void Timer::resume()
    {
        resume_ = hrc::now();
    }

    double Timer::elapsed() const
    {
        std::chrono::duration<double> duration;
        if (has_paused_)
        {
            if (stopped_)
            {
                duration = stop_ - resume_ + (pause_ - start_);
            }
            else
            {
                duration = hrc::now() - resume_ + (pause_ - start_);
            }

            return duration.count();
        }		

        if (stopped_)
        {
            duration = stop_ - start_;
        }
        else
        {
            duration = hrc::now() - start_;
        }

        return duration.count();
    }

private:
    hrc::time_point start_, pause_, resume_, stop_;
    bool has_paused_;
    bool stopped_;

};
