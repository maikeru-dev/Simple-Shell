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
  char input[COMMAND_LENGTH];
  char *originalPath = getenv("PATH");

  chdir(getenv("HOME"));


  while (1) {
    printf(PROMPT);


    switch (getInput(input)) {
    case INPUT_TERMINATE:
      goto escape;
    case INPUT_SKIP:
      continue;
      // case INPUT_OK:
    }

   eval_command(input);

  }
  escape:

  setenv("PATH", originalPath, 1); // Replace PATH with the saved one
  quit();
  return 0;
}
