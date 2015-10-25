#ifndef Tasks_h
#define Tasks_h

/**
 * @file Tasks.h
 * @author PhoneDeveloper, LLC
 * @date 10/10/2015
 * @brief Header file for Tasks library
 * 
 * This is the header file for the Tasks library for Arduino.
 * 
 * Requires Callback.h from the Callback Arduino library.
 * 
 * The Tasks library allows you to schedule() a function or method for future
 * execution. You specify the name of the function, an optional parameter to
 * pass it, and the length of the delay.
 * 
 * You then repeatedly call the dispatch() method in your sketch's loop() which
 * will returns quickly if it isn't time to execute, or will execute a stored 
 * functions and methods when it is time.
 * 
 * It's possible to delay execution of a method in an class. If the method
 * is a class method, you just provide the full name of the class method
 * (such as MyClass::myMethod). If you wish to call an instance method,
 * your class must implement callback() method of the Delayable 
 * interface. callback() takes one parameter, a void*, in which you
 * can use casting to pass anything you want including data types
 * less than or equal in size to sizeof(void*).
 * 
 * Tasks does not use threads, timers, or run within an interrupt. Execution
 * of tasks is performed by dispatch() which cannot return until after
 * the executing task returns. If there is no task ready to execute
 * when dispatch() is called, it returns quickly. This permits the developer
 * to do other tasks in loop().
 * 
 * Tasks also lets you specify a function or method that will be called
 * once for every call to dispatch(), except when a scheduled task is
 * called instead by dispatch(). The function to be called each time
 * can be set or changed using setLoopFunction() or setLoopMethodInstance().
 * The word "loop" in the name implies that it runs repeatedly, like
 * Arduino's "loop". This functionality is provided to let you organize
 * your code better, by allowing you to move recurring task-specific
 * functions from the Arduino loop() into a task-specific loop (which
 * is still called from the Arduino loop() if you call dispatch() from
 * there).
 * 
 * When you combine schedule() and setLoopFunction()/setLoopMethodInstance(),
 * you can separate and organize functionality into tasks, and build larger
 * sketches that are more maintainable.
 * 
 * CAVEAT: if a scheduled task or loop function takes too long to return,
 * dispatch() will also not return promptly and will not be able to make
 * calls to scheduled tasks in a timely fashion. Because a delayed task 
 * or a loop function/method can only run when dispatch() runs, execution
 * of delayed tasks can occur long after they are ready to run, if 
 * something else is preventing dispatch() from running.
 * 
 * More information about Tasks can be found in the README.md file of
 * the Tasks GitHub project at https://github.com/PhoneDeveloper/Tasks.
 *
 * Copyright (c) 2015, PhoneDeveloper, LLC
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */
#include "Arduino.h"

/*
 * Use callbacks "library" for typedefs and class interface for 
 * callback function/method pointers
 */
#include "Callback.h" // <-- Hey! Add this line to the top of your sketch!

// Update this whenever releasing a new version of the library to Github
const static char* TASKS_LIBRARY_VERSION = "0.0.4";

/*
 * Used in place of millis() to reduce execution time.
 */
extern volatile unsigned long timer0_millis;

/*
 * A class implementing this interface can provide a loop method
 * that can be called from dispatch() if Tasks is told the
 * class instance using setLoopMethodInstance().
 */
class Loopable
{
public:
    virtual void loop();
};

/*
 * An interface that defines call(). This allows us to
 * call back functions that take different parameters by
 * allowing a subclass to define how the call() function
 * calls the callback.
 * 
 * Since Tasks supports scheduling of more than one
 * scheduled task at a time, Tasks keeps a pointer to the
 * one which must execute soonest, and each task's next
 * pointer holds a pointer to the task which most closely
 * follows it.
 * 
 * Each ScheduledTask also holds the time it should be
 * executed (timeout).
 */
class ScheduledTask
{
public:
    virtual void call();

protected:
    ScheduledTask* next = NULL;
    unsigned long timeout;
    friend class Tasks;
};

/*
 * Holds scheduled tasks and the loop method to be called.
 * Provides methods for scheduling callbacks with different
 * signatures and dispatching tasks and a loop function or method.
 */ 
class Tasks
{
private:
    ScheduledTask* head = NULL;
    void schedule(ScheduledTask* timeout);
    Callback loopTask = NULL;
    Loopable* loopInstance = NULL;

public:
    ~Tasks();
    boolean dispatch();

