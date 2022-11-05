#include "Tree.h"

#include <stdlib.h>
#include <string.h>
#include "Assert.h"

#pragma GCC diagnostic ignored "-Wunused-parameter"

#define ERROR(...)                              \
  do                                            \
    {                                           \
      if (!error)                               \
        *error = -1;                            \
                                                \
      return __VA_ARGS__;                       \
    } while (0)

Node *createNode(element_t value, int *error)
{
  assert(validateValue(value));

  Node *node = (Node *)calloc(1, sizeof(Node));

  if (!node)
    ERROR(nullptr);

  Node newNode = {};
  strncpy(newNode.value, value, MAX_SIZE);

  *node = newNode;

  return node;
}

Node *createNode(element_t value, Node *parent, int leftChild, int *error)
{
  assert(parent);

  Node *node = createNode(value, error);

  if (!node)
    ERROR(nullptr);

  return setParent(node, parent, leftChild, error);
}

Node *setParent(Node *child, Node *parent, int leftChild, int *error)
{
  assert(child);
  assert(parent);

  child->parent = parent;

  if (leftChild)
    parent->left  = child;
  else
    parent->right = child;

  return child;
}

void removeNode(Node *node, int *error)
{
  assert(node);

  if (node->left)
    removeNode(node->left);

  if (node->right)
    removeNode(node->right);

  free(node);
}
