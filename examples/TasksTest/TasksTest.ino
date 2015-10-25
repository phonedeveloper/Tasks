#line 2 "TasksTest.ino"
#include <ArduinoUnit.h>

/*
 * TasksTest - test the Tasks class
 * 
 * This sketch tests the Tasks class. It outputs the
 * test results through the main serial port.
 * 
 * Run these tests after modifying the library to ensure you
 * haven't broken something. Add a test if you find a bug, where
 * the test fails if run on the unfixed library, but passes
 * in a fixed version of the library.
 * 
 * Uses arduinounit from https://github.com/mmurdoch/arduinounit
 * 
 * Executed on an Arduino Mega ADK.
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

#include <Callback.h>
#include <Tasks.h>

//
// These are printed when the Arduino boots. Change version number 
// and compare it to what the Arduino prints to the serial port to
// ensure that the Arduino is actually running an updated test sketch.
// The numbering is (major version).(minor version).(build) such as
// 1.11.364 - major version 1, minor version 11, build 364.
// At a minimum, update the build number whenever a change is made.
//
static const char* VERSION = "0.0.12";
static const char* VERSION_STRING = "TasksTest test sketch version ";

//
// Update these with the version numbers of the libraries against
// which this test suite can be run. The version number is found in the
// top of the header file for each library. If you don't want to test
// older library versions for compatibility, simply put only one
// entry, the library being tested, into the list.
//
static const char* TESTED_TASKS_VERSIONS[] = {"0.0.4"};
static const int TESTED_TASKS_VERSIONS_COUNT = 1;  // must match number of versions in list above

static const char* TESTED_CALLBACK_VERSIONS[] = {"0.0.2"};
static const int TESTED_CALLBACK_VERSIONS_COUNT = 1;  // must match number of versions in list above

// Used in place of millis() for speed:
extern volatile unsigned long timer0_millis;

//
// Variables and functions for function callback tests
//
// The following variables and functions are used by
// one or more tests in the suite.
//
bool functionCalled = false;
void function(){ functionCalled = true; }

bool boolValue = false;
void boolFunction(bool value){ boolValue = value; }

char charValue = 0;
void charFunction(char value){ charValue = value; }

byte byteValue = 0;
void byteFunction(byte value){ charValue = value; }

unsigned char unsignedCharValue = 0;
void unsignedCharFunction(unsigned char value){ unsignedCharValue = value; }

int intValue = 0;
void intFunction(int value){ intValue = value; }

unsigned int unsignedIntValue = 0;
void unsignedIntFunction(unsigned int value){ unsignedIntValue = value; }

long longValue = 0;
void longFunction(long value){ longValue = value; }

unsigned long unsignedLongValue = 0;
void unsignedLongFunction(unsigned long value){ unsignedLongValue = value; }

float floatValue = 0;
void floatFunction(float value){ floatValue = value; }

double doubleValue = 0;
void doubleFunction(double value){ doubleValue = value; }

void* pointerValue = NULL;
void pointerFunction(void* pointer){ pointerValue = pointer; }

char* charString = "empty char*";
String arduinoString = "empty String";

//
// compares two strings, returns true if they match.
// 
boolean compareStrings(const char* actual, const char* expected) {
  boolean retval = true; // return value; assume there is a match
  int i=0;
  int j=0;
  char character;
  while(character = actual[i] != 0) i++;
  while(character = expected[j] != 0) j++;
  if (i != j) {
    retval = false;
  }
  else if (i <= 0) {
    retval = false;
  }
  else if (j <= 0) {
    retval = false;
  }
  else for (int i=0; i<j; i++) {  // compare version strings
    if (actual[i] != expected[i]) {
      retval = false;
      break;
    }
  }
  return retval;
}





//
// Test for library versions first.
//
// We first check the versions of the Callback and Tasks
// libraries, to be sure that we're running against the
// versions for which TasksTest has been updated.
//
test(AA_CallbackLibraryVersion) {
  boolean foundMatch = false;
  for (int i=0; i<TESTED_CALLBACK_VERSIONS_COUNT; i++) {
    if (compareStrings(TESTED_CALLBACK_VERSIONS[i], CALLBACK_LIBRARY_VERSION))
      foundMatch = true;
  }
  if (!foundMatch) {
    Serial.print("Library \"Callback\" version (");
    Serial.print(CALLBACK_LIBRARY_VERSION);
    Serial.println(") doesn't match one of TESTED_CALLBACK_VERSIONS:");
    for (int i=0; i<TESTED_CALLBACK_VERSIONS_COUNT; i++) {
      Serial.println(TESTED_CALLBACK_VERSIONS[i]);
    }
  }
  assertTrue(foundMatch);
}

test(AA_TasksLibraryVersion) {
  boolean foundMatch = false;
  for (int i=0; i<TESTED_TASKS_VERSIONS_COUNT; i++) {
    if (compareStrings(TESTED_TASKS_VERSIONS[i], TASKS_LIBRARY_VERSION))
      foundMatch = true;
  }
  if (!foundMatch) {
    Serial.print("Library \"Tasks\" version (");
    Serial.print(TASKS_LIBRARY_VERSION);
    Serial.println(") doesn't match one of TESTED_TASKS_VERSIONS:");
    for (int i=0; i<TESTED_TASKS_VERSIONS_COUNT; i++) {
      Serial.println(TESTED_TASKS_VERSIONS[i]);
    }
  }
  assertTrue(foundMatch);
}




//
// Test for memory leaks next.
//
//
// Memory Leak Test - tests for memory leaks
//
// Tests for leaks when going out of scope for:
// * instantiated but unused object
// * instantiated; function stored and executed
// * instantiated; function stored and not executed
// * instantiated; two functions stored and executed
// * instantiated; two functions stored, one executed
// * instantiated; two functions stored, none executed
//
test(A_MemoryLeak) {
  unsigned long mem = freeMemory();
  assertEqual(mem, freeMemory());
  {
    Tasks tasks;
  }
  assertEqual(mem, freeMemory());
  {
    Tasks tasks;
    tasks.schedule(function,0);
    delay(2);
    tasks.dispatch();
  }
  assertEqual(mem, freeMemory());
  {
    Tasks tasks;
    tasks.schedule(function,1000);
  }
  assertEqual(mem, freeMemory());
  {
    Tasks tasks;
    tasks.schedule(function,0);
    tasks.schedule(function,0);
    delay(2);
    tasks.dispatch();
  }
  assertEqual(mem, freeMemory());
  {
    Tasks tasks;
    tasks.schedule(function,1000);
    tasks.schedule(function,0);
    delay(2);
    tasks.dispatch();
  }
  assertEqual(mem, freeMemory());
  {
    Tasks tasks;
    tasks.schedule(function,1000);
    tasks.schedule(function,1000);
    delay(2);
    tasks.dispatch();
  }
  assertEqual(mem, freeMemory());
}




//
// Function Callback Tests
//
// Verifies that functions are called back with the
// proper values.
//

test(CallFunction) {
  functionCalled = false;
  Tasks tasks;
  tasks.schedule(function, 0);
  delay(2);
  tasks.dispatch();
  assertTrue(functionCalled);
}

test(CallBoolFunction) {
  boolValue = false;
  Tasks tasks;
  tasks.schedule(boolFunction, 0, true);
  delay(2);
  tasks.dispatch();
  assertTrue(boolValue);
}

test(CallCharFunction) {
  charValue = 0;
  Tasks tasks;
  tasks.schedule(charFunction, 0, 0xff);
  delay(2);
  tasks.dispatch();
  assertEqual(-1, charValue);
}

test(CallUnsignedCharFunction) {
  unsignedCharValue = 0;
  Tasks tasks;
  tasks.schedule(unsignedCharFunction, 0, 0xff);
  delay(2);
  tasks.dispatch();
  assertEqual(255, unsignedCharValue);
}

test(CallIntFunction) {
  intValue = 0;
  Tasks tasks;
  tasks.schedule(intFunction, 0, 0xffff);
  delay(2);
  tasks.dispatch();
  assertEqual(-1, intValue);
}

test(CallFloatFunction) {
  floatValue = 0;
  Tasks tasks;
  tasks.schedule(floatFunction, 0, 1.01);
  delay(2);
  tasks.dispatch();
  assertEqual(1.01, floatValue);
}

test(CallDoubleFunction) {
  doubleValue = 0;
  Tasks tasks;
  tasks.schedule(doubleFunction, 0, 1.01);
  delay(2);
  tasks.dispatch();
  assertEqual(1.01, doubleValue);
}

test(CallUnsignedIntFunction) {
  unsignedIntValue = 0;
  Tasks tasks;
  tasks.schedule(unsignedIntFunction, 0, 0xffff);
  delay(2);
  tasks.dispatch();
  assertEqual(65535l, unsignedIntValue);
}

test(CallLongFunction) {
  longValue = 0;
  Tasks tasks;
  tasks.schedule(longFunction, 0, 0xffffffff);
  delay(2);
  tasks.dispatch();
  assertEqual(-1, longValue);
}

test(CallUnsignedLongFunction) {
  unsignedLongValue = 0;
  Tasks tasks;
  tasks.schedule(unsignedLongFunction, 0, 0xffffffff);
  delay(2);
  tasks.dispatch();
  assertEqual(-1l, unsignedLongValue);
}

test(CallPointerFunction) {
  pointerValue = NULL;
  Tasks tasks;
  tasks.schedule(pointerFunction, 0, (void*) 0xffff);
  delay(2);
  tasks.dispatch();
  assertTrue((void*) 0xffff == pointerValue);
}




//
// Method Callback Tests
//
// Verifies that methods are called back with the
// proper values.
//

/**
 * SimpleDelayListener - a simple test class
 * 
 * You can verify that callback() was called
 * by providing a value other than NULL for the
 * pointer parameter when calling schedule(), and 
 * verify that this value was stored by calling
 * getPointer(). See test(DelayListener), below.
 */
