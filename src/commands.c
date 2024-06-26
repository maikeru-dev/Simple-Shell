#include "commands.h"
#include "constants.h"
#include "shell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int invoke_fn(int argc, char **argv, Command **aliases, int *aliasC,
              Command **history, int *historyC, Command **builtInCommands,
              int *builtInC) {
  if (argc == 1) {
    for (int i = 0; i < *historyC; i++) {
      if (history[i] != NULL) {
        printf("\n%d: %s", i + 1, history[i]->tokens[0]);
      } else {
        printf("\n%d: (null)", i + 1);
      }
    }
    printf("\n");
  }

  return 0;
}

int exit_fn(int argc, char **argv, Command **aliases, int *aliasC,
            Command **history, int *historyC, Command **builtInCommands,
            int *builtInC) {
  (void)(argc); // supress unused warning
  (void)(argv);
  quit();
  return 0;
}

int chdir_fn(int argc, char **argv, Command **aliases, int *aliasC,
             Command **history, int *historyC, Command **builtInCommands,
             int *builtInC) {
  argc--;
  if (argc == 0) {
    chdir(getenv("HOME"));
    return 1;
  }

  if (argc > 1) {
    perror("cd: Too many arguments\n");
    return 1;
  }

  if (argc < 1) {
    perror("cd: No arguments provided\n");
    return 1;
  }

  if (chdir(argv[1]) != 0) {
    fprintf(stderr, "cd: %s\n", strerror(errno));
    return 1;
  }

  return 0;
}

int getpath_fn(int argc, char **argv, Command **aliases, int *aliasC,
               Command **history, int *historyC, Command **builtInCommands,
               int *builtInC) {
  printf("%s\n", getenv("PATH"));
  return 0;
}

int setpath_fn(int argc, char **argv, Command **aliases, int *aliasC,
               Command **history, int *historyC, Command **builtInCommands,
               int *builtInC) {
  argc--;
  if (argc > 1) {
    printf("setpath: Too many arguments\n");
    return 1;
  }

  if (argc < 1) {
    printf("setpath: No arguments provided\n");
    return 1;
  }

  setenv("PATH", argv[1], 1);

  return 0;
}

int pasteOffsetArguments(Command *child, Command *parent, int offset) {
  char **argvC = calloc(child->length + parent->length, sizeof(char *));
  for (int i = 0; i < child->length; i++) {
    argvC[i] = child->tokens[i];
  }

  for (int i = child->length; i < child->length + parent->length - 1; i++) {
    argvC[i] = strdup(parent->tokens[i - child->length + 1]);
  }

  free(child->tokens);
  child->tokens = argvC;
  child->length += parent->length - 1;
  child->tokens[child->length] = NULL;

  // printf("new length: %d\nnew array: ", cmdA->length);
  //
  // for (int i = 0; i < cmdA->length; i++) {
  //   printf("%s, ", cmdA->tokens[i]);
  // }
  // printf("\n");
  return 0;
}

int executeCommand(Command *command, Command **aliases, int *aliasC,
                   Command **history, int *historyC, Command **builtInCommands,
                   int *builtInC) {
  extendCommand(command, cmdChkExists(aliases, *aliasC, command));

  switch (command->type) {
  case BUILTIN:
    fflush(stdout);
    command->fn(command->length, command->tokens, aliases, aliasC, history,
                historyC, builtInCommands, builtInC);
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

  case ALIAS_COMMAND:
    pasteOffsetArguments(command->alias, command, 0);
    executeCommand(command->alias, aliases, aliasC, history, historyC,
                   builtInCommands, builtInC);
    break;
  }

  return 0;
}

int freeCommand(Command *command) {
  for (int i = 0; i < command->length; i++) {
    free(command->tokens[i]);
  }

  free(command->fullCommand);
  free(command->tokens);
  if (command->type == ALIAS_COMMAND) {
    freeCommand(command->alias);
  }

  return 0;
}

int freeCommands(Command **commands, int argc) {
  for (int i = 0; i < argc; i++) {
    if (commands[i] == NULL)
      continue;

    freeCommand(commands[i]);
    free(commands[i]);
  }

  free(commands);
  return 0;
}

Command *copyCommand(Command *child, Command *parent) {
  child->tokens = calloc(parent->length, sizeof(char *));
  child->length = parent->length;
  child->type = parent->type;
  child->fn = parent->fn;
  child->fullCommand = strdup(parent->fullCommand);

  for (int i = 0; i < parent->length; i++) {
    child->tokens[i] = strdup(parent->tokens[i]);
  }

  return child;
}

int extendCommand(Command *child, Command *parent) {
  if (child == parent)
    return 0;

  if (parent->type == ALIAS_COMMAND) {
    child->alias = malloc(sizeof(Command));
    copyCommand(child->alias, parent->alias);
  }

  child->type = parent->type;
  child->fn = parent->fn;

  return 0;
}

