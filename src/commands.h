#include "constants.h"

#ifndef COMMANDS_H_
#define COMMANDS_H_

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


// Built-In commands -- private
int exit_fn(int argc, char **argv);
int chdir_fn(int argc, char **argv);
int getpath_fn(int argc, char **argv);
int setpath_fn(int argc, char **argv);
int invoke_fn(int argc, char **argv);
int alias_fn(int argc, char **argv);
int unalias_fn(int argc, char **argv);
// End

Builtin * produceBuiltins();
int (*getBuiltin(Builtin *builtin, char *toks)) (int, char **);

Parsed _tokenise(char *input);

#endif // COMMANDS_H_