class SimpleDelayListener : public Callable {
  private:
    void* pointer = NULL;
  public:
    void callback(void* pointer) {
      this->pointer = pointer; 
    }
    void* getPointer() { return pointer; }
};

/**
 * DelayListenerTest - tests Tasks ability to call a Callable
 */
test(DelayListener) {
  SimpleDelayListener testClass;
  // Verify that class is instantiated
  assertTrue(testClass.getPointer() == NULL);
  Tasks tasks;
  // Verify that callback occurs and value passed
  tasks.schedule(&testClass, 0, (void*) 0xffff);
  delay(2);
  tasks.dispatch();
  assertTrue(testClass.getPointer() == (void*) 0xffff);
  // Verify that callback occurs and NULL passed when
  // optional pointer parameter is omitted from schedule().
  tasks.schedule(&testClass, 0);
  delay(2);
  tasks.dispatch();
  assertTrue(testClass.getPointer() == NULL);
}





//
// Delay time test - tests if delay amount works
//
void futureTime(void* time) {
  unsigned long* now = (unsigned long*) time;
  *now = timer0_millis;
}
test(DelayTime) {
  unsigned long later = 0;
  Tasks tasks;
  unsigned long now = timer0_millis;
  tasks.schedule(futureTime, 10ul, (void*) &later);
  while((now + 12ul) > timer0_millis) {
    tasks.dispatch();
  }
  assertTrue(later >= now + 10 && later <= now + 11);
}





