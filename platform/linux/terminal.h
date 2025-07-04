#ifndef TERMINAL_H
#define TERMINAL_H

#include "types.h"

void init_terminal(void);
bool get_char(u8 *data);
void put_char(u8 data);

#endif
