#include <iostream>
#include <fstream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <random>
#include <ctime>

#define MAX_COL 30
#define MAX_ROW 30

using namespace std;

int main(void) {
	int n, m;
	cin >> n >> m;

	fstream fp;
	fp.open("result.maz", ios::out);//result.txt

	//random val
	srand(static_cast<unsigned int>(time(nullptr)));
	default_random_engine engine(rand());
	uniform_int_distribution<int> distribution(0, 1);

	int b[MAX_COL][MAX_ROW]; //block set ID
	char f[MAX_COL * 2 + 1][MAX_ROW * 2 + 1];//what's actually maze look like

	//initialize set ID
	int init = 1;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			b[i][j] = init++;
		}
	}

	//initialize maze
	for (int i = 0; i < n * 2 + 1; i++) {
		if (i % 2 == 0) {
			for (int j = 0; j < m * 2 + 1; j++) {
				if (j % 2 == 0) {
					f[i][j] = '+';
				}
				else {
					f[i][j] = '-';
				}
			}
		}
		else {
			for (int j = 0; j < m * 2 + 1; j++) {
				if (j % 2 == 0) {
					f[i][j] = '|';
				}
				else {
					f[i][j] = ' ';
				}
			}
		}
	}

	//until last row
	for (int i = 0; i < n - 1; i++) {
		//right wall
		for (int j = 0; j < m - 1; j++) {
			int rnd = distribution(engine);
			if (rnd == 1 && b[i][j + 1] != b[i][j]) {
				int temp = b[i][j + 1];
				f[i * 2 + 1][(j + 1) * 2] = ' ';
				for (int k = 0; k < m; k++) {
					if (b[i][k] == temp) {
						b[i][k] = b[i][j];
					}
				}
			}
		}

		//down wall
		vector<int> del_flag;//�ѹ��̶� ������ ������ ����ٰ� id ������ 
		for (int j = 0; j < m; j++) {
			int cnt = 1;//���������� ���� ���� ī��Ʈ 
			for (int k = j + 1; k < m; k++) {
				if (b[i][j] == b[i][k]) {//������ �ִ��� Ȯ�� 
					cnt++;
					break;//������ �����ϸ� �� ���� 
				}
			}

			if (cnt == 1) {//���Ұ� �ϳ��� ���� or �ϳ��� ���� ���� 
				//�׷��� ���͵����� ������ �ִ��� Ȯ�� -> ���࿡ ������ ���� �� ���������� 
				if (find(del_flag.begin(), del_flag.end(), b[i][j]) == del_flag.end()) {
					//�� ���� ������ ���� ��쿡 ������ �����ִ� �� 
					b[i + 1][j] = b[i][j];
					f[(i + 1) * 2][(j) * 2 + 1] = ' ';
					del_flag.push_back(b[i][j]);
					continue;
				}
			}

			//�ϳ��� ���ų� �ϳ��� ������ �ƴϸ� �׳� �������� ������ ���� ����			
			int rnd = distribution(engine);
			if (rnd == 1) {
				b[i + 1][j] = b[i][j];
				f[(i + 1) * 2][(j) * 2 + 1] = ' ';
				del_flag.push_back(b[i][j]);//����� ������ �ִٰ� ���� 
			}
		}
		del_flag.clear();
	}

	//last row
	for (int j = 0; j < m - 1; j++) {
		if (b[n - 1][j + 1] != b[n - 1][j]) {
			int temp = b[n - 1][j + 1];
			f[n * 2 - 1][(j + 1) * 2] = ' ';
			for (int k = 0; k < m; k++) {
				if (b[n - 1][k] == temp) {
					b[n - 1][k] = b[n - 1][j];
				}
			}
		}
	}
	/* //for debug
	for(int i=0; i<n*2+1; i++) {
		for(int j=0; j<m*2+1; j++) {
			cout << f[i][j];
		}
		cout <<"\n";
	}
	for(int i=0; i<n; i++) {
		for(int j=0; j<m; j++) {
			cout << b[i][j]<< " ";
		}
		cout << endl;
	}
	*/

	//make .maz file
	for (int i = 0; i < n * 2 + 1; i++) {
		for (int j = 0; j < m * 2 + 1; j++) {
			fp << f[i][j];
		}
		fp << "\n";
	}
	//cout << cnt;




	return 0;
}