//
// Correct order test - are callbacks called in the correct order?
// Also verifies that multiple callbacks can exist simultaneously.
//
// for CorrectOrder tests
unsigned long times[5];
unsigned long now;
void storeMillis(int index) {
  times[index] = timer0_millis - now;
}
test(CorrectOrder) {
  Tasks tasks;
  now = timer0_millis;
  tasks.schedule(storeMillis, 20, 2);
  tasks.schedule(storeMillis, 40, 4);
  tasks.schedule(storeMillis, 10, 1);
  tasks.schedule(storeMillis, 0, 0);
  tasks.schedule(storeMillis, 30, 3);
  while(now + 50 > timer0_millis) tasks.dispatch();
  for(int i=0; i<5; i++) {
    assertTrue(times[i] >= i*10 && times[i] <= (i*10)+1);
  }
}




//
// Loop speed tests - confirms that library still performs OK
//

//
// LoopSpeedEmpty - confirms performance of loop
//
// Confirms performance with no stored functions.
test(LoopSpeedEmpty) {
  Tasks tasks;
  unsigned long count = 0;
  unsigned int start = timer0_millis + 500;
  while(timer0_millis < start);
  start += 1000;
  while(timer0_millis < start) {
    count++;
    tasks.dispatch();
  }
  // The number in parenthesis is from a past run, intended to provide
  // a past result simply for your reference. Feel free to update it
  // after changing the library or test suite.
  Serial.print(count);
  Serial.println(" (182669) calls to loop per second, no pending entries");
  assertMore(count, 150000);
  assertLess(count, 200000);
}

