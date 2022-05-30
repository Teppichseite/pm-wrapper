#ifndef TYPES_H
#define TYPES_H
#include <vector>

enum PointerType { UNKNOWN, PM, NO_PM, UNDECLARED };

struct FunctionType {
  PointerType returnType;
  std::vector<PointerType> parameterTypes;
  bool reserved;
};

#endif