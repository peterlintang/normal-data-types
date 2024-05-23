
#include <stdio.h>
#define PRINT_GENERIC_INT(expr) \
  _Generic((expr), \
           short: printf("short: %hd\n", (short)expr), /* 如果表达式是short类型 */ \
           int: printf("int: %d\n", (int)expr), /* 如果表达式是int类型 */ \
           long: printf("long: %ld\n", (long)expr), /* 如果表达式是long类型 */ \
           default: printf("other type: %ld\n", (long)expr)) /* 如果表达式是其他类型 */
int main() {
  short s = 10;
  int i = 20;
  long l = 30;
  PRINT_GENERIC_INT(s); // 输出: short: 10
  PRINT_GENERIC_INT(i); // 输出: int: 20
  PRINT_GENERIC_INT(l); // 输出: long: 30
  return 0;
}

