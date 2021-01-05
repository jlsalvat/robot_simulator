#include "Timer.h"
#include <SDL2/SDL.h>
#include <cassert>

Timer::Timer()
{
    //Initialize the variables
    mStartTicks = 0;
    mPausedTicks = 0;

    mPaused = false;
    mStarted = false;
}

void Timer::start()
{
    //Start the timer
    mStarted = true;

    //Unpause the timer
    mPaused = false;

    //Get the current clock time
    mStartTicks = SDL_GetTicks();
    mPausedTicks = 0;
}

void Timer::stop()
{
    //Stop the timer
    mStarted = false;

    //Unpause the timer
    mPaused = false;

    //Clear tick variables
    mStartTicks = 0;
    mPausedTicks = 0;
}

void Timer::reset()
{
    //Clear tick variables
    mStartTicks = SDL_GetTicks();
    mPausedTicks = SDL_GetTicks();
}

void Timer::pause()
{
    //If the timer is running and isn't already paused
    if( mStarted && !mPaused )
    {
        //Pause the timer
        mPaused = true;

        //Calculate the paused ticks
        mPausedTicks = SDL_GetTicks() - mStartTicks;
        mStartTicks = 0;
    }
}

void Timer::unpause()
{
    //If the timer is running and paused
    if( mStarted && mPaused )
    {
        //Unpause the timer
        mPaused = false;

        //Reset the starting ticks
        mStartTicks = SDL_GetTicks() - mPausedTicks;

        //Reset the paused ticks
        mPausedTicks = 0;
    }
}

uint32_t Timer::getTicks()
{
    //The actual timer time
    uint32_t time = 0;

    //If the timer is running
    if( mStarted )
    {
        //If the timer is paused
        if( mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - mStartTicks;
        }
    }

    return time;
}

bool Timer::isStarted()
{
    //Timer is running and paused or unpaused
    return mStarted;
}

bool Timer::isPaused()
{
    //Timer is running and paused
    return mPaused && mStarted;
}

template< typename... Args >
std::string Timer::string_sprintf(const char* format, Args... args) {
	int length = std::snprintf(nullptr, 0, format, args...);
	assert(length >= 0);

	char* buf = new char[length + 1];
	std::snprintf(buf, length + 1, format, args...);

	std::string str(buf);
	delete[] buf;
	return str;
}

std::string Timer::getTime() {
	std::string time = "";
	int minute=0, heure=0, seconde=0, milli=0;
	uint32_t i_time =  getTicks();
	milli = i_time % 1000;
	seconde = i_time / 1000;
	heure = i_time / 3600;
	minute = i_time - heure * 60 - seconde * 60;
	time += string_sprintf("%d", seconde)+":";
	time += string_sprintf("%000d", milli);
	if (i_time > 60 * 1000) {
		time += minute + ":" + time;
	}
	if (i_time > 3600 * 1000) {
		time += heure + ":" + time;
	}
	time += " ms";
	return time;
}
