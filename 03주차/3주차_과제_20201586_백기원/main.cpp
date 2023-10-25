#include "Str.h"
#include <iostream>

using namespace std;

int main(void) {
	//b라는 Str을 선언해 "i love you!"라는 내용으로 초기화 해주었다.
	Str b("i love you!");
	// a라는 Str을 선언해 "I'm a girl"라는 내용으로 초기화 해주었다.
	Str a("I'm a girl");
	//a의 내용을 출력한다. 출력 내용은 "I'm a girl"이다. 
	cout << a.contents();
	//a에 "I'm a boydddd\n"을 대입한다.
	a="I'm a boydddd\n";
	//a의 내용을 출력한다. 출력 내용은 "I'm a boydddd\n"이다.
	cout << a.contents();
	//a의 내용을 string "I'm a boydddd"와 비교한다. 10번째 원소에서 불일치 하므로 출력값은 10이다.
	cout << a.compare("I'm a boydddd") << endl; 
	//a의 길이를 출력한다. 출력값은 14이다.
	cout << a.length() << endl;
	//a에 Str b를 대입한다.
	a=b;
	//a의 내용을 출력한다. 출력 내용은 "i lvoe you!"이다.
	cout << a.contents() << endl;
	return 0;
}
