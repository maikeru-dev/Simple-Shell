#ifndef COMMANDS_H_
#define COMMANDS_H_

#define TOTAL_CMDS 1

typedef struct {
  char* name;
  int (*fn)(int, char**);
} Command;

Command commands[TOTAL_CMDS];


#endif // COMMANDS_H_
