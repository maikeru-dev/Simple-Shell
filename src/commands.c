#include "commands.h"
#include "constants.h"
#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>

int exit_fn(int argc, char **argv) {
  (void)(argc); // supress unused warning
  (void)(argv);

  quit();
  return 0;
}

int chdir_fn(int argc, char **argv) {
  argc--;
  if (argc > 1) {
    printf("cd: Too many arguments\n");
    return 1;
  }
  if (argc < 1) {
    printf("cd: No arguments provided\n");
    return 1;
  }

  if (chdir(argv[1]) == -1) {
    fprintf(stderr, "cd: %s\n", strerror(errno));
    return 1;
  }

  return 0;
}

int getpath_fn(int argc, char **argv) {
  printf("%s\n", getenv("PATH"));
  return 0;
}

int setpath_fn(int argc, char **argv) {
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

/*
 * Compares whether the first token is the same within another command from a
 * list.
 * */
int (*getBuiltin(Builtin *builtins, char *name)) (int, char **){
  for (int i = 0; i < TOTAL_CMDS; i++) {
    if (strcmp(builtins[i].name, name) == 0) {

      return builtins[i].fn;
    }
  }

  return NULL;
}

// Note: command[] should have a length of TOTAL_CMDS (found in "constants.h")
Builtin *produceBuiltins()  {
  Builtin* builtins = malloc(TOTAL_CMDS * sizeof(Builtin));
  builtins[0] = (Builtin) { "exit", &exit_fn };
  builtins[1] = (Builtin) {"getpath", &getpath_fn};
  builtins[2] = (Builtin) {"setpath", &setpath_fn};
  builtins[3] = (Builtin) {"cd", &chdir_fn};
  return builtins;
}

// TODO: Make sure the string tokeniser fn uses redirection and operators as
// well as spaces and new line characters. Redirection and operators will not be
// supported (in this branch..?)
Parsed _tokenise(char *input) {
  Parsed r;
  r.argc = 0;
  r.argv = malloc(sizeof(char*) * TOKENS_LENGTH);
  r.argv[r.argc] = strtok(input, " \n");
  do {
    (r.argc)++;
    r.argv[r.argc] = strtok(NULL, " \n");;
  } while (r.argv[r.argc] != NULL);
  return r;
}
