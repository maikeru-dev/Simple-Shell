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
  Builtin* builtins = produceBuiltins();
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

    Parsed p = _tokenise(input);
    int (*builtin)(int, char **) = getBuiltin(builtins, (p.argv)[0]);
    if (builtin != NULL) {
      builtin(p.argc, p.argv);
      continue;
    }

    pid_t pid = fork();
      if (pid < 0) {
        perror("No forking allowed apparently!");
        return 1;
      }

      if (pid == 0) {
                                        // var is used to send an error back
                                        // when execvp fails.
        execvp(p.argv[0], p.argv); // no need to check if it fails. it
                                          // only comes back when it fails, as
                                          // it exits on success.
        perror(p.argv[0]);
        exit(0);
      }

      wait(NULL);

  }
  escape:

  setenv("PATH", originalPath, 1); // Replace PATH with the saved one
  free(builtins);
  quit();
  return 0;
}
