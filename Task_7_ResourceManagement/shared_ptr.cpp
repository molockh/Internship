#include <iostream>

#include "shared_ptr.h"

int main() {
  {
    std::shared_ptr<int> def;
    sp::shared_ptr<int> def1;

    std::shared_ptr<int> ptr_strl(new int(10));

    sp::shared_ptr<int> ptr(new int(5));

    sp::shared_ptr<int> ptr_1(ptr);

    sp::shared_ptr<int> ptr_2(std::move(ptr));

    {
      sp::weak_ptr<int> weak(ptr_1);
    }

    std::cout << *ptr_1 << " " << *ptr_2 << std::endl;
  }
//  sp::shared_ptr<int> shared = ddsp::make_shared<int>(5);

  return 0;
}