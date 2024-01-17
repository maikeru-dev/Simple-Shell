#include "constants.h"

#ifndef COMMANDS_H_
#define COMMANDS_H_

enum CommandType { BUILTIN, EXECUTE, ALIASES };

struct Command {
  char ***tokens;
  int length;
  enum CommandType type;
  union {
    int (*fn)(int argc, char **argv);
    struct Command *alias;
  };
};

typedef struct Command Command;

/* BUILTIN COMMANDS */
int exit_fn(int argc, char **argv);

// BUILTIN COMMANDS END

int produceBuiltIn(Command **commands, int argc);
int extendCommand(Command *child, Command *parent);
Command *cmdChkExists(Command **commands, int argc, Command *command);
int compareCommand(Command *commandA, Command *commandB);
int executeCommand(Command *command);
int createCommand(Command *command, char *input);

Command *_createBuiltInCommand(char *input, int (*fn)(int argc, char **argv));
int _tokenise(char **argv, int argc, char *input);

#endif // COMMANDS_H_
