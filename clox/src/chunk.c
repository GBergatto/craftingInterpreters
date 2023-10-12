#include <stdlib.h>

#include "chunk.h"
#include "memory.h"

void initChunk(Chunk* chunk) {
  chunk->count = 0;
  chunk->lastLine = 0;
  chunk->capacity = 0;
  chunk->linesCapacity = 0;
  chunk->code = NULL;
  chunk->lines = NULL;
  initValueArray(&chunk->constants); // constant pool
}

// append a byte to the chunk
void writeChunk(Chunk* chunk, uint8_t byte, int line) {
  if (chunk->capacity < chunk->count + 1) { // reallocate values array (constant pool)
    int oldCapacity = chunk->capacity;
    chunk->capacity = GROW_CAPACITY(oldCapacity);
    chunk->code = GROW_ARRAY(uint8_t, chunk->code, oldCapacity, chunk->capacity);
  }

  if (chunk->linesCapacity < line + 1) { // reallocate lines array
    int oldLinesCapacity = chunk->linesCapacity;
    chunk->linesCapacity = GROW_CAPACITY(oldLinesCapacity);
    chunk->lines = GROW_ARRAY(int, chunk->lines, oldLinesCapacity, chunk->linesCapacity);
  }

  chunk->code[chunk->count] = byte;
  chunk->count++;

  if (chunk->lastLine == line) {
    // increase the number of occurences
    chunk->lines[chunk->lastLine]++;
  } else {
    // add the first element on the new line
    chunk->lastLine = line;
    chunk->lines[chunk->lastLine] = 1;
  }
}

void freeChunk(Chunk* chunk) {
  FREE_ARRAY(uint8_t, chunk->code, chunk->capacity);
  FREE_ARRAY(int, chunk->lines, chunk->capacity);
  freeValueArray(&chunk->constants);
  initChunk(chunk);
}

int addConstant(Chunk* chunk, Value value) {
  writeValueArray(&chunk->constants, value);
  return chunk->constants.count - 1;
}

int getLine(Chunk* chunk, int offset) {
  int pos = 1;
  while (chunk->lines[pos] <= offset) {
    offset -= chunk->lines[pos];
    pos++;
  }
  return pos;
}
