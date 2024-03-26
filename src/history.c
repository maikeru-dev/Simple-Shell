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
  if (command->tokens[0][0] != '!') {
    return 0;
  }
  int size = getSize(command->fullCommand);
  int numIndex;
  if (size == 1) {
    numIndex = 1;
  } else {
    // TODO: Expect other formats.
    numIndex = getNumberHistory(&command->tokens[0][0], size);
    if (numIndex == 0) {
      printf("Usage: !<num> | !\n");
      return 0;
    }
  }

  if (history[numIndex] == NULL) {
    printf("Processed index %d but invalid history invocation.\n", numIndex);
    return 0;
  }

  return numIndex;
}
