#ifndef BUFFER_H
#define BUFFER_H

struct Buffer
{
  int max_length;
  int length;
  char * data;
};

#define BUFFER_CHUNK_SIZE 4096

void new_buffer(struct Buffer * buffer);

void delete_buffer(struct Buffer * buffer);

unsigned int buffer_grow(struct Buffer * buffer, unsigned int length);

void buffer_append(struct Buffer * buffer, const char * data, unsigned int length);

void buffer_append_string(struct Buffer * buffer, const char * data);

void buffer_append_encode_base64(struct Buffer * buffer, const uint8_t * data, unsigned int length);

int buffer_from_file(struct Buffer * buffer, const char * filename);

#endif//BUFFER_H
