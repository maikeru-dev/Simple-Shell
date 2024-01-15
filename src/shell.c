#include "shell.h"
#include <stdio.h>
#include <stdlib.h>

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
  printf("Exiting... ");
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
      break;
    }
  }
}
