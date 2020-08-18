#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "io.h"
#include "wave.h"
#include <assert.h>

int main(int argc, char* argv[]){
  int16_t buf[88200];
  for (int i = 0; i<88200; i++){
    buf[i] = (int16_t)0;    
  }
  render_sine_wave(buf, 44100u, 0, 130.81, 0.2);
  FILE *output = fopen("test2.wav", "wb");
  write_wave_header(output, 88200);
  fwrite(buf, sizeof(int16_t), 88200, output);
  fclose(output);
  return 0;

}
