#include "shell.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int getInput(char *command) {
  if (fgets(command, COMMAND_LENGTH, stdin) == NULL) {
    return INPUT_TERMINATE;
  }

  if (command[0] == '\n') {
    return INPUT_SKIP;
  }

  // char *newLine;
  // if ((newLine = strtok(command, "\n")) != NULL) {
  //   *newLine = '\0';
  // }

  return INPUT_OK;
}

void quit() {
  printf("Exiting...");
  exit(0);
}

int main() {
  int loop = 1;
  char input[COMMAND_LENGTH];
  int builtInC = 0;
  Command **builtInCommands = malloc(sizeof(Command *) * TOTAL_CMDS);
  char *originalPath = getenv("PATH");

  chdir(getenv("HOME"));
  produceBuiltIn(builtInCommands, &builtInC);

  while (loop) {
    Command *command = malloc(sizeof(Command));
    printf(PROMPT);

    switch (getInput(input)) {
    case INPUT_TERMINATE:
      loop = 0;
    case INPUT_SKIP:
      continue;
      // case INPUT_OK:
    }

    createCommand(command, input);
    extendCommand(command, cmdChkExists(builtInCommands, builtInC, command));

    executeCommand(command);

    freeCommand(command);
  }

  setenv("PATH", originalPath, 1); // Replace PATH with the saved one
  freeCommands(builtInCommands, builtInC);
  quit();
  return 0;
}
