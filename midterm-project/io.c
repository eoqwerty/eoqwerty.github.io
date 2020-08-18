#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "io.h"

// takes in a string error message, prints the message, and exits the proram 
void fatal_error(const char *message){
  fprintf(stderr, "Error: %s", message);
  exit(1);
}

// takes in a file pointer and a character, writes the character to the file
// calls fatal_error if fputc fails
void write_byte(FILE *out, char val){
  if(fputc(val, out) == EOF)
    fatal_error("Write_byte failed\n");
 }

// takes in a file pointer, an array of chars, and the length of that array
// writes each character in the array to the file by calling write_byte
void write_bytes(FILE *out, const char data[], unsigned n){
  for(unsigned int i = 0; i<n; i++){
    write_byte(out, data[i]);
  }
}

// takes in a file pointer and an unsigned int16_t value
// writes the value to the file in little endian format
void write_u16(FILE *out, uint16_t value){
  if(fputc(value%256, out) == EOF)
    fatal_error("write_u16 failed\n");
  if(fputc((value/256)%256, out) == EOF)
    fatal_error("write_u16 failed\n");
}

// takes in a file pointer and an unsigned int32_t value
// writes the value to the file in little endian format
void write_u32(FILE *out, uint32_t value){
  if (fputc((value)%256, out) == EOF)
    fatal_error("write_u32 failed\n");
  if (fputc((value/256)%256, out) == EOF)
    fatal_error("write_u32 failed\n");
  if (fputc((value/65536)%256, out) == EOF)
    fatal_error("write_u32 failed\n");
  if (fputc((value/16777216)%256, out) == EOF)
    fatal_error("write_u32 failed\n");
}

// takes in a file pointer and an int16_t value                                   
// writes the value to the file in little endian format  
void write_s16(FILE *out, int16_t value){
  if (fputc(value%256, out) == EOF) fatal_error("write_s16 failed\n");
  if (fputc((value/256)%256, out) == EOF) fatal_error("write_s16 failed\n");
}

// takes in a file pointer, an array of ing16_t values, and the length of the array
// writes each value in the array to the file in little endian format
void write_s16_buf(FILE *out, const int16_t buf[], unsigned n){
  for(unsigned int i = 0; i<n; i++){
    write_s16(out, buf[i]);
  }
}

// takes in a file pointer and a char pointer
// reads a byte of data from the file and stores it in the char
// calls fatal_error if the file pointer is at the end of file
void read_byte(FILE *in, char *val)
{
  char temp = fgetc(in);
  if (feof(in))
    {
      fatal_error("read_byte failed");
    }
  *val = temp;
}

// takes in a file pointer and a pointer to an unsigned char
// reads a byte of data from the file and stores it in the char 
// calls fatal_error if the file pointer is at the end of file 
void read_ubyte(FILE *in, unsigned char *val)
{
  unsigned char temp = fgetc(in);
  if (feof(in))
    {
      fatal_error("read_byte failed");
    }
  *val = temp;
}

// takes in a file pointer, an array of chars, and the length of the array
// reads n bytes of data from the file and stores them in the array
void read_bytes(FILE *in, char data[], unsigned n)
{
  for (unsigned int i = 0; i< n; i++)
    {
      read_byte(in, &data[i]);
    }
}

// takes in a file pointer and a pointer to an unsigned int16_t value
// reads a uint16_t value from the file one byte at a time
// then reconstructs the overall value and stores in in the uint16_t pointer
void read_u16 (FILE *in, uint16_t *val)
{
  unsigned char v1, v2;
  read_ubyte(in, &v1);
  read_ubyte(in, &v2);
  *val = v1 + (v2 * 256);
}

// takes in a file pointer and a pointer to an unsigned int32_t value
// reads a uint32_t value from the file one byte at a time                                 
// then reconstructs the overall value and stores in in the uint32_t pointer 
void read_u32 (FILE *in, uint32_t *val)
{
  unsigned char v1, v2, v3, v4;
  read_ubyte(in, &v1);
  read_ubyte(in, &v2);
  read_ubyte(in, &v3);
  read_ubyte(in, &v4);
  *val = v1 + (v2 * 256) + (v3 * 65536) + (v4 * 16777216);
}

// takes in a file pointer and a pointer to an int16_t value
// reads an int16_t value from the file one byte at a time
// reconstructs the value and stores it in the int16_t pointer
void read_s16 (FILE *in, int16_t *val)
{
  char v1, v2;
  read_byte(in, &v1);
  read_byte(in, &v2);
  *val = v1 + (v2 * 256);
}

// takes in a file pointer, an array of int16_t values, and the length of the array
// calls read_s16 to read n int16_t values and store them in the array
void read_s16_buf(FILE *in, int16_t buf[], unsigned n)
{
  for (unsigned int i = 0; i < n; i++)
    {
      read_s16(in, &buf[i]);
    }
}
