#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "io.h"

int main(){
  /*  write_byte(stdout, 'F');
  printf("\n");

  char x[5] = {'t', 'r', 'e', 'e', '\0'};
  write_bytes(stdout, x, 5);

  uint16_t z = 10;
  write_u16(stdout, z);

  uint32_t y = 300;
  write_u32(stdout, y);

  int16_t sx = -10;
  write_s16(stdout, sx);

  int16_t sxs[5] = {-4, -3, -1, 0, -2};
  write_s16_buf(stdout, sxs, 5);
  */

  char plzwork;
  read_byte(stdin, &plzwork);
  printf("%c", plzwork);

}
