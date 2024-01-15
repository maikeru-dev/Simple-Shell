#include "commands.h"
#include "shell.h"
#include <stdlib.h>

int exit_fn(int argc, char** argv) {
  (void)(argc); //supress unused warning
  (void)(argv);

  quit();
  return 0;
}

Command commands[] = {
  { "exit", &exit_fn }
};
