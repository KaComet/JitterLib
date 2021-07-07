#include "JitLTimer.h"

namespace Jit {
    JitLTimer::JitLTimer() {
        //Initialize the variables
        mStartTicks = 0;
        mPausedTicks = 0;

        mPaused = false;
        mStarted = false;
    }

    void JitLTimer::start() {
        //Start the timer
        mStarted = true;

        //Unpause the timer
        mPaused = false;

        //Get the current clock time
        mStartTicks = SDL_GetTicks();
        mPausedTicks = 0;
    }

    void JitLTimer::stop() {
        //Stop the timer
        mStarted = false;

        //Unpause the timer
        mPaused = false;

        //Clear tick variables
        mStartTicks = 0;
        mPausedTicks = 0;
    }

    void JitLTimer::pause() {
        //If the timer is running and isn't already paused
        if (mStarted && !mPaused) {
            //Pause the timer
            mPaused = true;

            //Calculate the paused ticks
            mPausedTicks = SDL_GetTicks() - mStartTicks;
            mStartTicks = 0;
        }
    }

    void JitLTimer::unpause() {
        //If the timer is running and paused
        if (mStarted && mPaused) {
            //Unpause the timer
            mPaused = false;

            //Reset the starting ticks
            mStartTicks = SDL_GetTicks() - mPausedTicks;

            //Reset the paused ticks
            mPausedTicks = 0;
        }
    }

    void JitLTimer::restart() {
        stop();
        start();
    }

    Uint32 JitLTimer::getTicks() const {
        //The actual timer time
        Uint32 time = 0;

        //If the timer is running
        if (mStarted) {
            //If the timer is paused
            if (mPaused) {
                //Return the number of ticks when the timer was paused
                time = mPausedTicks;
            } else {
                //Return the current time minus the start time
                time = SDL_GetTicks() - mStartTicks;
            }
        }

        return time;
    }

    long double JitLTimer::getSeconds() const {
        return (long double) getTicks() / (long double) 1000;
    }

    bool JitLTimer::isStarted() const {
        // Return the timers state.
        return mStarted;
    }

    bool JitLTimer::isPaused() const {
        //Timer is running and paused
        return mPaused && mStarted;
    }
}