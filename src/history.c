#include "commands.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int shuntPtrArr(Command **arr, int argc) {
  // size: 5, assume arr is full
  // i: 0 is removed, i:5 is now null.
  //

  for (int i = 0; i < argc - 1; i++) {
    arr[i] = arr[i + 1];
  }

  arr[argc - 1] = NULL;

  return 0;
}

int getSize(char *input) {
  int i = 0;
  while (input[i] != '\0') {
    i++;
  }

  return i;
}
int getNumberHistory(char *input, int length) {

  if (length <= 1)
    return 0;

  char out[length];
  out[length - 1] = '\0';

  for (int i = 1; i < length; i++) {
    out[i - 1] = input[i];
  }

  return atoi(out);
}

int validateHistory(Command **history, int historyC, Command *command) {

  int size = getSize(command->fullCommand);
  int numIndex;
  printf("Command length: %d\n", size);
  if (size >= 2) {
    // TODO: Expect other formats.
    switch (command->tokens[0][1]) {
    case '-':
      numIndex = historyC - getNumberHistory(&command->tokens[0][0], size);
      if (numIndex == 0) {
        printf("Usage: !<num> | !\n");
        return -1;
      }
      break;
    case '!':
      numIndex = historyC;
      break;

    default:
      numIndex = getNumberHistory(&command->tokens[0][0], size);
      if (numIndex == 0) {
        printf("Usage: !<num> | !\n");
        return -1;
      }
      break;
    }
  } else {
    printf("Usage: !<num> | !! | !-<num>\n");
  }

  if (history[numIndex - 1] == NULL) {
    printf("Processed index %d but invalid history invocation.\n", numIndex);
    return -1;
  } else {
    printf("Found correct index %d\n", numIndex);
  }

  return numIndex;
}
