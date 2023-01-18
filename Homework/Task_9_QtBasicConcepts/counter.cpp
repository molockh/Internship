#include "counter.h"

Counter::Counter(int startValue) : m_count(startValue) {}
int Counter::Count() const {
  return m_count;
}
void Counter::Increment() {
  std::cout << "Counter incremented. New value: " << ++m_count << std::endl;
  emit SignalChange(Operation::kIncrement);
}
void Counter::Decrement() {
  std::cout << "Counter decremented. New value: " << --m_count << std::endl;
  emit SignalChange(Operation::kDecrement);
}

void Counter::SlotProcessChange(Operation operation) {
  if (operation == Operation::kIncrement) {
    std::cout << "Increment signal received. New value: " << ++m_count << std::endl;
  } else if (operation == Operation::kDecrement) {
    std::cout << "Decrement signal received. New value: " << --m_count << std::endl;
  }
}

