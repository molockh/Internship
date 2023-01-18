#include <iostream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <string>
#include <iomanip>
#include <numeric>

#include "forward_list.h"

template<typename T>
std::ostream& operator<<(std::ostream& s, const ForwardList<T>& v) {
  s.put('[');
  char comma[3] = {'\0', ' ', '\0'};
  for (const auto& e : v) {
    s << comma << e;
    comma[0] = ',';
  }
  return s << ']';
}

template<typename Container>
void print(const std::string& comment, const Container& container) {
  auto size = container.size();
  std::cout << comment << "{ ";
  for (auto const& element : container)
    std::cout << element << (--size ? ", " : " ");
  std::cout << "}\n";
}

int main() {
  { // Constructor
    std::cout << "-------------- Constructors test --------------" << std::endl;
    // C++11 initializer list syntax:
    ForwardList<std::string> words1{"the", "frogurt", "is", "also", "cursed"};
    std::cout << "words1: " << words1 << '\n';

    // words2 == words1
    ForwardList<std::string> words2(words1.begin(), words1.end());
    std::cout << "words2: " << words2 << '\n';

    // words3 == words1
    ForwardList<std::string> words3(words1);
    std::cout << "words3: " << words3 << '\n';

    // words4 is {"Mo", "Mo", "Mo", "Mo", "Mo"}
    ForwardList<std::string> words4(5, "Mo");
    std::cout << "words4: " << words4 << '\n';
    std::cout << "-------------- End --------------" << std::endl;
  }

  std::cout << std::endl;

  { // operator=
    std::cout << "-------------- operator= test --------------" << std::endl;

    ForwardList<int> x{1, 2, 3}, y, z;
    const auto w = {4, 5, 6, 7};

    std::cout << "Initially:\n";
    print("x = ", x);
    print("y = ", y);
    print("z = ", z);

    std::cout << "Copy assignment copies data from x to y:\n";
    y = x;
    print("x = ", x);
    print("y = ", y);

    std::cout
        << "Move assignment moves data from x to z, modifying both x and z:\n";
    z = std::move(x);
    print("x = ", x);
    print("z = ", z);

    std::cout << "Assignment of initializer_list w to z:\n";
    z = w;
    print("w = ", w);
    print("z = ", z);

    std::cout << "-------------- End --------------" << std::endl;
  }

  std::cout << std::endl;

  { // front()
    std::cout << "-------------- front() test --------------" << std::endl;

    ForwardList<char> letters{'o', 'm', 'g', 'w', 't', 'f'};

    if (!letters.empty()) {
      std::cout << "The first character is '" << letters.front() << "'.\n";
    }

    std::cout << "-------------- End --------------" << std::endl;
  }

  std::cout << std::endl;

  { // Iterators
    std::cout << "-------------- Iterators test --------------"
              << std::endl;

    ForwardList<int> nums{1, 2, 4, 8, 16};
    ForwardList<std::string> fruits{"orange", "apple", "raspberry"};
    ForwardList<char> empty;

    // Print forward_list.
    std::for_each(nums.begin(),
                  nums.end(),
                  [](const int n) { std::cout << n << ' '; });
    std::cout << '\n';

    // Sums all integers in the forward_list nums (if any), printing only the result.
    std::cout << "Sum of nums: "
              << std::accumulate(nums.begin(), nums.end(), 0) << '\n';

    // Prints the first fruit in the forward_list fruits, checking if there is any.
    if (!fruits.empty())
      std::cout << "First fruit: " << *fruits.begin() << '\n';

    if (empty.begin() == empty.end())
      std::cout << "forward_list 'empty' is indeed empty.\n";

    std::cout << "-------------- End --------------" << std::endl;
  }

  std::cout << std::endl;

  { // empty()
    std::cout << "-------------- empty() test --------------" << std::endl;

    ForwardList<int> numbers;
    std::cout << std::boolalpha;
    std::cout << "Initially, numbers.empty(): " << numbers.empty() << '\n';

    numbers.push_front(42);
    numbers.push_front(13317);
    std::cout << "After adding elements, numbers.empty(): " << numbers.empty()
              << '\n';

    std::cout << "-------------- End --------------" << std::endl;
  }

  std::cout << std::endl;

  { // clear()
    std::cout << "-------------- clear() test --------------" << std::endl;

    ForwardList<int> container{1, 2, 3};

    auto print = [](const int& n) { std::cout << " " << n; };

    std::cout << "Before clear:";
    std::for_each(container.begin(), container.end(), print);
    std::cout << '\n';

    std::cout << "Clear\n";
    container.clear();

    std::cout << "After clear:";
    std::for_each(container.begin(), container.end(), print);
    std::cout << '\n';

    std::cout << "-------------- End --------------" << std::endl;
  }

  std::cout << std::endl;

  { // insert_after(...)
    std::cout << "-------------- insert_after(...) test --------------"
              << std::endl;

    ForwardList<std::string> words{"the", "frogurt", "is", "also", "cursed"};
    std::cout << "words: " << words << '\n';

    // insert_after (2)
    auto beginIt = words.begin();
    words.insert_after(beginIt, "strawberry");
    std::cout << "words: " << words << '\n';

    // insert_after (3)
    auto anotherIt = beginIt;
    ++anotherIt;
    anotherIt = words.insert_after(anotherIt, 2, "strawberry");
    std::cout << "words: " << words << '\n';

    // insert_after (4)
    std::vector<std::string> V = {"apple", "banana", "cherry"};
    anotherIt = words.insert_after(anotherIt, V.begin(), V.end());
    std::cout << "words: " << words << '\n';

    // insert_after (5)
    words.insert_after(anotherIt, {"jackfruit", "kiwifruit", "lime", "mango"});
    std::cout << "words: " << words << '\n';

    std::cout << "-------------- End --------------" << std::endl;
  }

  std::cout << std::endl;

  { // emplace_after(...)
    std::cout << "-------------- emplace_after(...) test --------------"
              << std::endl;

    struct Sum {
      std::string remark;
      int sum;

      Sum(std::string remark, int sum)
          : remark{std::move(remark)}, sum{sum} {}

      void print() const {
        std::cout << remark << " = " << sum << '\n';
      }
    };

    ForwardList<Sum> list;

    auto iter = list.begin();
    std::string str{"1"};
    for (int i{1}, sum{1}; i != 10; sum += i) {
      iter = list.emplace_after(iter, str, sum);
      ++i;
      str += " + " + std::to_string(i);
    }

    for (const Sum& s : list) s.print();

    std::cout << "-------------- End --------------" << std::endl;
  }

  std::cout << std::endl;

  { // erase_after(...) && pop_front()
    std::cout
        << "-------------- erase_after(...) && pop_front() test --------------"
        << std::endl;

    ForwardList<int> l = {1, 2, 3, 4, 5, 6, 7, 8, 9};

    //    l.erase( l.begin() ); // ERROR: No function erase

    l.pop_front(); // Removes first element

    for (auto n : l) std::cout << n << " ";
    std::cout << '\n';

    auto fi = std::next(l.begin());
    auto la = std::next(fi, 3);

    l.erase_after(fi, la);

    for (auto n : l) std::cout << n << " ";
    std::cout << '\n';

    std::cout << "-------------- End --------------" << std::endl;
  }

  std::cout << std::endl;

  { // push_front(...)
    std::cout << "-------------- push_front(...) test --------------"
              << std::endl;

    ForwardList<std::string> letters;

    letters.push_front("abc");
    std::string s{"def"};
    letters.push_front(std::move(s));

    std::cout << "std::forward_list `letters` holds: ";
    for (auto&& e : letters) std::cout << std::quoted(e) << ' ';

    std::cout << "\nMoved-from string `s` holds: " << std::quoted(s) << '\n';

    std::cout << "-------------- End --------------" << std::endl;
  }

  std::cout << std::endl;

  { // swap()
    std::cout << "-------------- swap() test --------------" << std::endl;

    ForwardList<int> a1{1, 2, 3}, a2{4, 5};

    auto it1 = std::next(a1.begin());
    auto it2 = std::next(a2.begin());

    int& ref1 = a1.front();
    int& ref2 = a2.front();

    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2
              << '\n';
    a1.swap(a2);
    std::cout << a1 << a2 << *it1 << ' ' << *it2 << ' ' << ref1 << ' ' << ref2
              << '\n';

    std::cout << "-------------- End --------------" << std::endl;
  }

  return 0;
}
