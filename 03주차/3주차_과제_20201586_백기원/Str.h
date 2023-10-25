//만약 _Str_이 정의 되징 않았다면 정의한다.
#ifndef _Str_
//Str을 정의한다.
#define _Str_

//clas Str을 정의한다.
class Str{
	private:
		char *str;//string의 내용을 저장한다.
		int len;//string의 길이를 저장한다.
	public:
		Str(int leng);//leng이라는 길이가 주어졌을 때 Str에 메모리를 할당한다.
		Str(const char *neyong);//neyong이라는 string이 주어졌을 때 neyyong으로 초기화한다.
		~Str();//소멸자이다. 메모리를 지운다.
		int length(void);//string의 길이를 리턴하는 함수이다.
		char*contents(void);//string의 내용을 리턴하는 함수이다.
		int compare(class Str &a);//a의 내용과 비교한 값을 리턴하는 함수이다.
		int compare(const char *a);//a의 내용과 비교한 값을 리턴하는 함수이다.
		void operator=(const char *a);//string의 값을 대입하는 함수이다.
		void operator=(class Str &a);//Str의 내용을 대입하는 함수이다.
};
//정의 끝
#endif
