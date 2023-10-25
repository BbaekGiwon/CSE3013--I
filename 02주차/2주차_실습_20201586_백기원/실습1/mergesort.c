#include "header.h"

void mergesort(int *input, int left, int right, int size) {
        if(left==right) return;
        int mid=(left+right) /2;
        mergesort(input, left, mid, size);
        mergesort(input, mid+1, right, size);
        merge(input, left, right, size);
}
