#include "constants.h"


#ifndef COMMANDS_H_
#define COMMANDS_H_

struct Command {
    enum {
        BUILTIN,
        EXECUTE,
        ALIAS_COMMAND
    } type;
    int length;
    char **tokens;
    int (*fn)(int, char **, struct Command**, int*, struct Command**, int*, struct Command**, int*);
    struct Command *alias;
};

typedef struct Command Command;


// Built-In commands -- private
int exit_fn(int argc, char **argv, Command** aliases, int* aliasC, Command** history, int* historyC, Command **builtInCommands, int* builtInC);
int chdir_fn(int argc, char **argv, Command** aliases, int* aliasC, Command** history, int* historyC, Command **builtInCommands, int* builtInC);
int getpath_fn(int argc, char **argv, Command** aliases, int* aliasC, Command** history, int* historyC, Command **builtInCommands, int* builtInC);
int setpath_fn(int argc, char **argv, Command** aliases, int* aliasC, Command** history, int* historyC, Command **builtInCommands, int* builtInC);
int invoke_fn(int argc, char **argv, Command** aliases, int* aliasC, Command** history, int* historyC, Command **builtInCommands, int* builtInC);
int alias_fn(int argc, char **argv, Command** aliases, int* aliasC, Command** history, int* historyC, Command **builtInCommands, int* builtInC);
int unalias_fn(int argc, char **argv, Command** aliases, int* aliasC, Command** history, int* historyC, Command **builtInCommands, int* builtInC);
int printAliases_fn(int argc, char **argv, Command** aliases, int* aliasC, Command** history, int* historyC, Command **builtInCommands, int* builtInC);
// end

// Produces an array of size argc, which contains all currently (hardcode)
// enabled built-in
int produceBuiltIn(Command **commands, int *argc);

// Copies parents qualities (type, fn, alias) to child, does nothing if child ==
// parent.
int extendCommand(Command *child, Command *parent);

// Compares command's first token to all commands first token, given the array's
// size by argc
Command *cmdChkExists(Command **commands, int argc, Command *command);

// Executes any command for any type (alias, builtin, execute)
int executeCommand(Command *command, Command** aliases, int* aliasC, Command** history, int* historyC, Command **builtInCommands, int* builtInC);

/* Note: This does not create a dynamically allocated pointer to command, only
   edits it. Computes a token arr and its length, and attaches it to the command
*/
int createCommand(Command *command, char *input);

/* Note: This command expects the array to be on the heap, do not use on the
 * stack. Frees multiple commands, given a size of arr by argc.
 * */
int freeCommands(Command **commands, int argc);

/* Frees this command's tokens (but not the pointer!)
 * */
int freeCommand(Command *command);

// Returns a command assigned by input, private use only.
Command *_createBuiltInCommand(char *input, int (*fn)(int argc, char **argv, Command** aliases, int* aliasC, Command** history, int* historyC, Command **builtInCommands, int* builtInC));

int _tokenise(char **argv, int *argc, char *input);

#endif // COMMANDS_H_