//
// LoopSpeedOneEntry - confirms performance of loop
//
// Confirms performance with one stored function.
test(LoopSpeedOneEntry) {
  Tasks tasks;
  tasks.schedule(function, 1000);  // store one function
  unsigned long count = 0;
  unsigned int start = timer0_millis + 500;
  while(timer0_millis < start);
  start += 1000;
  while(timer0_millis < start) {
    count++;
    tasks.dispatch();
  }
  // The number in parenthesis is from a past run, intended to provide
  // a past result simply for your reference. Feel free to update it
  // after changing the library or test suite.
  Serial.print(count);
  Serial.println(" (165096) calls to loop per second, one pending entry");
  assertMore(count, 150000);
  assertLess(count, 200000);
}

//
// LoopSpeedTwoEntries - confirms performance of loop
//
// Confirms performance with two stored functions.
test(LoopSpeedTwoEntries) {
  Tasks tasks;
  tasks.schedule(function, 2000);  // store one function
  tasks.schedule(function, 2000);  // store another function
  unsigned long count = 0;
  unsigned int start = timer0_millis + 500;
  while(timer0_millis < start);
  start += 1000;
  while(timer0_millis < start) {
    count++;
    tasks.dispatch();
  }
  // The number in parenthesis is from a past run, intended to provide
  // a past result simply for your reference. Feel free to update it
  // after changing the library or test suite.
  Serial.print(count);
  Serial.println(" (148677) calls to loop per second, two pending entries");
  assertMore(count, 125000);
  assertLess(count, 150000);
}

//
// FunctionCallSpeedTest - tests speed at which functions are called
//
// Alternately calls schedule() and loop to see how many functions
// can be added and called in one millisecond.
//
unsigned long testCount = 0;
void countFunction() {
  testCount++;
}
test(FunctionCallSpeedTest) {
  Tasks tasks;
  testCount = 0;
  unsigned int start = timer0_millis + 500;
  while(timer0_millis < start);    // wait for top of millisecond
  start += 1000;
  while(timer0_millis < start) {  // run for one millisecond
    tasks.schedule(countFunction, 0);  // store another function
    tasks.dispatch();                // call it
  }
  // The number in parenthesis is from a past run, intended to provide
  // a past result simply for your reference. Feel free to update it
  // after changing the library or test suite.
  Serial.print(testCount);
  Serial.println(" (31786) functions stored and executed per second (none pending)");
  assertMore(testCount, 30000ul);
  assertLess(testCount, 40000ul);
}