int createCommand(Command *command, char *input) {
  char **argv = calloc(TOKENS_LENGTH, sizeof(char *));
  int argc;

  _tokenise(argv, &argc, input);

  command->fullCommand = strdup(input);
  command->type = EXECUTE;
  command->tokens = argv;
  command->length = argc;

  return 0;
}

Command *cmdChkExists(Command **commands, int argc, Command *command) {
  for (int i = 0; i < argc; i++) {
    // printf("%d %d ", commands, command);
    // printf("thingy: %s", commands[i]->tokens[0]);
    // fflush(stdout);
    if (strcmp(commands[i]->tokens[0], command->tokens[0]) == 0) {
      // printf("%s %s", commands[i]->tokens[0], command->tokens[0]);
      return commands[i];
    }
  }
  return command;
}

int produceBuiltIn(Command **commands, int *argc) {
  *argc = TOTAL_CMDS;

  commands[0] = _createBuiltInCommand("exit", &exit_fn);
  commands[1] = _createBuiltInCommand("getpath", &getpath_fn);
  commands[2] = _createBuiltInCommand("setpath", &setpath_fn);
  commands[3] = _createBuiltInCommand("cd", &chdir_fn);
  commands[4] = _createBuiltInCommand("alias", &alias_fn);
  commands[5] = _createBuiltInCommand("history", &invoke_fn);
  return 0;
}

Command *_createBuiltInCommand(char *input, int (*fn)(int, char **, Command **,
                                                      int *, Command **, int *,
                                                      Command **, int *)) {
  int argc = 0;
  char **argv = malloc(sizeof(char *));
  Command *command = malloc(sizeof(Command));

  //_tokenise(argv, &argc, input);
  argv[0] = input;
  command->tokens = argv;
  command->fullCommand = strdup(input);
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
int _tokenise(char **argv, int *argc, char *input) {
  *argc = 0;
  char *dummy = strtok(input, TOKENISER);

  do {
    argv[*argc] = strdup(dummy);
    dummy = strtok(NULL, TOKENISER);
    (*argc)++;
  } while (dummy != NULL);

  return 0;
}

// Function to handle alias command
int alias_fn(int argc, char **argv, Command **aliases, int *aliasC,
             Command **history, int *historyC, Command **builtInCommands,
             int *builtInC) {
  if (argc == 1) {
    int head = 0;

    while (head < ALIASES_LENGTH) {
      if (aliases[head] != NULL) {
        printf("\n%s: %s", aliases[head]->tokens[0],
               aliases[head]->alias->tokens[0]);
      }
      head += 1;
    }

    printf("\n");
    return 0;
  }

  if (argc < 3) {
    printf("Usage: alias <name> <command>\n");
    return 1;
  }

  Command *command = malloc(sizeof(Command));
  createCommand(command, strdup(argv[2])); // TODO: make this support arguments
  // Search for an existing alias with the same name
  extendCommand(command, cmdChkExists(builtInCommands, *builtInC, command));

  // create new alias using free space
  for (int i = 0; i < ALIASES_LENGTH; i++) {
    if (aliases[i] == NULL) {
      Command *alias = malloc(sizeof(Command));
      createCommand(alias, strdup(argv[1]));
      aliases[i] = alias;
      aliases[i]->type = ALIAS_COMMAND;
      aliases[i]->alias = command;
      (*aliasC)++;
      printf("Alias '%s' created.\n", argv[1]);
      return 0;
    }
  }

  printf("No more aliases can be set. Maximum limit reached.\n");
  return 1;
}

// Function to handle unalias command
int unalias_fn(int argc, char **argv, Command **aliases, int *aliasC,
               Command **history, int *historyC, Command **builtInCommands,
               int *builtInC) {
  if (argc < 2) {
    // printf("Usage: unalias <name>\n");
    return 1;
  }

  for (int i = 0; i < TOTAL_CMDS; i++) {
  }

  printf("Alias '%s' not found.\n", argv[1]);
  return 1;
}

int saveAliases(char *filepath, Command **commands, int argc) {
  FILE *file = fopen(filepath, "w");
  if (file == NULL) {
    return 1;
  }

  for (int i = 0; i < argc; i++) {
    fputs(commands[argc]->fullCommand, file);
    fputs("\n", file);
  }

  fclose(file);
  return 0;
}

int saveCommands(char *filepath, Command **commands, int argc) {
  FILE *file = fopen(filepath, "w");
  if (file == NULL) {
    return 1;
  }

  for (int i = 0; i < argc; i++) {
    fputs(commands[i]->fullCommand, file);
    fputs("\n", file);
  }

  fclose(file);
  return 0;
}
