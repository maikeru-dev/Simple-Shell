#include "commands.h"
#include "shell.h"
#include <stdlib.h>
#include <string.h>

int exit_fn(int argc, char** argv) {
  (void)(argc); //supress unused warning
  (void)(argv);

  quit();
  return 0;
}



int runCommand(char *command) {
  Command commands[] = {
    { "exit", &exit_fn }
  };
  // TODO: figure out what seperators we should use
  char* cmd = strtok(command, " \n");

  for (int i = 0; i < TOTAL_CMDS; i++) {
    if (strcmp(commands[i].name, cmd) == 0) {
      char* argv[256];
      int argc = 0;
      argv[argc] = cmd;
      do {
        argc++;
        argv[argc] = strtok(NULL, " \n");
      } while(argv[argc] != NULL);
      commands[i].fn(argc, argv);
    }
  }

  return 0;
}
