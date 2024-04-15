# quickrun
A script I wrote to compile and run C/C++ programs with one command. 

## How to Use
You can add a comment to the top of the file containing the command to compile the file. 
You can then pass the file to quickrun to compile and run it. Arguments are passed to 
the compiled program.

### Example
```c
// gcc %I% -o %O%
#include <stdio.h>

int main() {
  printf("Hello World\n");
  return 0;
}
```
```sh
$ qrun test.c
Hello World
```

## Compiling/Installing
```sh
$ git clone https://github.com/berserkware/quickrun
$ cd quickrun
$ make
$ sudo cp ./bin/qrun /usr/bin
```

## Preprocessor Options
 - ```%O%``` : Gets replaced with a automatic output file (usually in /tmp).
   You need to make the compiler output this, because it is the file that
   qrun executes after running your command.
 - ```%I%``` : Get replaced with the name of the file. Allows you to rename the
   file without having to change the qrun command.
