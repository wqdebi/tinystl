#include "interupthread.h"

thread_local interrupt_flag this_thread_interrupt_flag;
void interruptible_wait(std::condition_variable& cv,
                        std::unique_lock<std::mutex>& lk)
{
    interruption_point();
    this_thread_interrupt_flag.set_condition_variable(cv);
    clear_cv_on_destruct guard;
    interruption_point();
    cv.wait_for(lk, std::chrono::milliseconds(1));
    interruption_point();
}

void interruption_point()
{
    if (this_thread_interrupt_flag.is_set())
    {
        throw thread_interrupted();
    }
}

clear_cv_on_destruct:: ~clear_cv_on_destruct() {

    this_thread_interrupt_flag.clear_condition_variable();

}