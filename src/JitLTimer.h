#ifndef SDL_TEST_LTIMER_H
#define SDL_TEST_LTIMER_H

#include <SDL.h>

namespace Jit {
    class JitLTimer {
    public:
        //Initializes variables
        JitLTimer();

        //The various clock actions
        void start();

        void stop();

        void pause();

        void unpause();

        //Gets the timer's time
        [[nodiscard]] Uint32 getTicks() const;

        [[nodiscard]] long double getSeconds() const;

        //Checks the status of the timer
        [[nodiscard]] bool isStarted() const;

        [[nodiscard]] bool isPaused() const;

    private:
        //The clock time when the timer started
        Uint32 mStartTicks;

        //The ticks stored when the timer was paused
        Uint32 mPausedTicks;

        //The timer status
        bool mPaused;
        bool mStarted;
    };
}


#endif //SDL_TEST_LTIMER_H
