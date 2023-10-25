#include "header.h"
//cal_page는 input만큼의 페이지를 가진 책의 페이지를 이루는 숫자들의 개수를 세서 page배열에 저장하는 함수이다.
void cal_page(int input, int *page) {
	//i는 for문에 사용되는 변수이다.
	int i;
	//모든 페이지를 다 계산해야하기에 1페이지부터 input페이지 까지 반복한다.
	for(i=1; i<=input; i++) {
		//find_str함수를 호출해 그 결과값을 temp에 저장한다.
		int temp=find_str(i);
		//i에 저장된 값을 이루는 모든 숫자를 다 새서 page배열에 센 결과를 반영한다.
		while(temp!=0) {
			//높은 자리부터 작은 자리 순으로 사용된 숫자를 추출한다.
			page[i/temp%10]++;
			temp/=10;
		}
	}
}
			
