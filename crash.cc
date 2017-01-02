#include <stdio.h>

void Func() {
  volatile int x = 42;
  const char* y = "weewaa";
  *(volatile int*)3 = 333;
}

int main() {
  Func();
  getchar();
}
