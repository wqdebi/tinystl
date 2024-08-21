#pragma once
#include <thread>
#include <atomic>
#include <condition_variable>
#include <mutex>
#include <iostream>
#include <exception>
#include <string>
#include <future>

extern void interruption_point();

class interrupt_flag
{
    std::atomic<bool> flag;
    std::condition_variable* thread_cond;
    std::condition_variable_any* thread_cond_any;
    std::mutex set_clear_mutex;
public:
    interrupt_flag() :
            thread_cond(0), thread_cond_any(0)
    {}
    void set()
    {
        flag.store(true, std::memory_order_relaxed);
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        if (thread_cond)
        {
            thread_cond->notify_all();
        }
        else if (thread_cond_any) {
            thread_cond_any->notify_all();
        }
    }
    bool is_set() const
    {
        return flag.load(std::memory_order_relaxed);
    }
    void set_condition_variable(std::condition_variable& cv)
    {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond = &cv;
    }
    void clear_condition_variable()
    {
        std::lock_guard<std::mutex> lk(set_clear_mutex);
        thread_cond = 0;
    }


    template<typename Lockable>
    void wait(std::condition_variable_any& cv, Lockable& lk) {
        struct custom_lock {
            interrupt_flag* self;
            Lockable& lk;
            custom_lock(interrupt_flag* self_, std::condition_variable_any& cond, Lockable& lk_) :
                    self(self_), lk(lk_) {
                self->set_clear_mutex.lock();
                self->thread_cond_any = &cond;
            }

            void unlock() {
                lk.unlock();
                self->set_clear_mutex.unlock();
            }

            void lock() {
                std::lock(self->set_clear_mutex, lk);
            }

            ~custom_lock() {
                self->thread_cond_any = 0;
                self->set_clear_mutex.unlock();
            }
        };

        custom_lock cl(this, cv, lk);
        interruption_point();
        cv.wait(cl);
        interruption_point();
    }
};


struct clear_cv_on_destruct {
    ~clear_cv_on_destruct();
};

extern thread_local interrupt_flag this_thread_interrupt_flag;

class thread_interrupted : public std::exception
{
public:
    thread_interrupted() : message("thread interrupted.") {}
    ~thread_interrupted() throw () {
    }

    virtual const char* what() const throw () {
        return message.c_str();
    }

private:
    std::string message;
};

class interruptible_thread
{
private:
    std::thread internal_thread;
    interrupt_flag* flag;
public:
    template<typename FunctionType>
    interruptible_thread(FunctionType f)
    {
        //⇽-- - 2
        std::promise<interrupt_flag*> p;
        //⇽-- - 3
        internal_thread = std::thread([f, &p] {
            p.set_value(&this_thread_interrupt_flag);
            //⇽-- - 4
            f();
        });
        //⇽-- - 5
        flag = p.get_future().get();
    }

    void join() {
        internal_thread.join();
    }
    void interrupt()
    {
        if (flag)
        {
            //⇽-- - 6
            flag->set();
        }
    }
};


extern void interruptible_wait(std::condition_variable& cv,
                               std::unique_lock<std::mutex>& lk);


template<typename Predicate>
void interruptible_wait(std::condition_variable& cv,
                        std::unique_lock<std::mutex>& lk,
                        Predicate pred)
{
    interruption_point();
    this_thread_interrupt_flag.set_condition_variable(cv);
    clear_cv_on_destruct guard;
    while (!this_thread_interrupt_flag.is_set() && !pred())
    {
        cv.wait_for(lk, std::chrono::milliseconds(1));
    }
    interruption_point();
}

template<typename Lockable>
void interruptible_wait(std::condition_variable_any& cv, Lockable& lk) {
    this_thread_interrupt_flag.wait(cv, lk);
}

