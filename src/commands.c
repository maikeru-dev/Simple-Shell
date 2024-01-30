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

#define TOTAL_CMDS 4

typedef struct Builtin Builtin;
struct Builtin {
  char *name;
  int (*fn)(int argc, char **argv);
};

typedef struct Parsed Parsed;
struct Parsed {
  int argc;
  char **argv;
};

int exit_fn(int argc, char **argv) {
  (void)(argc); // supress unused warning
  (void)(argv);

  quit();
  return 0;
}

int chdir_fn(int argc, char **argv) {
  argc--;
  if (argc > 1) {
    perror("cd: Too many arguments\n");
    return 1;
  }
  if (argc < 1) {
    perror("cd: No arguments provided\n");
    return 1;
  }

  if (chdir(argv[1]) == -1) {
    fprintf(stderr, "cd: %s\n", strerror(errno));
    return 1;
  }

  return 0;
}

int getpath_fn(int argc, char **argv) {
  (void)(argc); // supress unused warning
  (void)(argv);
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

// TODO: Make sure the string tokeniser fn uses redirection and operators as
// well as spaces and new line characters. Redirection and operators will not be
// supported (in this branch..?)
Parsed tokenise(char *input) {
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

// evaluates input and returns exit code
void eval_command(char* input) {
  Builtin builtins[TOTAL_CMDS] = {
    { "exit", &exit_fn },
    {"getpath", &getpath_fn},
    {"setpath", &setpath_fn},
    {"cd", &chdir_fn},
  };

  Parsed p = tokenise(input);
  int (*builtin)(int, char **) = getBuiltin(builtins, (p.argv)[0]);
  if (builtin != NULL) {
    builtin(p.argc, p.argv);
    return;
  }

  pid_t pid = fork();
  if (pid < 0) {
    perror("No forking allowed apparently!");
    exit(1);
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
