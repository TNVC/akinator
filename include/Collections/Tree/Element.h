#pragma once

#include <string.h>

#define MAX_SIZE 256

typedef char element_t[MAX_SIZE];

inline int validateValue(const element_t element)
{
  return element != nullptr;
}

inline int compareValues(const element_t first, const element_t second)
{
  return strcmp(first, second);
}

inline const char *toString(const element_t element)
{
  return element;
}
