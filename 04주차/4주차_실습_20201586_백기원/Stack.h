#include "LinkedList.h"


//1. 템플릿 클래스로 확장해야함
template<class T>
//2. Stack형식으로 Delete 함수 재정의해야함
//주의: first, current_size는 class의 멤버 변수이기 때문에 this 포인터를 사용하여 가져와야함

//LinkedList class를 상속받음
class Stack : public LinkedList<T> {
	public:
		//LinkedList와 달리 Stack 은 LIFO이므로 Delete함수를 재정의하여준다.
		bool Delete (T &element){
			//first가 0이면 false반환
			if(this->first==0) {
				return false;
			}

			// LinkedList와 달리 Stack은 current가 가리키는 곳을 삭제
			Node<T> *current = this->first;
			//first가 가르키는 노드를 하나 올며준다.
			this->first=this->first->link;
			//element 변수에 지우는 내용을 저장한다.
			element = current->data;
			//제일 나중에 들어온 current 데이터를 삭제한다.
			delete current;
			//하나 삭제했으므로 크기를 1줄여준다.
			this->current_size--;
			
			return true;
			}
};
