#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "io.h"
#include "wave.h"
#include <assert.h>

int main(int argc, char* argv[]){
  if (argc!=6) fatal_error("Wrong number of arguments");
  int voice = atoi(argv[1]);
  float frequency = atof(argv[2]);
  float amplitude = atof(argv[3]);
  unsigned int numsamples = (unsigned int)atoi(argv[4]);
  int16_t *buf = calloc(numsamples*2, sizeof(int16_t));

  render_voice_stereo(buf, numsamples, frequency, amplitude, voice);
  FILE *output =  fopen(argv[5], "wb");
  write_wave_header(output, numsamples);
  int items_written = fwrite(buf, sizeof(int16_t), numsamples*2, output);
  assert(items_written == (int)numsamples*2);
  free(buf);
  fclose(output);

  
  return 0;
}
