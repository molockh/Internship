#ifndef TASK_9_QT_BASICCONCEPTS__COUNTER_H_
#define TASK_9_QT_BASICCONCEPTS__COUNTER_H_

#include <QObject>
#include <iostream>

enum class Operation {
  kIncrement = 0,
  kDecrement
};

class Counter : public QObject {
 Q_OBJECT
 public:
  explicit Counter(int startValue);
  int Count() const;
  void Increment();
  void Decrement();

 signals:
  void SignalChange(Operation operation);

 public slots:
  void SlotProcessChange(Operation operation);

 private:
  int m_count;
};

#endif //TASK_9_QT_BASICCONCEPTS__COUNTER_H_
