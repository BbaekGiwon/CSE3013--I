#include "header.h"



void merge(int *input, int left, int right, int size) {
        int l, r, k, i;
        int mid = (left + right) /2;
        l=left;
        r=mid+1;
        k=left;
        int temp[size];
        
        while(l<=mid &&r<=right) {
                temp[k++] = input[l] <= input[r] ? input[l++] : input[r++];
        }
        
        if(l>mid) {
                for(i=r; i<=right; i++) {
                        temp[k++] = input[i];
                }
        }
        else {
                for(i=l; i<=mid; i++) {
                        temp[k++] = input[i];
                }
        }
        
        for(i=left; i<=right; i++) {
                input[i] = temp[i];
        }
}

