/* constants */
#ifndef CONSTANTS_H
#define CONSTANTS_H

#define MAX_INPUT_LEN 256
#define TRUE 1
#define FALSE 0

#ifndef NULL
#define NULL 0
#endif

enum error_code {
    INVALID_BUTTON = -3,
    CANNOT_GET_CHAR = -2,
    CANNOT_PUT_CHAR = -1,
    SUCCESS, /* implicitly counting on this to be 0 */
    READ_ERROR,
    WRITE_ERROR,
    TIME_ERROR,
    EXIT_ARGS_UNSUPPORTED,
    DATE_ARGS_UNSUPPORTED,
    WRONG_NUMBER_ARGS,
    MALLOC_ERROR,
    INVALID_INPUT,
    CANNOT_CREATE_FILE,
    CANNOT_DELETE_FILE,
    CANNOT_OPEN_FILE,
    CANNOT_CLOSE_FILE
};

#endif
