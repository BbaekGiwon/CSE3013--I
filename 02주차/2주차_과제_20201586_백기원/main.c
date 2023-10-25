#include "header.h"
//main 함수: 입력을 받고, cal_page 함수를 호출해 계산을 실시하고 출력한다.
int main(void) {
	//t는 테스트 케이스의 개수, i는 for문에서 사용할 변수이다.
	int t, i;
	//t의 값을 입력받는다.
	scanf("%d", &t);
	//테스트 케이스의 개수 만큼 반복해준다.
	for(i=0; i<t; i++) {
		//input은 책의 페이지를 저장, page 배열은 각 숫자의 개수를 저장한다. page[0]은 0의 개수를 의미한다.
		int input, page[10]={0,};
		//input을 입력받는다.
		scanf("%d", &input);
		//cal_page함수를 호출하여 계산을 실시한다. 계산결과는 page배열에 저장된다.
		cal_page(input, page);
		//for문에 활용될 변수이다.
		int j;
		//page 배열에 저장된 값을 출력한다.
		for(j=0; j<10; j++) {
			printf("%d ", page[j]);
		}
		printf("\n");
	}
	return 0;
}
