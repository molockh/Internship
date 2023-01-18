#include <iostream>
#include "myvector.h"
//#include "codereviewtask_myvector.hpp"

int main() {
  int x = 5;
  int& a = x;
  int& b = x;
  ++x;
  std::cout << x << " " << a << " " << b << std::endl;
  return 0;
}
