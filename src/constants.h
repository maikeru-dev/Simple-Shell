#define PROMPT "> "

// These are defined in the Simple_Shell.pdf document
// Please avoid changing them.
#define COMMAND_LENGTH 512
#define TOKENS_LENGTH 50
#define HISTORY_LENGTH 6 // 5 technically.
#define ALIASES_LENGTH 10

// Our definitions:
#define WORD_LENGTH 16 // I don't think this is used.
#define TOKENISER " \n|><&;\t"

#define INPUT_OK 0
#define INPUT_TERMINATE 1
#define INPUT_SKIP 2

#define PATH_LENGTH 32767

#define TOTAL_CMDS 6
