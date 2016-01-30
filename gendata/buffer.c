#include <stdint.h>
#include <malloc.h>
#include <string.h>
#include <stdio.h>

#include "buffer.h"

void new_buffer(struct Buffer * buffer)
{
  buffer->max_length = BUFFER_CHUNK_SIZE;
  buffer->length = 0;
  buffer->data = malloc(BUFFER_CHUNK_SIZE);
}

void delete_buffer(struct Buffer * buffer)
{
  free(buffer->data);
}

unsigned int buffer_grow(struct Buffer * buffer, unsigned int length)
{
  unsigned int new_length = length + buffer->length;
  if (new_length > buffer->max_length)
  {
    int chunks = (new_length + BUFFER_CHUNK_SIZE - 1) / BUFFER_CHUNK_SIZE;
    buffer->max_length = chunks * BUFFER_CHUNK_SIZE;
    buffer->data = realloc(buffer->data, buffer->max_length);
  }
  return new_length;
}

void buffer_append(struct Buffer * buffer, const char * data, unsigned int length)
{
  unsigned int new_length = buffer_grow(buffer, length);
  memcpy(buffer->data + buffer->length, data, length);
  buffer->length = new_length;
}

void buffer_append_string(struct Buffer * buffer, const char * data)
{
  int len = strlen(data);
  buffer_append(buffer, data, len);
}

void buffer_append_encode_base64(struct Buffer * buffer, const uint8_t * data, unsigned int length)
{
  unsigned int new_length = buffer_grow(buffer, ((length*8/6 + 3) & (~3)) );// + 1);
  static const char *b64 =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  int i, j, a, b, c;
  uint8_t * dst = buffer->data + buffer->length;

  for (i = j = 0; i < length; i += 3)
  {
    a = data[i];
    b = i + 1 >= length ? 0 : data[i + 1];
    c = i + 2 >= length ? 0 : data[i + 2];

    dst[j++] = b64[a >> 2];
    dst[j++] = b64[((a & 3) << 4) | (b >> 4)];
    if (i + 1 < length) {
      dst[j++] = b64[(b & 15) << 2 | (c >> 6)];
    }
    if (i + 2 < length) {
      dst[j++] = b64[c & 63];
    }
  }
  while (j % 4 != 0)
  {
    dst[j++] = '=';
  }
  //dst[j++] = '\0';

  buffer->length = new_length;
}

int buffer_from_file(struct Buffer * buffer, const char * filename)
{
  FILE * f;
  size_t size;

  buffer->length = 0;
  buffer->data = NULL;

  f = fopen(filename, "rb");
  if (f)
  {
    fseek(f, 0, SEEK_END);
    size = ftell(f);
    fseek(f, 0, SEEK_SET);
    
    if (size > 0)
    {
      new_buffer(buffer);
      buffer_grow(buffer, size);
      fread(buffer->data, 1, size, f);
      buffer->length = size;
    }
    
    fclose(f);
  }

  return buffer->length;
}
