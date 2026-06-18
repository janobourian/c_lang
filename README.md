# C lang

```bash
gcc main.c -o main
gcc -R main.c -o main.i
```

## C Macros

```c
__LINE__
__FILE__
__DATE__
__TIME__
__STDC__
__FUNCTION__
__COUNTER__
```

## Pointers

* Pointer: value
* Content: address

```c
int a = 10;
int *p = &a; // p is a pointer to a
// p = a is equivalent
```

## System calls

* open()
* write()
* close()
* remove()
* read()
* lseek()

## Process

* `ps -A`
* fork(): create new process child
* execv(): execute another program inside a program
* exit()
* wait(): wait for the end of the current execution

## Threads

* `gcc main.c -lpthread -o main`
* pthread_create()
* pthread_join()
* pthread_exit()

## Synchronization

* signal()
* wait()