#pragma once

#include "Element.h"

#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>

struct Node {
  element_t value;
  Node *parent;
  Node *left;
  Node *right;
};

struct Tree {
  Node *root;
  size_t size;
};

enum TreeError {
  TREE_NULLPTR = 0x01 << 0,
};

const int ERRORS_COUNT = 1;

Node *createNode(element_t value, int *error = nullptr);

Node *createNode(element_t value, Node *parent, int leftChild = true, int *error = nullptr);

Node *setParent(Node *child, Node *parent, int leftChild = true, int *error = nullptr);

void removeNode(Node *node, int *error = nullptr);


unsigned validateTree(const Tree *tree);

void createTree(Tree *tree, int *error = nullptr);

void destroyTree(Tree *tree, int *error = nullptr);

void addElement(Node *node, element_t value, int leftChild, int *error = nullptr);

const Node *findElement(const Tree *tree, element_t value, int isList = false, int *error = nullptr);

#define dumpTree(TREE, ERROR, FILE)                           \
  do_dumpTree(TREE, ERROR, FILE, __FILE__, __func__, __LINE__, "")

#define dumpTreeWithMessage(TREE, ERROR, FILE, MESSAGE, ...)            \
  do_dumpTree(TREE, ERROR, FILE, __FILE__, __func__, __LINE__, MESSAGE __VA_OPT__(,) __VA_ARGS__)

void do_dumpTree(
              const Tree *tree,
              unsigned error,
              FILE *file,
              const char* fileName,
              const char* functionName,
              int line,
              const char *message,
              ...
              );

void saveTree(const Tree *tree, FILE *file, int *error = nullptr);

void loadTree(Tree *tree, FILE *file, int *error = nullptr);
