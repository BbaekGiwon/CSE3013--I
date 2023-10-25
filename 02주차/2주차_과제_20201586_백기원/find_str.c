#include "header.h"
//자리수를 계산하는 함수이다.
int find_str(int input) {
	//input의 최대값을 일단 temp에 저장해둔다.
	int temp=1000000000;
	//input과 temp가 자릿수가 같아지면 while문의 조건을 불만족하게 된다.
	while(input/temp==0) {
		temp/=10;
	}
	//input과 자릿수가 같아진 temp를 반환한다.
	return temp;
}
