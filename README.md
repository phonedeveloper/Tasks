# Tasks
...a replacement for **delay()** that doesn't stop the **loop()**.

Requires the [Callback](https://github.com/phonedeveloper/Callback) library.

### Typical use

Use **Tasks** to **schedule()** a function to be called some time in the future. Then, in your sketch's **loop()**, call **dispatch()** repeatedly, and **dispatch()** will run those tasks when their time has arrived:

Library setup:

```
#include "Callback.h" // required by Tasks
#include "Tasks.h"
Tasks myTask;
```

Loop:

```
void loop() {
  myTask.dispatch();
  ...
}
```

A function that uses Tasks to call itself after a delay:

```
void setLedState(int state) {
  digitalWrite(13, state);
  myTask.schedule(setLedState, 1000, !state);
}
```
Using the above, start an LED blinking:

```
void setup() {
  pinMode(13, OUTPUT);
  setLedState(LOW);
}
```

### More advanced uses

More advanced users can schedule not only functions but the **callable()** method of a class that implements Callable.

Additionally, **setLoopFunction()** / **setLoopMethodInstance** can be used to specify a function or method to be called when **dispatch()** is called and doesn't have a scheduled function or method to run. This lets you isolate code functions into Tasks that have a recurring loop(), and have multiple Tasks that run "independently" as long as they are "nice" and return promptly.

Examples of these uses can be found in the [TasksTest](https://github.com/phonedeveloper/Tasks/examples/TasksTest) sketch found in the examples folder.

# Quickstart Example
See [BlinkUsingTasks](https://github.com/phonedeveloper/Tasks/examples/BlinkUsingTasks) in the examples folder for the basic use of Tasks as a replacement for **delay()**.

# Caveats
You must add

```
#define "Callback.h"
```
ahead of the line

```
#define "Tasks.h"
```
in your sketch.

Tasks requires that no other code in the sketch is "long running". That is, no other code runs so long that `dispatch()` is not called sufficiently frequently.

Timing is only as accurate as how often `dispatch()` is called, which depends on what else is in loop().

Tasks is not implemented using interrupts or timers and is intended to be used only on the main Arduino thread (from Arduino's **setup()** or **loop()** functions. If you try to **schedule()** a task or run **dispatch()** from within an interrupt or timer, you will eventually break the library.

The maximum delay is 2^32 / 2 (the maximum positive integer a **long** can represent). If specify a delay longer than this, it may execute much sooner than you expect.

The library's **schedule()** method returns **false** if memory cannot be allocated for the delayed function. However, by then it may be too late; there may not be enough memory for your app to continue. If you think you may run low on memory, put your own memory checks anywhere you might allocate memory. **schedule()** consumes somewhere around a dozen bytes for each added function, which is released when the function is eventually run.

# How fast is it?
If there is no function ready to be called, a call to **Tasks**' **dispatch()** returns in around 10 *microseconds*, independent of the number of pending functions. Because functions are sorted and stored in order of execution time, **dispatch()** only needs to look at the first one when checking if one is ready to run.

Sorting of the list entries is done by **schedule()** very simply: it walks the list until it finds a function whose execution time is later than the one it is inserting.
# How many functions can be pending at once?
The limitation is available memory.

## Heap fragmentation
**Tasks** creates and deletes objects that hold delayed function information. As mentioned before, these objects consume about a dozen bytes of memory each. These are created on the heap by **schedule()** and deleted after execution by **dispatch()**.

Since they are created on the stack, when they are deleted they can leave holes of about a dozen bytes if additional memory was created afterwards. These holes can be filled by future calls to **schedule()** or by other things that consume the heap.

The original [Actions](https://github.com/phonedeveloper/Actions) library, which this library is inteded to replace, avoids this by preallocating a fixed size queue.

## Installation
From the command line, go to your **sketchbook** folder. Inside is a folder named **libraries** (if not, create it). **cd** to the **sketchbook\libraries** folder, and if there are no other libraries named **Tasks** or **Callback** in that folder, run the following command:

`$ git clone https://github.com/phonedeveloper/Callback.git`
`$ git clone https://github.com/phonedeveloper/Tasks.git`

This will install the **Tasks** library, the **Callback** library on which it depends, and some examples. Restart the Arduino app to see them.

## Key Functions

`Tasks task` - creates a Tasks instance called **task** that can handle multiple postponed functions or methods.

`task.schedule(function, delay)` - sets **function**, which takes no parameters, to be called **delay** milliseconds in the future. **function** must have the following signature:

```
void function() {...}
```

`task.schedule(function, delay, value)` - sets **function**, which takes the **value** parameter, to be called **delay** milliseconds in the future and passed **value**. **function** must have one of the following signatures:
```
void function(bool) {...}
void function(char) {...}
void function(int) {...}
void function(long) {...}
void function(unsigned char) {...}
void function(unsigned int) {...}
void function(unsigned long) {...}
void function(float) {...}
void function(double) {...}
void function(char*) {...}
void function(String) {...}
void function(void*) {...}

`task.set(instance, delay)` - sets **function**, which takes the **instance** parameter which is a pointer to an instance of a class that implements the **callback()** method of the **Callable** interface.

`task.set(instance, delay, value)` - as above, where value is a void* that will be passed to the instance's **callback()** method.

`task.dispatch()` - call this repeatedly from your sketch's **loop()**. It looks at the queue and calls any callback whose **delay** has passed since it was added. 

## Some usage tips

Sometimes you will want a timeout to be repeated some time later, as shown in the example above. For this to happen, you should call `schedule()` before you return from the function/method. If you only need to run a task once, just let it return. An example of this appears in [BlinkUsingTasks](https://github.com/phonedeveloper/Tasks/examples/BlinkUsingTasks).

Be sure to call `task.dispatch()` repeatedly, such as from within your sketch's `loop()` method, keeping in mind any long-running operations might delay the execution of a callback. If your loop and callback functions/methods are short (for instance, they are only reading and writing digital lines), you can process thousands of timeouts per second.

### Under the hood

Only one scheduled task is called per loop, even if several tasks happen to be ready at the same time. If multiple tasks come ready during the same `dispatch()`, the first one that was added (the older task) will be the first one that is run.

Take a look at the source code for **Tasks::dispatch()** and **Tasks::schedule()** if you want to understand more about how tasks are added and removed from the queue.


## Power Users
### Ensuring "high priority" tasks are called before "low priority" tasks
If you need to prioritize function tasks, you can do this by creating two queues:

```
Tasks highPriorityFunctions;
Tasks lowPriorityFunctions;

void loop() {
    if (! highPriorityFunctions.dispatch()) {
        lowPriorityFunctions.dispatch();
    }
}
```
This is an equivalent implementation:

```
void loop() {
    while(highPriorityFunctions.dispatch());
    lowPriorityFunctions.dispatch();
}
```

The `dispatch()` method returns **true** if a task was called, and **false** if not. This code ensures that if multiple high priority tasks are ready to go at the same time, they are all executed before any low priority tasks are called. It also makes sure that only one low priority tasks is run so that if a high priority task becomes ready, it runs before a second low priority task that becomes ready just before it.

If a low priority task is running and taking its jolly time, and a high priority task becomes ready, the high priority task is not going to run until the low priority task completes.

Remember that each call to `task.dispatch()` only results in at most one task being called. If several tasks are ready, it will take the same number of calls to the task's `dispatch()` to process them all.

If a looping function has been specified using **setLoopFunction** or **setLoopMethodInstance**, that method will be called only once during every call to **dispatch()** unless **dispatch()** is supposed to call a task during its current round.

## License
(c) 2015, PhoneDeveloper LLC

Licensed under the BSD license. See the LICENSE file.

## Author
Sean Sheedy, PhoneDeveloper LLC