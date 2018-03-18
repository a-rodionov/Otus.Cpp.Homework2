#include <cassert>
#include <cstdlib>
#include "ip.h"
#include "filter.h"

int main(int argc, char const *argv[])
{
  try
  {
    process(std::cout, std::cin);
  }
  catch (const std::exception &e)
  {
    std::cerr << e.what() << std::endl;
  }
  return 0;
}