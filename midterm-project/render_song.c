#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "io.h"
#include "wave.h"

int main(int argc, char* argv[]){
  if (argc!=3) fatal_error("Wrong number of arguments");
  char * song_input = argv[1];
  char * wave_output = argv[2];
  FILE * song = fopen(song_input, "r");
  unsigned int num_samples, beat_size;
  assert(fscanf(song, "%u %u", &num_samples, &beat_size) == 2); 
  int16_t *buf = calloc(num_samples*2, sizeof(int16_t));
  int index = 0;

  char command;
  int note;
  int voice = 0;
  // array to hold notes in chord command
  int capacity = 10;
  int * notes = malloc(sizeof(int) * capacity);
  float amplitude = 0.1;
  float frequency, num_of_beats;

  // continues reading commands until file ends
  // first letter read determines which of the 5 possible commands will execute
  // commands: N for note, C for chord, P for pause, V for change of voice,
  //           A for change in amplitude
  // for every reading by scanf, we check that the value was properly read
  while (fscanf(song, " %c", &command) != EOF)
    {
      switch (command)
	{
	case 'N' :
	  if (fscanf(song, "%f%d", &num_of_beats, &note) != 2)
	    fatal_error("malformed note command");
	  frequency = 440 * (float)pow(2.0, (float)(note-69)/12);
	  render_voice_stereo (&buf[index], num_of_beats*beat_size, frequency, amplitude, voice);
	  index += num_of_beats*beat_size*2;
	  break;
	case 'C' :
	  if (fscanf(song, "%f", &num_of_beats) != 1)
	    fatal_error("malformed chord command");
	  int i = 0;
	  // reads first note
	  if (fscanf(song, "%d", &notes[i]) != 1)
	    fatal_error("malformed chord command");
	  // reads the rest of the notes, expanding array if needed
	  while (notes[i] != 999)
	    {
	      if (i+1 >= capacity)
		{
		  capacity+= 10;
		  notes = realloc(notes, sizeof(int) * capacity);
		}
	      i++;
	      if (fscanf(song, "%d", &notes[i]) != 1)
		fatal_error("malformed chord command");
	    }
	  // i number of notes in notes array, and 999 is at index i
	  // we iterate through the array to render and add samples for each note
	  for (int k = 0; k < i; k++)
	    {
	      frequency = 440 * (float)pow(2.0, (double)(notes[k]-69)/12);
	      render_voice_stereo (&buf[index], num_of_beats*beat_size, frequency, amplitude, voice);
	    }
	  index += num_of_beats*beat_size*2;
	  break;
	case 'P' :
	  if (fscanf(song, "%f", &num_of_beats) != 1)
	    fatal_error("malformed pause command");
	  index += beat_size*num_of_beats*2;
	  break;
	case 'V' :
	  if (fscanf(song, "%d", &voice) != 1)
	    fatal_error("malformed voice change command");
	  break;
	case 'A' :
	  if (fscanf(song, "%f", &amplitude) != 1)
	      fatal_error("malformed amplitude change command");
	  break;
	default :
	  fatal_error("malformed song file");
	  
	}
    }

  fclose(song);
  FILE * output = fopen(wave_output, "wb");
  write_wave_header(output, num_samples);
  int items_written = fwrite(buf, sizeof(int16_t), num_samples*2, output);
  assert(items_written == (int)num_samples*2);
  fclose(output);
  free(buf);
  free(notes);
  
  return 0;
}
