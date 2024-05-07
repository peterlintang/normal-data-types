
#include <stdio.h>

int main(int argc, char *argv[])
{
	printf("%+19.5f\n", -1.32413);	 // you dui qi
	printf("%-19.5f\n", -1.32413);	 // zou dui qi
	printf("%#x\n", 0x1234);	 // 
	printf("%019.5f\n", -1.32413);	 // zou dui qi, bu 0
	printf("%+19hhd\n", 1234);	 // 
	printf("%+19hd\n", 1234);	 // 
//	printf("%+19ld\n", 1234);	 // 
	printf("%e\n", 1234.5);	 // 
	printf("%g\n", 1234.5);	 // 
	printf("%a\n", 2.5);	 // 
	return 0;
}

