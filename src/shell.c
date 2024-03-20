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

  // TODO: Add a way to detect whether input is out of bounds, special note:
  // if '\n' is NOT present, that means it must have been cut off by input
  // limit denoted "COMMAND_LENGTH".
  //
  // char *newLine;
  // if ((newLine = strtok(command, "\n")) != NULL) {
  //   *newLine = '\0';
  // }

  return INPUT_OK;
}

void quit() {
  printf("Exiting...\n");
  exit(0);
}

int main() {
  int loop = 1;
  char input[COMMAND_LENGTH];
  int builtInC = 0;
  int aliasesC = 0;
  int historyC = 0;
  Command **builtInCommands = malloc(sizeof(Command *) * TOTAL_CMDS);
  Command **aliases = calloc(ALIASES_LENGTH, sizeof(Command *));
  Command **history = calloc(HISTORY_LENGTH, sizeof(Command *));
  char *originalPath = getenv("PATH");

  char dir[100];
  getcwd(dir,100);

  chdir(dir);
  produceBuiltIn(builtInCommands, &builtInC);

  while (loop) {
    /* Be very careful using this variable, any references used to any of its
     * mallocd/const members must be duplicated, if you do not duplicate them,
     * expect unexpected runtime bugs. */
    Command command;
    
    getcwd(dir,100);
    printf("%s\n%s", dir, PROMPT);

    switch (getInput(input)) {
    case INPUT_TERMINATE:
      loop = 0;
      printf("\n");
    case INPUT_SKIP: // remember INPUT_TERMINATE flows into INPUT_SKIP.
      continue;
    }

    createCommand(&command, input);

    // If found a match, apply parent properties to child's, else do nothing.
    extendCommand(&command, cmdChkExists(builtInCommands, builtInC, &command));
    
    // If found an alias, apply parent
    extendCommand(&command, cmdChkExists(aliases, aliasesC, &command));

    // Executes any command of any type.
    executeCommand(&command, aliases, &aliasesC, history, &historyC, builtInCommands, &builtInC);

    // Free any malloc'd data.
    freeCommand(&command);
  }

  setenv("PATH", originalPath, 1); // Replace PATH with the saved one
  freeCommands(builtInCommands, builtInC);
  quit();
  return 0;
}