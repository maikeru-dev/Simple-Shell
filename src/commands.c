#include "commands.h"
#include "constants.h"
#include "shell.h"
#include <stdlib.h>
#include <string.h>

int exit_fn(int argc, char **argv) {
  (void)(argc); // supress unused warning
  (void)(argv);

  quit();
  return 0;
}

int executeCommand(Command *command) {
  switch (command->type) {

  case BUILTIN:
    command->fn(command->length, *command->tokens);
  case EXECUTE:
  case ALIASES:
    break;
  }

  return 0;
}

int extendCommand(Command *child, Command *parent) {
  child->type = parent->type;
  child->fn = parent->fn;

  return 0;
}

int createCommand(Command *command, char *input) {
  command = malloc(sizeof(Command));
  char **argv = malloc(sizeof(char *) * TOKENS_LENGTH);
  int argc;

  _tokenise(argv, argc, input);

  command->tokens = &argv;
  command->length = argc;

  return 0;
}

/*
 * Compares whether the first token is the same within another command from a
 * list.
 * */
Command *cmdChkExists(Command **commands, int argc, Command *command) {
  for (int i = 0; i < argc; i++) {
    if (strcmp(*commands[i]->tokens[0], *command->tokens[0])) {
      return &command[i];
    }
  }

  return 0;
}

// Note: command[] should have a length of TOTAL_CMDS (found in "constants.h")
int produceBuiltIn(Command **commands, int argc) {
  commands = malloc(sizeof(Command *) * TOTAL_CMDS);
  argc = TOTAL_CMDS;

  commands[0] = _createBuiltInCommand("exit", &exit_fn);
  return 0;
}

Command *_createBuiltInCommand(char *input, int (*fn)(int, char **)) {
  int argc = 0;
  char **argv = malloc(sizeof(char *) * TOKENS_LENGTH);
  Command *command = malloc(sizeof(Command));

  _tokenise(argv, argc, input);
  command->fn = fn;
  command->length = argc; // we don't actually need to care about the length,
                          // all we want is the command name
  command->tokens = &argv;

  return command;
}

int _tokenise(char **argv, int argc, char *input) {
  argc = 0;
  argv[argc] = strtok(input, " \n");
  do {
    argc++;
    argv[argc] = strtok(NULL, " \n");
  } while (argv[argc] != NULL);
  return 0;
}
