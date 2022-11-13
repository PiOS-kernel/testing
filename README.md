# simulation-debugging
Default project for debug and simulation using gdb and qemu

### How to simulate and debug
1. launch compilation with `make` command
2. launch simulation with qemu typing `make gdb`
3. in another terminal windows, type the following command `gdb-multiarch -q executable_file.elf` to open the gdb console.\
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

