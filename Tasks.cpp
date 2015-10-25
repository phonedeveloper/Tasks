/*
 * Tasks.cpp
 *
 * Author: PhoneDeveloper, LLC
 * Date: 10/10/2015
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
#include "Tasks.h"

/*
 * dispatch() - calls tasks when it is time, and any stored loop function/method.
 *
 * Looks at the task that would be executed first, and if ready to
 * execute, calls its function and removes it from the list of
 * pending tasks.
 */
boolean Tasks::dispatch()
{
    // Check if a task is ready to be called. If so, call it and return after it exits.
    if((head != NULL) && ((long)(timer0_millis - head->timeout) >= 0))
    {
        // remove from list now, in case the callback modifies list by calling schedule()
        ScheduledTask* timeout = head;
        head = timeout->next;
        // call it
        timeout->call();
        // delete it.
        delete timeout;
        return true; // callback was called
    }

    // Check if a loopTask is installed. If so, call it.
    else
    {
        if(loopTask != NULL)
        {
            loopTask();
        }
        else if(loopInstance != NULL)
        {
            loopInstance->loop();
        }

        return false; // indicate that no scheduled task was called
    }
}

/*
 * schedule - requests the supplied function to be run at a later time.
 *
 * The caller must provide the name of the function to run, as well
 * as the delay in milliseconds, and depending on the task, a parameter
 * to pass to it.  
 * 
 * An example call to this method would look like this, where function is
 * void someFunction(boolean);  // declared here, defined elsewhere
 *
 * unsigned long delay = 1000ul;  // how long after now to call the function
 * boolean parameter = true;      // the parameter to pass to the function
 * bool result = schedule( someFunction, delay, parameter );
 *
 * Note the lack of parenthesis after the function name.
 */
