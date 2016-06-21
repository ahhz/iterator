#include <blink\iterator\range_algebra.h>
#include <blink\iterator\range_algebra_transform.h>

#include <string>
#include <iostream>
#include <vector>

using namespace blink::iterator;

int my_plus(int a, int b)
{
  return a + b;
}

int main()
{
  std::vector<int> a = { 1, 2, 3, 4, 5, 6 };
  
  auto rat = make_range_algebra_transform(std::plus < > {}, range_algebra(a), range_algebra(a));
  
  for (auto&& i : rat)
  {
    std::cout << i << std::endl;
  }
 
  std::vector<int> b = { 100, 200, 300, 400, 500, 600 };
  auto fun = [](int a, int b) {return 10 * a + b; };
  
  auto ra = range_algebra(a);
  auto rb = range_algebra(b);

  auto sum = make_transform_range(my_plus, ra, rb);        // function
  auto sum1 = make_transform_range(fun, ra, rb);           // lambda
  auto sum2 = make_transform_range(std::plus<>{}, ra, rb); // function object
  auto sum3 = ra + rb ;                                  // operator
  
  for (auto&& i : sum)
  {
    std::cout << i << std::endl;
  }

  for (auto&& i : sum1)
  {
    std::cout << i << std::endl;
  }

  for (auto&& i : sum2)
  {
    std::cout << i << std::endl;
  }
  for (auto&& i : sum3)
  {
    std::cout << i << std::endl;
  }

  std::cin.get();
  return 0;
}

