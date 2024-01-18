#include "commands.h"
#include "constants.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int exit_fn(int argc, char **argv) {
  (void)(argc); // supress unused warning
  (void)(argv);

  quit();
  return 0;
}

int chdir_fn(int argc, char **argv) {}

int getpath_fn(int argc, char **argv) {
  printf("%s\n", getenv("PATH"));
  return 0;
}

int setpath_fn(int argc, char **argv) {
  if (argc > 1) {
    printf("setpath: Too many arguments");
    return 1;
  }
  if (argc < 1) {
    printf("setpath: No arguments provided");
  }

  setenv("PATH", argv[0], 1);

  return 0;
}

int executeCommand(Command *command) {
  switch (command->type) {

  case BUILTIN:
    command->fn(command->length, command->tokens);
    break;
  case EXECUTE:
    // remember that when aliases occur, you must compile the arguments in case
    // they are included.
    {
      pid_t pid = fork();
      if (pid < 0) {
        perror("No forking allowed apparently!");
        return 1;
      }

      if (pid == 0) {
        char *cmdCopy =
            strdup(command->tokens[0]); // feel free to improve this name. this
                                        // var is used to send an error back
                                        // when execvp fails.
        execvp(cmdCopy, command->tokens); // no need to check if it fails. it
                                          // only comes back when it fails, as
                                          // it exits on success.
        perror(cmdCopy);
        exit(0);
      }

      wait(NULL);
    }
    break;
  case ALIASES:
    break;
  }

  return 0;
}

int freeCommand(Command *command) {
  free(command->tokens);
  free(command);
  command = 0;
  return 0;
}

int freeCommands(Command **commands, int argc) {
  for (int i = 0; i < argc; i++) {
    freeCommand(commands[i]);
  }
  free(commands);

  return 0;
}
int extendCommand(Command *child, Command *parent) {
  child->type = parent->type;
  child->fn = parent->fn;

  return 0;
}

int createCommand(Command *command, char *input) {
  char **argv = malloc(sizeof(char) * TOKENS_LENGTH);
  int argc;

  _tokenise(argv, argc, input);

  command->type = EXECUTE;
  command->tokens = argv;
  command->length = argc;

  return 0;
}

/*
 * Compares whether the first token is the same within another command from a
 * list.
 * */
Command *cmdChkExists(Command **commands, int argc, Command *command) {
  for (int i = 0; i < argc; i++) {
    // printf("%d %d ", commands, command);
    // fflush(stdout);
    if (strcmp(commands[i]->tokens[0], command->tokens[0]) == 0) {

      // printf("%s %s", commands[i]->tokens[0], command->tokens[0]);
      return commands[i];
    }
  }

  return command;
}

// Note: command[] should have a length of TOTAL_CMDS (found in "constants.h")
int produceBuiltIn(Command **commands, int *argc) {
  *argc = TOTAL_CMDS;

  commands[0] = _createBuiltInCommand("exit", &exit_fn);
  commands[1] = _createBuiltInCommand("getpath", &getpath_fn);
  return 0;
}

Command *_createBuiltInCommand(char *input, int (*fn)(int, char **)) {
  int argc = 0;
  char **argv = malloc(sizeof(char) * TOKENS_LENGTH);
  Command *command = malloc(sizeof(Command));

  _tokenise(argv, argc, input);
  command->type = BUILTIN;
  command->fn = fn;
  command->length = argc; // we don't actually need to care about the length,
                          // all we want is the command name
  command->tokens = argv;

  return command;
}

// TODO: Make sure the string tokeniser fn uses redirection and operators as
// well as spaces and new line characters. Redirection and operators will not be
// supported (in this branch..?)
int _tokenise(char **argv, int argc, char *input) {
  argc = 0;
  argv[argc] = strtok(input, " \n");
  do {
    argc++;
    argv[argc] = strtok(NULL, " \n");
  } while (argv[argc] != NULL);
  return 0;
}
