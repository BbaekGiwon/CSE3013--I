#include "Str.h"
#include <iostream>
#include <cstdlib>
#include <cstring>

using namespace std;

//Str생성자, 변수 leng+1만큼 메모리를 할당한다.
Str::Str(int leng) {
	//leng이 0보다 작을 때 오류를 출력하고 프로그램을 종료한다.
	if(leng<0) {
		cout << "Error: length is negative\n";
		exit(1);
	}
	//오류가 없을 시 leng+1만큼 str에 메모리를 할당 및 len에는 str에 저장가능한 char 개수를 할당한다.
	else {
		str= new char[leng+1];
		len=leng;
	}
}

//Str생성자, string을 입력받으면 string의 길이만큼 메모리를 할당하고 저장한다.
Str::Str(const char *neyong) {
	//len에 입력받은 string의 길이를 저장한다.
	len=strlen(neyong);
	//str에 len+1만큼의 메모리를 할당한다.
	str= new char[len+1];
	//str에 입력받은 string을 저장한다.
	strcpy(str,neyong);
}

//Str소멸자, Str을 소멸시킨다.
Str::~Str() {
	//str에 할당된 메모리를 지운다.
	delete [] str;
}

//Str의 길이를 반환하는 함수
int Str::length(void) {
	//Str의 길이가 저장된 len을 반환한다.
	return len;
}

//Str의 저장된 내용을 반환하는 함수
char* Str::contents(void) {
	//Str의 내용이 저장된 str을 반환한다.
	return str;
}

//다른 Str과 내용을 비교하는 함수
int Str::compare(class Str &a) {
	//strcmp를 활용하여 각 Str에 저장된 str값(내용)을 비교한 값을 반환한다.
	return strcmp(str, a.str);
}

//다른 string과 내용을 비교하는 함수
int Str::compare(const char *a) {
	//strcmp를 활용하여 str과 a를ㄹ 비교해 그 값을 반환한다.
	return strcmp(str, a);
}

//Str뒤에 =이 나왔을 때, =뒤에 있는 string을 대입하는 함수이다.
void Str::operator=(const char *a) {
	//대입하기 때문에 그 전의 메모리는 지워준다.
	delete [] str;
	//대입하는 string의 길이를 len에 저장한다.
	len=strlen(a);
	//str에 len+1만큼 메모리를 할당한다.
	str=new char[len+1];
	//str에 대입하는 string을 저장한다.
	strcpy(str,a);
}

//Str뒤에 =이 나왔을 때, =뒤에 있는 Str을 대입하는 함수이다.
void Str::operator=(class Str &a) {
	//대입하기 때문에 그 전의 메모리는 지워준다.
	delete [] str;
	//대입하는 Str의 str 길이를 len에 저장한다.
	len=strlen(a.str);
	//str에 len+1만큼 메모리를 할당한다.
	str=new char[len+1];
	//str에 대입하는Strdml str을 저장한다.
	strcpy(str,a.str);
}
	
