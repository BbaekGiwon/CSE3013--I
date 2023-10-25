#include <stdio.h>

int main (void) {
	int i;
	double temp;
	double num;

	for(i=0; i<5; i++) {
		temp=i;
		num=temp/2 +temp;
		printf("num is %lf \n", num);
	}
	return 0;
}