//
// FunctionCallSpeedTest2 - tests speed at which functions are called
//
// Alternately calls schedule() and loop to see how many functions
// can be added and called in one millisecond.
//
// Same as FunctionCallSpeedTest, but preloads one function into
// the tasks instance.
//
test(FunctionCallSpeedTest2) {
  Tasks tasks;
  testCount = 0;
  tasks.schedule(function, 1000); // store one ahead of time
  unsigned int start = timer0_millis + 500;
  while(timer0_millis < start);    // wait for top of millisecond
  start += 1000;
  while(timer0_millis < start) {  // run for one millisecond
    tasks.schedule(countFunction, 0);  // store another function
    tasks.dispatch();                // call it
  }
  // The number in parenthesis is from a past run, intended to provide
  // a past result simply for your reference. Feel free to update it
  // after changing the library or test suite.
  Serial.print(testCount);
  Serial.println(" (30829) functions stored and executed per second (one pending)");
  assertMore(testCount, 30000ul);
  assertLess(testCount, 40000ul);
}




//
// Loop tests - tests "loop" functionality of Tasks
//

//
// Function loop test - tests setLoopFunction(Callback functionCallback)
//
// This tests the Tasks method setLoopFunction(function), which allows the library
// user to specify a function that will be called every time
// dispatch() is run (except when a scheduled task is due to be
// run.) The test stores the loop function to be called using
// setLoopFunction(), and then calls tasks.dispatch() multiple times
// to ensure that the loop function is called every time. It also
// stores a task using schedule() to verify that the scheduled
// task runs instead of the loop task but doesn't prevent the
// loop task from running later.
//
test(setLoopFunction) {
  Tasks tasks;
  tasks.setLoopFunction(function);
  functionCalled = false;
  tasks.dispatch();
  assertTrue(functionCalled);
  functionCalled = false;
  tasks.dispatch();
  functionCalled = false;
  tasks.schedule(boolFunction, 0, true);
  tasks.dispatch();
  assertFalse(functionCalled);  // because boolFunction should have been called instead
  tasks.dispatch();
  assertTrue(functionCalled);
}

//
// Class method loop test - tests setLoopMethodInstance(Loopable class instance)
//
// This tests the Tasks method setLoopMethodInstance(method instance), 
// which allows the library user to specify an instance of
// Loopable whose loop() function will be called every time
// dispatch() is run (except when a scheduled task is due to be
// run.) The test stores the instance whose loop() will be called using
// setLoopMethodInstance(), and then calls tasks.dispatch() multiple times
// to ensure that the loop method is called every time.
//
class LoopableTest : public Loopable {
  private:
    Tasks task;
    unsigned long count = 0;
  protected:
    void loop() { count++; };
  public:
    LoopableTest() {
      task.setLoopMethodInstance(this);
    };
    int getCount() { return count; };
    void dispatch() { task.dispatch(); };
};

test(setLoopMethodInstance) {
  LoopableTest looptest;
  assertEqual(looptest.getCount(), 0);
  looptest.dispatch();
  assertEqual(looptest.getCount(), 1);
  looptest.dispatch();
  assertEqual(looptest.getCount(), 2);
}

// Static class method loop test - tests setLoopFunction with a static class method
class LoopableTest2 {
  private:
    Tasks task;
  protected:
    static void loop() {
      count++;
    };
  public:
    LoopableTest2() {
      task.setLoopFunction(LoopableTest2::loop);
    };
    static unsigned long count;
    void dispatch() { task.dispatch(); };
};

unsigned long LoopableTest2::count = 0;

test(setLoopFunctionWithStaticMethod) {
  LoopableTest2 looptest;
  assertEqual(looptest.count, 0);
  looptest.dispatch();
  assertEqual(looptest.count, 1);
  looptest.dispatch();
  assertEqual(looptest.count, 2);
}




//
// for arduinounit execution
//
void setup() {
  Serial.begin(9600);
  Serial.print(VERSION_STRING);
  Serial.println(VERSION);
}

void loop() {
  Test::run();
}
