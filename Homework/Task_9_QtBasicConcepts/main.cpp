#include <QApplication>
#include <iostream>

#include "counter.h"

int main(int c, char** v) {
  Counter c1(10);
  Counter c2(20);

  std::cout << "Initial values: 10, 20\n" << std::endl;

  QObject::connect(&c1,
                   &Counter::SignalChange,
                   &c2,
                   &Counter::SlotProcessChange);
  QObject::connect(&c2,
                   &Counter::SignalChange,
                   &c1,
                   &Counter::SlotProcessChange);

  c1.Increment();
  std::cout << c1.Count() << " " << c2.Count() << std::endl;
  std::cout << std::endl;

  c2.Increment();
  std::cout << c1.Count() << " " << c2.Count() << std::endl;
  std::cout << std::endl;

  c1.Decrement();
  std::cout << c1.Count() << " " << c2.Count() << std::endl;
  std::cout << std::endl;

  c2.Decrement();
  std::cout << c1.Count() << " " << c2.Count() << std::endl;
  std::cout << std::endl;

  return 0;
}