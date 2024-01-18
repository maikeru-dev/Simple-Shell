#include "shell.h"
#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int getInput(char *command) {
  if (fgets(command, COMMAND_LENGTH, stdin) == NULL) {
    return INPUT_TERMINATE;
  }

  if (command[0] == '\n') {
    return INPUT_SKIP;
  }

  return INPUT_OK;
}

void quit() {
  printf("Exiting...");
  exit(0);
}

int main() {
  char input[COMMAND_LENGTH];
  int builtInC = 0;
  Command **builtInCommands = malloc(sizeof(Command *) * TOTAL_CMDS);

  produceBuiltIn(builtInCommands, &builtInC);

  while (1) {
    Command *command = malloc(sizeof(Command));
    printf(PROMPT);

    switch (getInput(input)) {
    case INPUT_TERMINATE:
      quit();
      break;
    case INPUT_SKIP:
      continue;
      break;
      // case INPUT_OK:
    }

    createCommand(command, input);
    extendCommand(command, cmdChkExists(builtInCommands, builtInC, command));

    executeCommand(command);

    freeCommand(command);
  }

  freeCommands(builtInCommands, builtInC);
  return 0;
}
