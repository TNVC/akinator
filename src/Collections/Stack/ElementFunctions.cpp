#include <stdio.h>
#include "SystemLike.h"
#include "ElementFunctions.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

int printElement(int element, FILE *filePtr)
{
  if (!isPointerCorrect(filePtr))
    return -1;

  return fprintf(filePtr, "%d", element);
}

int elementLength(int value)
{
  int charsNum = !(value > 0);

  value = value * !charsNum - value * charsNum;
  while (value > 0)
    {
      ++charsNum;

      value /= 10;
    }

  return charsNum;
}

int maxElementLength(int element)
{
  return 12;
}

int getPoison(int element)
{
  return (int)0xDED00DED;
}

int isPoison(int element)
{
  return (int)0xDED00DED == element;
}


int printElement(Node *element, FILE *filePtr)
{
  if (!isPointerCorrect(filePtr))
    return -1;

  return fprintf(filePtr, "%60.60s", element->value);
}

int elementLength(Node *value)
{
  return 60;
}

int maxElementLength(Node *element)
{
  return 60;
}

Node *getPoison(Node *element)
{
  return nullptr;
}

int isPoison(Node * element)
{
  return nullptr == element;
}
