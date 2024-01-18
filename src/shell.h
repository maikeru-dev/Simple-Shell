#include "commands.h"
#include "constants.h"

#ifndef SHELL_H_
#define SHELL_H_

// TODO: Finish state.
// Undeveloped Idea: state is necessary with History and Alias list, but could
// be useful.
struct State {
  Command **Aliases;
  Command **History;
};

typedef struct State State;

void quit();

#endif // SHELL_H_
