#ifndef PRINTELEMENT_H_
#define PRINTELEMENT_H_

#include <stdio.h>

/// Print int element
/// @param [in] element Stack element for writing
/// @param [in] filePtr File for writing
/// @return Count of chars which was write or -1 if element == nullptr
int printElement(int element, FILE *filePtr);

/// Return length of elemtnt
/// @param [in] element Stack element for writing
/// @return Length of element in chars or -1 if element == nullptr
int elementLength(int element);

/// Return max length of element
/// @param [in] element Stack element
/// @return Max element length
int maxElementLength(int element);

/// Return Poison value for stack
/// @param [in] element Stack element
/// @return Poison value
int getPoison(int element);

/// Check that element is poison
/// @param [in] element Stack element
/// @return Is element poison
int isPoison(int element);

#endif
