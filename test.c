#include <stdio.h>
#include <unistd.h>

int main() {

	for (int i = 0; i < 50; i++) {
		printf("I am a program\n");
		nice(10);
	}
	return 0;
}
