# Arduino Async Framework

This library FreeRTOS extensions.

The code is simple module programming.


## Functionality

 
The async functions available in the library include

```c
invoke();
action();          // sync execute
fork();            // parallel execute
cancel();          // job cancel
pause();           // job pause
resume();          // job resume
report();          // report progress
wait();            // sync execute
```

## How to use

create module

header file
```c
class example01 {
   private:
    queue que;
    static void func(job* parameter);  // function
  public:
    void init();
    job* funcAsync();                  // async function
};

```

c source
```c
void example01::func(job* parameter) {
  Serial.println("hello world");
}

void example01::init() {
  Serial.begin(19200);
  taskStart(&que, "example01", 4096, 3, 1);
}

void example01::funcAsync() {
  return que.invoke(this, func, NULL, NULL);
}

```

ino source
```c
static example01 ex;

void setup() {
  ex.init();
}

void loop() {
  ex.funcAsync()->wait();
}
```

let's enjoy!
