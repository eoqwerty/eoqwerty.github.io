#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "io.h"
#include "wave.h"
#include <assert.h>

int main(int argc, char* argv[]){
  if (argc!=5) fatal_error("Wrong number of arguments");
  FILE *wavfilein =  fopen(argv[1], "rb");
  FILE *wavfileout =  fopen(argv[2], "wb");
  int delay = atoi(argv[3]);
  float amplitude = atof(argv[4]);
  unsigned int num_samples;
  read_wave_header(wavfilein, &num_samples);
  int16_t *buf = calloc(num_samples*2, sizeof(int16_t));

  int samples_read = fread(buf, sizeof(int16_t), num_samples*2, wavfilein);
  assert(samples_read == (int)num_samples*2);

  int16_t p;
  int32_t sample_value;

  // for loops iterate through the array of samples starting from the end
  // the sample to be echoed is accessed using delay, then added to to current sample
  // 2 different for loops for left and right channel
  for (int i = (num_samples*2)-2; i >= delay*2; i-=2)
    {
      p = amplitude * buf[i-(delay*2)];
      sample_value = p + buf[i];
      buf[i] = clip_check(sample_value);
    }

  for (int i = (num_samples*2)-1; i >= delay*2; i-=2)
    {
      p = amplitude * buf[i-(delay*2)];
      sample_value = p + buf[i];
      buf[i] = clip_check(sample_value);
    }
  
  write_wave_header(wavfileout, num_samples);
  int items_written = fwrite(buf, sizeof(int16_t), num_samples*2, wavfileout);
  assert(items_written == (int)num_samples*2);
  fclose(wavfilein);
  fclose(wavfileout);
  free(buf);

  return 0;
}
