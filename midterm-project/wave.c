#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include "io.h"
#include "wave.h"

/*
 * Write a WAVE file header to given output stream.
 * Format is hard-coded as 44.1 KHz sample rate, 16 bit
 * signed samples, two channels.
 *
 * Parameters:
 *   out - the output stream
 *   num_samples - the number of (stereo) samples that will follow
 */
void write_wave_header(FILE *out, unsigned num_samples) {
  /*
   * See: http://soundfile.sapp.org/doc/WaveFormat/
   */

  uint32_t ChunkSize, Subchunk1Size, Subchunk2Size;
  uint16_t NumChannels = NUM_CHANNELS;
  uint32_t ByteRate = SAMPLES_PER_SECOND * NumChannels * (BITS_PER_SAMPLE/8u);
  uint16_t BlockAlign = NumChannels * (BITS_PER_SAMPLE/8u);

  /* Subchunk2Size is the total amount of sample data */
  Subchunk2Size = num_samples * NumChannels * (BITS_PER_SAMPLE/8u);
  Subchunk1Size = 16u;
  ChunkSize = 4u + (8u + Subchunk1Size) + (8u + Subchunk2Size);

  /* Write the RIFF chunk descriptor */
  write_bytes(out, "RIFF", 4u);
  write_u32(out, ChunkSize);
  write_bytes(out, "WAVE", 4u);

  /* Write the "fmt " sub-chunk */
  write_bytes(out, "fmt ", 4u);       /* Subchunk1ID */
  write_u32(out, Subchunk1Size);
  write_u16(out, 1u);                 /* PCM format */
  write_u16(out, NumChannels);
  write_u32(out, SAMPLES_PER_SECOND); /* SampleRate */
  write_u32(out, ByteRate);
  write_u16(out, BlockAlign);
  write_u16(out, BITS_PER_SAMPLE);

  /* Write the beginning of the "data" sub-chunk, but not the actual data */
  write_bytes(out, "data", 4);        /* Subchunk2ID */
  write_u32(out, Subchunk2Size);
}

/*
 * Read a WAVE header from given input stream.
 * Calls fatal_error if data can't be read, if the data
 * doesn't follow the WAVE format, or if the audio
 * parameters of the input WAVE aren't 44.1 KHz, 16 bit
 * signed samples, and two channels.
 *
 * Parameters:
 *   in - the input stream
 *   num_samples - pointer to an unsigned variable where the
 *      number of (stereo) samples following the header
 *      should be stored
 */
void read_wave_header(FILE *in, unsigned *num_samples) {
  char label_buf[4];
  uint32_t ChunkSize, Subchunk1Size, SampleRate, ByteRate, Subchunk2Size;
  uint16_t AudioFormat, NumChannels, BlockAlign, BitsPerSample;

  read_bytes(in, label_buf, 4u);
  if (memcmp(label_buf, "RIFF", 4u) != 0) {
    fatal_error("Bad wave header (no RIFF label)");
  }

  read_u32(in, &ChunkSize); /* ignore */

  read_bytes(in, label_buf, 4u);
  if (memcmp(label_buf, "WAVE", 4u) != 0) {
    fatal_error("Bad wave header (no WAVE label)");
  }

  read_bytes(in, label_buf, 4u);
  if (memcmp(label_buf, "fmt ", 4u) != 0) {
    fatal_error("Bad wave header (no 'fmt ' subchunk ID)");
  }

  read_u32(in, &Subchunk1Size);
  if (Subchunk1Size != 16u) {
    fatal_error("Bad wave header (Subchunk1Size was not 16)");
  }

  read_u16(in, &AudioFormat);
  if (AudioFormat != 1u) {
    fatal_error("Bad wave header (AudioFormat is not PCM)");
  }

  read_u16(in, &NumChannels);
  if (NumChannels != NUM_CHANNELS) {
    fatal_error("Bad wave header (NumChannels is not 2)");
  }

  read_u32(in, &SampleRate);
  if (SampleRate != SAMPLES_PER_SECOND) {
    fatal_error("Bad wave header (Unexpected sample rate)");
  }

  read_u32(in, &ByteRate); /* ignore */

  read_u16(in, &BlockAlign); /* ignore */

  read_u16(in, &BitsPerSample);
  if (BitsPerSample != BITS_PER_SAMPLE) {
    fatal_error("Bad wave header (Unexpected bits per sample)");
  }

  read_bytes(in, label_buf, 4u);
  if (memcmp(label_buf, "data", 4u) != 0) {
    fatal_error("Bad wave header (no 'data' subchunk ID)");
  }

  /* finally we're at the Subchunk2Size field, from which we can
   * determine the number of samples */
  read_u32(in, &Subchunk2Size);
  *num_samples = Subchunk2Size / NUM_CHANNELS / (BITS_PER_SAMPLE/8u);
}

/*
 * ensures that the sample value stored in the array is within the allowed range
 * clips sample value if necessary
 *
 * Parameters:
 *  sample_value - the calculated sample value that must be checked
 * 
 */
int16_t clip_check (int32_t sample_value)
{
  if (sample_value > 32767)
    {
      return 32767;
    }
  else if (sample_value < -32768)
    {
      return -32768;
    }
  return (int16_t)sample_value;
}

/*
 * generates sample values for a sine wave at the given frequency and amplitude 
 * adds sample values for one channel to the given array
 *
 * Parameters:
 *   buf - the array of samples that the sine wave will be added to
 *   num_samples - the number of values to generate
 *   channel - determines the channel (0 is left, 1 is right)
 *   freq_hz - the frequency of the sine wave
 *   amplitude - the amplitude of the sine wave
 */
