#pragma once

#include <cstdint>
#include <string>

class Timer
{
public:
    //Initializes variables
    Timer();

    //The various clock actions
    void start();
    void stop();
    void reset();
    void pause();
    void unpause();

    //Gets the timer's time
    uint32_t getTicks();
	std::string getTime();

    //Checks the status of the timer
    bool isStarted();
    bool isPaused();

private:
	template< typename... Args >
	std::string  string_sprintf(const char* format, Args... args);
    //The clock time when the timer started
    uint32_t mStartTicks;

    //The ticks stored when the timer was paused
    uint32_t mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;
};
