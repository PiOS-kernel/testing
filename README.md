# πOS repo for testing
This repository includes kernel code as a submodule, and it contains code to run integration tests.

## Writing an integration test

Each integration test is organized as a setup function, and a set of tasks that interact between each other and that communicate the result of the test once it is completed.

A **test_runner** task is responsible for executing tests. Before running a test it calls its setup function, which instantiates the data necessary for the test, creates the tasks and yields control to them until the test is finished. The general scheme for a setup function is the following:

```
typedef struct SharedData {...} SharedData;
void producer_task(SharedData* data);
void consumer_task(SharedData* data);

void test_producer_consumer() {
    // INSTANTIATION OF DATA NECESSARY FOR THE TEST
    SharedData data = {0, true};
    
    // INITIALIZATION OF THE TEST TASKS
    create_task((void(*)(void*)) producer_task, (void*)&data, 0);
    create_task((void(*)(void*)) consumer_task, (void*)&data, 0);

    // THE FUNCTION SHOULD NOT RETURN UNTIL THE TEST HAS BEEN COMPLETED,
    // BECAUSE TEST RELATED DATA HAS BEEN INSTANTIATED ONTO THE STACK, WHICH
    // WOULD BE ERASED IF THE FUNCTION RETURNS.
    while(!test_completed)
        PendSVTrigger();
}
```

The tasks that run the test are responsible of communicating when the test is completed and whether it was successfull or not. Also, after compleating their
job they should terminate by calling `exit()` syscall. Refer to [this test](testing/scheduling.c) to check out the correct behavior of test tasks.

## Running tests in Qemu
1. `make test` to run the integration tests. When issuing this command the executable will be recompiled from scratch.
2. If you need to debug the code using GDB, issue `make gdb`. Also this command recompiles all the suorce files.
3. When running the tests with GDB, in another terminal window type the following command `gdb-multiarch -q executable_file.elf` to open the gdb console.\
Then on gdb console type `target remote :3333` to connect to qemu via tcp.\
To add a breakpoint, use the command `break _symbol` where `_symbol` represents a valid memory symbol.\
To let the execution begin, type `continue`; this will execute the program until the following breakpoint. Otherwise, use the command `next` to proceed to the following code line.
4. when the program terminates:
- in qemu terminal press `control+a` and then `x`
- close gdb typing `quit`

If you want to just run qemu (without enabling debug session), type `make run`

### How to use VSCode Extension
In ordet to debug using VSCode GUI, you need to install Native Debug extension (https://marketplace.visualstudio.com/items?itemName=webfreak.debug). Make sure the `.vscode/launch.json` file is properly configured.

```
"configurations": [
        {
            "name": "GDB",
            "type": "gdb",
            "request": "attach",
            "cwd": "${workspaceRoot}",
            "executable": "./debug/final.elf",
            "target": ":3333",
            "remote": true,
            "gdbpath" : "/usr/bin/gdb-multiarch",
            "stopAtConnect": true
        }
    ]
```

1. set breakpoints throughout your code
2. launch simulation with qemu typing `make gdb`
3. start a debug session using VSCode GUI (left panel -> Run and Debug). The default breakpoint is set to `ResetHandler` entry.
4. use VSCode GUI to debug your code
5. when the program terminates, disconnect the gdb debug session (red icon of VSCode debugger GUI) and in qemu terminal press `control+a` and then `x`


## Running tests on the MSP432 board

1. Create a brand new ccs project.
2. Open a shell inside the ccs project directory:
```
$ git clone https://github.com/PiOS-kernel/testing.git
$ git checkout msp-port
$ git submodule update --init --recursive --remote
```
3. Exclude from the build:
    * pios-kernel/kernel/port/ccs
    * pios-kernel/testing
