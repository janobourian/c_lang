#include <stdio.h>

int main(void) {
  int *ptr = nullptr;
  if (ptr == nullptr) {
    printf("C23 nullptr is supported.\n");
  }
  return 0;
}