    bool setLoopFunction(Callback loopTask);
    bool setLoopMethodInstance(Loopable* loopInstance);
    bool schedule(Callback callback, unsigned long delay);
    bool schedule(CallbackTakesBool callback, unsigned long delay, bool value);
    bool schedule(CallbackTakesFloat callback, unsigned long delay, float value);
    bool schedule(CallbackTakesDouble callback, unsigned long delay, double value);
    bool schedule(CallbackTakesCharPointer callback, unsigned long delay, char* value);
    bool schedule(CallbackTakesString callback, unsigned long delay, String value);
    bool schedule(CallbackTakesChar callback, unsigned long delay, char value);
    bool schedule(CallbackTakesUnsignedChar callback, unsigned long delay, unsigned char value);
    bool schedule(CallbackTakesInt callback, unsigned long delay, int value);
    bool schedule(CallbackTakesUnsignedInt callback, unsigned long delay, unsigned int value);
    bool schedule(CallbackTakesLong callback, unsigned long delay, long value);
    bool schedule(CallbackTakesUnsignedLong callback, unsigned long delay, unsigned long value);
    bool schedule(CallbackTakesVoidPointer callback, unsigned long delay, void* pointer);
    bool schedule(Callable* listener, unsigned long delay, void* pointer = NULL);
};

//
// Tasks - subclasses of ScheduledTask to support each callback signature
//
// Below, we've defined multiple children of ScheduledTask, each of
// which supports a callback that takes a different parameter.
// 
// Each task must implement the call() method from ScheduledTask and
// if providing a value to its callback, store that value.
//
// The constructor sets up the task by accepting the function to be
// called back, the delay time, and any parameter to be passed. The
// converts the delay time to the time in millis() at which the task
// should be executed.
//
class Task : public ScheduledTask
{
public:
    Task(Callback callback, unsigned long delay);
    void call();

private:
    Callback callback;
};

class TaskTakesBool : public ScheduledTask
{
public:
    TaskTakesBool(CallbackTakesBool callback, bool value, unsigned long delay);
    void call();

private:
    CallbackTakesBool callback;
    bool value;
};

class TaskTakesFloat : public ScheduledTask
{
public:
    TaskTakesFloat(CallbackTakesFloat callback, float value, unsigned long delay);
    void call();

private:
    CallbackTakesFloat callback;
    float value;
};

class TaskTakesDouble : public ScheduledTask
{
public:
    TaskTakesDouble(CallbackTakesDouble callback, double value, unsigned long delay);
    void call();

private:
    CallbackTakesDouble callback;
    double value;
};

class TaskTakesCharPointer : public ScheduledTask
{
public:
    TaskTakesCharPointer(CallbackTakesCharPointer callback, char* value, unsigned long delay);
    void call();

private:
    CallbackTakesCharPointer callback;
    char* value;
};

class TaskTakesString : public ScheduledTask
{
public:
    TaskTakesString(CallbackTakesString callback, String value, unsigned long delay);
    void call();

private:
    CallbackTakesString callback;
    String value;
};

class TaskTakesChar : public ScheduledTask
{
public:
    TaskTakesChar(CallbackTakesChar callback, char value, unsigned long delay);
    void call();

private:
    CallbackTakesChar callback;
    char value;
};

class TaskTakesUnsignedChar : public ScheduledTask
{
public:
    TaskTakesUnsignedChar(CallbackTakesUnsignedChar callback, unsigned char value, unsigned long delay);
    void call();

private:
    CallbackTakesUnsignedChar callback;
    unsigned char value;
};

class TaskTakesInt : public ScheduledTask
{
public:
    TaskTakesInt(CallbackTakesInt callback, int value, unsigned long delay);
    void call();

private:
    CallbackTakesInt callback;
    int value;
};

class TaskTakesUnsignedInt : public ScheduledTask
{
public:
    TaskTakesUnsignedInt(CallbackTakesUnsignedInt callback, unsigned int value, unsigned long delay);
    void call();

private:
    CallbackTakesUnsignedInt callback;
    unsigned int value;
};

class TaskTakesLong : public ScheduledTask
{
public:
    TaskTakesLong(CallbackTakesLong callback, long value, unsigned long delay);
    void call();

private:
    CallbackTakesLong callback;
    long value;
};

class TaskTakesUnsignedLong : public ScheduledTask
{
public:
    TaskTakesUnsignedLong(CallbackTakesUnsignedLong callback, unsigned long value, unsigned long delay);
    void call();

private:
    CallbackTakesUnsignedLong callback;
    unsigned long value;
};

class TaskTakesVoidPointer : public ScheduledTask
{
public:
    TaskTakesVoidPointer(CallbackTakesVoidPointer callback, void* pointer, unsigned long delay);
    void call();

private:
    CallbackTakesVoidPointer callback;
    void* pointer;
};

/*
 * A ScheduledTask that can call back a method in an instance of a class
 * 
 * This ScheduledTask client can call a method that's contained in an
 * instance of a class (an instance method.) The class must extend
 * "Callable" and implement the "callback()" method defined
 * in "Callable".
 */
class MethodTask : public ScheduledTask
{
public:
    MethodTask(Callable* listener, void* pointer, unsigned long delay);
    void call();

private:
    Callable* listener;
    void* pointer;
};

#endif