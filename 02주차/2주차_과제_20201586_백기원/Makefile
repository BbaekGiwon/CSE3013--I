#gcc 컴파일러를 이용한다.
cc=gcc
#컴파일러 옵션을 설정한다. -W는 오류문자 출력, -g는 디버깅이 가능하게 컴파일하는 것이다.
cflags= -W -g
#실행파일의 이름을 설정한다. 여기서는 ‘main’으로 설정되었다.
target=main
#실행파일을 만들기위해 필요한 오브젝트 파일들을 정의한 것이다.
objects=main.o cal_page.o find_str.o
#실행파일을 만들기 위해 어떤 오브젝트 파일이 필요한지, 어떤 컴파일러를 사용할지, 어떤 컴파일 옵션을 적용할지 정한 것이다.
$(target) : $(objects)
	$(cc) $(cflags) -o  $(target) $(objects)
#브젝트 파일은 Header.h 파일을 참조한다는 것을 의미한다. 
$(objects) : header.h
#.PHONY는 해당 규칙이 실행될 때 파일이름과 관계없이 실행한다는 것을 의미한다. 즉 clean이라는 규칙을 선언한 것이다.
.PHONY : clean
#clean 규칙을 상술하는 문장이다. clean를 실행하게 되면 생성된 traget파일과 objects 파일을 삭제한다는 것을 의미한다.
clean : 
	rm $(target) $(objects)
