#include "header.h"

int main(void) {
        int n=5, i;
        
	printf("3 4 2 1 5 will be sorted by mergesort   ");
        int input[5]={3,4,2,1,5};
        
        mergesort(input, 0, n-1, n);
        
	printf("The result is: ");
        for(i=0; i<n; i++) {
                printf("%d ", input[i]);
        }
	printf("\n");
        
        
        return 0;
}
