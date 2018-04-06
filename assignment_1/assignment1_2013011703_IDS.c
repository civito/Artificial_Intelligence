#include<stdio.h>

#define INF 10000

int row;
int col;
int goallev = 1;
int time = 0;

int N[500][500] = { 0, };

int maze(int x, int y, int level) {
	time++;

	if (N[y][x] == 4)
		return 1;

	if (N[y][x] == 2)
		N[y][x] = 5;

	if (level == goallev)
		return -1;

	if (x + 1 <= col - 1 && N[y][x + 1] % 2 == 0) {
		if (maze(x + 1, y, level + 1) == 1)
			return 1;
		else
			N[y][x + 1] = 2;
	}
	if (x - 1 >= 0 && N[y][x - 1] % 2 == 0) {
		if (maze(x - 1, y, level + 1) == 1)
			return 1;
		else
			N[y][x - 1] = 2;
	}
	if (y + 1 <= row - 1 && N[y + 1][x] % 2 == 0) {
		if (maze(x, y + 1, level + 1) == 1)
			return 1;
		else
			N[y + 1][x] = 2;
	}
	if (y - 1 >= 0 && N[y - 1][x] % 2 == 0) {
		if (maze(x, y - 1, level + 1) == 1)
			return 1;
		else
			N[y - 1][x] = 2;
	}

	return -1;


}

void Print() {
	int i, j;
	int count = 0;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			printf("%d", N[i][j]);
			if (N[i][j] == 5) {
				count++;
			}
		}
		printf("\n");
	}
	printf("---\n");

	printf("length = %d\n", count);
	printf("time = %d\n", time);
}


int main() {
	int i, j;
	int starty, startx;
	int cnt = 0;
	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);

	scanf("%d %d", &row, &col);

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			scanf("%d", &N[i][j]);
			if (N[i][j] == 3) {
				starty = i;
				startx = j;
			}
		}
	}

	for (goallev = 1; goallev < INF; goallev++) {
		if (maze(startx, starty, 1) == 1) {
			Print();
			break;
		}

	}
}