bool Tasks::schedule(Callback callback, unsigned long delay)
{
    Task* callbackInstance = new Task(callback, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesBool callback, unsigned long delay, bool value)
{
    TaskTakesBool* callbackInstance = new TaskTakesBool(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesFloat callback, unsigned long delay, float value)
{
    TaskTakesFloat* callbackInstance = new TaskTakesFloat(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesDouble callback, unsigned long delay, double value)
{
    TaskTakesDouble* callbackInstance = new TaskTakesDouble(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesCharPointer callback, unsigned long delay, char* value)
{
    TaskTakesCharPointer* callbackInstance = new TaskTakesCharPointer(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesString callback, unsigned long delay, String value)
{
    TaskTakesString* callbackInstance = new TaskTakesString(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesChar callback, unsigned long delay, char value)
{
    TaskTakesChar* callbackInstance = new TaskTakesChar(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesUnsignedChar callback, unsigned long delay, unsigned char value)
{
    TaskTakesUnsignedChar* callbackInstance = new TaskTakesUnsignedChar(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesInt callback, unsigned long delay, int value)
{
    TaskTakesInt* callbackInstance = new TaskTakesInt(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesUnsignedInt callback, unsigned long delay, unsigned int value)
{
    TaskTakesUnsignedInt* callbackInstance = new TaskTakesUnsignedInt(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesLong callback, unsigned long delay, long value)
{
    TaskTakesLong* callbackInstance = new TaskTakesLong(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesUnsignedLong callback, unsigned long delay, unsigned long value)
{
    TaskTakesUnsignedLong* callbackInstance = new TaskTakesUnsignedLong(callback, value, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}
bool Tasks::schedule(CallbackTakesVoidPointer callback, unsigned long delay, void* pointer)
{
    TaskTakesVoidPointer* callbackInstance = new TaskTakesVoidPointer(callback, pointer, delay);
    if(callbackInstance == NULL) // out of memory?
    {
        return false;
    }
    else // successfully created Callback subclass instance; now create ScheduledTask
    {
        schedule(callbackInstance);
        return true;
    }
}

/*************************************************************************
 * PUBLIC METHODS THAT MORE ADVANCED SKETCHES MIGHT USE                  *
 *                                                                       *
 * The methods below are those that would be used by sketches needing to *
 * do more than just delay execution of a function.                      *
 *************************************************************************/

/*
 * schedule(instance, delay, parameter) - call a class instance method after a delay
 *
 * The caller must provide a pointer to the instance, the delay in 
 * milliseconds, and optionally, a void pointer to pass as a parameter.
 */
bool Tasks::schedule(Callable* listener, unsigned long delay, void* pointer)
{
    MethodTask* listenerCallback = new MethodTask(listener, pointer, delay);
    if(listenerCallback == NULL) // out of memory?
    {
        return false;
    }
    else
    {
        schedule(listenerCallback);
        return true;
    }
}

/*
 * Replaces the current loopTask or loopInstance with the provided loop function.
 * We only support one looper per Task.
 */
boolean Tasks::setLoopFunction(Callback loopFunction)
{
    loopTask = loopFunction;
    loopInstance = NULL;
}

/*
 * Replaces the current loopTask or loopInstance with the provided loop class instance.
 * We only support one looper per Task.
 */
boolean Tasks::setLoopMethodInstance(Loopable* loopingClassInstance)
{
    loopInstance = loopingClassInstance;
    loopTask = NULL;
}


/***********************************************
 * METHODS THAT SKETCHES WILL *NOT* USE        *
 *                                             *
 * The methods below are used by the library.  *
 ***********************************************/

/*
 * call - Calls a callback function or method
 *
 * Two types of callbacks can be stored in Callback:
 * - an ordinary function (not part of a class)
 * - a method in an instantiated class
 *
 * This method calls the type currently stored.
 */
void Task::call()
{
    callback();
}
void TaskTakesBool::call()
{
    callback(value);
}
void TaskTakesFloat::call()
{
    callback(value);
}
void TaskTakesDouble::call()
{
    callback(value);
}
void TaskTakesCharPointer::call()
{
    callback(value);
}
void TaskTakesString::call()
{
    callback(value);
}
void TaskTakesChar::call()
{
    callback(value);
}
void TaskTakesUnsignedChar::call()
{
    callback(value);
}
void TaskTakesInt::call()
{
    callback(value);
}
void TaskTakesUnsignedInt::call()
{
    callback(value);
}
void TaskTakesLong::call()
{
    callback(value);
}
void TaskTakesUnsignedLong::call()
{
    callback(value);
}
void TaskTakesVoidPointer::call()
{
    callback(pointer);
}
void MethodTask::call()
{
    listener->callback(pointer);
}

/*
 * set - places a timeout in the list of timeouts, sorted by when the timeout will occur (soonest to latest)
 */
void Tasks::schedule(ScheduledTask* timeout)
{
    ScheduledTask* previous = NULL; // Set up for first pass through loop
    ScheduledTask* current = head;

    while(current != NULL) // not at end of list?
    {
        // Note: the following test not only causes the timeout to be inserted after earlier ones,
        // if it has the same timeout as one added earlier, it will be queued behind that one,
        // so that the older entry is executed before the newer one (given that the execution times
        // are the same). This really should not matter but it keeps the execution in the order
        // presented if schedule() is called multiple times in quick succession with the same delay
        // value for each function.
        if((long)(current->timeout - timeout->timeout) > 0) // if new timeout occurs before that saved
        {
            timeout->next = current; // move current after this one
            if(previous == NULL)     // if no previous ScheduledTask
            {
                head = timeout; // then this one is the first in the list
            }
            else // but if there is an earlier timeout
            {
                previous->next = timeout; // point from that one to this one.
            }
            return; // and we're done!
        }
        else // new timeout occurs later than current?
        {
            previous = current; // then check next timeout in next iteration
            current = current->next;
        }
    }

    // current is NULL; we reached end of list without finding a longer timeout
    if(previous == NULL) // if there were no timeouts already in the list
    {
        head = timeout; // make this one the first in the list
    }
    else // else if the previous timeout was the last one
    {
        previous->next = timeout; // make this one the last one on in the list
    }
}

/*
 * Deletes any objects stored in the list of delayed functions/methods.
 */
Tasks::~Tasks()
{
    ScheduledTask* timeout = head;
    while(timeout != NULL)
    {
        ScheduledTask* discarded = timeout;
        timeout = timeout->next;
        delete discarded;
    }
}

/*
 * Creates a task from the provided callback and delay
 * (and stores the parameter to call the task, if any).
 * Converts the delay time into an absolute time value
 * by adding the delay time to the current time (millis()).
 */
Task::Task(Callback callback, unsigned long delay)
    : callback(callback)
{
    timeout = timer0_millis + delay;
};

TaskTakesBool::TaskTakesBool(CallbackTakesBool callback, bool value, unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesFloat::TaskTakesFloat(CallbackTakesFloat callback, float value, unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesDouble::TaskTakesDouble(CallbackTakesDouble callback, double value, unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesCharPointer::TaskTakesCharPointer(CallbackTakesCharPointer callback, char* value, unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesString::TaskTakesString(CallbackTakesString callback, String value, unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesChar::TaskTakesChar(CallbackTakesChar callback, char value, unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesUnsignedChar::TaskTakesUnsignedChar(CallbackTakesUnsignedChar callback,
                                             unsigned char value,
                                             unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesInt::TaskTakesInt(CallbackTakesInt callback, int value, unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesUnsignedInt::TaskTakesUnsignedInt(CallbackTakesUnsignedInt callback, unsigned int value, unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesLong::TaskTakesLong(CallbackTakesLong callback, long value, unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesUnsignedLong::TaskTakesUnsignedLong(CallbackTakesUnsignedLong callback,
                                             unsigned long value,
                                             unsigned long delay)
    : callback(callback)
    , value(value)
{
    timeout = timer0_millis + delay;
};

TaskTakesVoidPointer::TaskTakesVoidPointer(CallbackTakesVoidPointer callback, void* pointer, unsigned long delay)
    : callback(callback)
    , pointer(pointer)
{
    timeout = timer0_millis + delay;
};

MethodTask::MethodTask(Callable* listener, void* pointer, unsigned long delay)
    : listener(listener)
    , pointer(pointer)
{
    timeout = timer0_millis + delay;
}
