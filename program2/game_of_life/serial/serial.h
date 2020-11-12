#ifndef SERIAL_H
#define SERIAL_H

#include <stdlib.h>

#include "../utils.h"

int ** generate_game(command_args_t config);

int ** run_game(int** board, command_args_t config);

#endif