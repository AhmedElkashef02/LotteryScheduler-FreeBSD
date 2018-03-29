#include <stdio.h>
#include <unistd.h>

int gift(int a, int b) {
	return syscall(548, a, b);
}

int main() {

	printf("I am a program\n");
	gift(750,10);
	return 0;
}