void render_sine_wave(int16_t buf[], unsigned num_samples, unsigned channel,  float freq_hz, float amplitude){
  int16_t p;
  int32_t sample_value;
  // iterates through the buf array, adding the sine wave value samples in the appripriate channel
  // uses clip_check to ensure that the new values in buf are within allowed range
  for(unsigned int i = channel; i<num_samples*2; i+=2)
    {
      // (i/2) is the sample number, so (i/2)/SAMPLES_PER_SECOND gives the current time
      p  = (int16_t)(MAX*(amplitude * (sin(((double)(i/2u)/SAMPLES_PER_SECOND)*freq_hz*2*PI))));
      sample_value = p + buf[i];
      buf[i] = clip_check(sample_value);
    }
}

/*
 * generates sample values for both channels of a sine wave at the given frequency and amplitude 
 *
 * Parameters:
 *   buf - the array of samples that the sine wave samples will be added to
 *   num_samples - the number of values to generate
 *   freq_hz - the frequency of the sine wave
 *   amplitude - the amplitude of the sine wave
 */
void render_sine_wave_stereo(int16_t buf[], unsigned num_samples, float freq_hz, float amplitude){
  render_sine_wave(buf, num_samples, 0u, freq_hz, amplitude);
  render_sine_wave(buf, num_samples, 1u, freq_hz, amplitude);  
}


/*
 * generates sample values for a square wave at the given frequency and amplitude 
 * adds sample values for one channel to the given array
 *
 * Parameters:
 *   buf - the array of samples that the square wave samples will be added to
 *   num_samples - the number of values to generate
 *   channel - determines the channel (0 is left, 1 is right)
 *   freq_hz - the frequency of the square wave
 *   amplitude - the amplitude of the square wave
 */
void render_square_wave(int16_t buf[], unsigned num_samples, unsigned channel, float freq_hz, float amplitude)
{
  int16_t p;
  int32_t sample_value;
  // iterates through the buf array, adding the square wave value samples in the appropriate channel
  // uses clip_check to ensure that the new values in buf are within allowed range
  for (unsigned int i = channel; i < num_samples*2; i+=2)
    {
      if (sin(((double)(i/2u)/SAMPLES_PER_SECOND)*freq_hz*2*PI) >= 0)
	p = (int16_t)(amplitude*MAX);
      else
	p = (int16_t)(amplitude*(MIN));
      sample_value = p + buf[i];
      buf[i] = clip_check(sample_value);
    }
}


/*
 * generates sample values in both channels for a square wave at the given frequency and amplitude 
 *
 * Parameters:
 *   buf - the array of samples that the square wave samples will be added to
 *   num_samples - the number of values to generate
 *   freq_hz - the frequency of the square wave
 *   amplitude - the amplitude of the square wave
 */
void render_square_wave_stereo(int16_t buf[], unsigned num_samples, float freq_hz, float amplitude)
{
  render_square_wave(buf, num_samples, 0u, freq_hz, amplitude);
  render_square_wave(buf, num_samples, 1u, freq_hz, amplitude);
}


/*
 * generates sample values for a saw wave at the given frequency and amplitude 
 * adds sample values for one channel to the given array
 *
 * Parameters:
 *   buf - the array of samples that the saw wave samples will be added to
 *   num_samples - the number of values to generate
 *   channel - determines the channel (0 is left, 1 is right)
 *   freq_hz - the frequency of the saw wave
 *   amplitude - the amplitude of the saw wave
 */
void render_saw_wave(int16_t buf[], unsigned num_samples, unsigned channel, float freq_hz, float amplitude){

  double cyclenum;
  double fraction;
  int32_t sample_val;
  int16_t p;
  // iterates through array, adding saw wave sample values in the appropriate channel
  // (i/2)/SAMPLES_PER_SECOND gives the current time, and 1/freq_hz gives the time of one cycle
  // so dividing the current time by the cycle time gives us the current cycle we are on
  // we only care about where in the current cycle, so we subtract the floor of the cycle number
  for(unsigned int i = channel; i<num_samples*2; i+=2){
    cyclenum = ((double)(i/2)/SAMPLES_PER_SECOND)/(1/freq_hz);
    fraction = cyclenum - floor(cyclenum);
    p = (int16_t)((amplitude*(MAX-MIN)*fraction) + (amplitude*MIN)); 
    sample_val = p+buf[i];
    buf[i] = clip_check(sample_val);
  }
}


/*
 * generates sample values for both channels for a saw wave at the given frequency and amplitude 
 *
 * Parameters:
 *   buf - the array of samples that the saw wave samples will be added to
 *   num_samples - the number of values to generate
 *   freq_hz - the frequency of the saw wave
 *   amplitude - the amplitude of the saw wave
 */
void render_saw_wave_stereo(int16_t buf[], unsigned num_samples, float freq_hz, float amplitude){
  render_saw_wave(buf, num_samples, 0u, freq_hz, amplitude);
  render_saw_wave(buf, num_samples, 1u, freq_hz, amplitude);
}


/*
 * generates sample values for both channels the given wave type at the given frequency and amplitude 
 *
 * Parameters:
 *   buf - the array of samples that the generated samples will be added to
 *   num_samples - the number of values to generate
 *   freq_hz - the frequency of the wave
 *   amplitude - the amplitude of the wave
 *   voice - determines type of wave to generate (0 is sine, 1 is square, 2 is saw)
 */
void render_voice_stereo(int16_t buf[], unsigned num_samples, float freq_hz, float amplitude, unsigned voice){
  if (voice == SINE){
    render_sine_wave_stereo(buf, num_samples, freq_hz, amplitude);
  }
  if (voice == SQUARE){
    render_square_wave_stereo(buf, num_samples, freq_hz, amplitude);
  }
  if (voice == SAW){
    render_saw_wave_stereo(buf, num_samples, freq_hz, amplitude);
  }
  
}
