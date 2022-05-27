#ifndef TYPES_H
#define TYPES_H
#include <vector>

enum PointerType { PM, VM, UNDECLARED, UNKNOWN, NO_POINTER };

struct FunctionType {
  PointerType returnType;
  std::vector<PointerType> parameterTypes;
  bool reserved;
};

#endif