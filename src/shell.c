#include "shell.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getInput(char *command) {
  if (fgets(command, COMMAND_LENGTH, stdin) == NULL) {
    return INPUT_TERMINATE;
  }

  if (command[0] == '\n' ) {
    return INPUT_SKIP;
  }

  return INPUT_OK;
}

int runCommand(char *command) {
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



void quit() {
  printf("Exiting...");
  exit(0);
}

int main() {
  char command[COMMAND_LENGTH];
  while (1) {
    printf(PROMPT);

    switch (getInput(command)) {
    case INPUT_TERMINATE:
      quit();
      break;
    case INPUT_SKIP:
      continue;
      break;
    case INPUT_OK:
      runCommand(command);
    }
  }
  return 0;
}
