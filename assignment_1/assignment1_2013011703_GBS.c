#include<stdio.h>
#include <math.h>
#define INF 10000
#define PASS 6

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define MIN(a,b) (a)>(b)?(b):(a)

struct N {
	int xpos;
	int ypos;
	int heuri;
	int val;
	int pre;
};

struct N Queue[INF];

int size = 0;

int heuri_min;

int row;
int col;

int time = -1;

struct N N[500][500];
int H[500][500] = { 0, };

void swap(struct N* a, struct N * b) {
	struct N tmp;

	tmp = *a;
	*a = *b;
	*b = tmp;
}

void MinHeapify(int i) {
	int smallest;
	if ((2 * i <= size) && (Queue[2 * i].heuri < Queue[i].heuri)) {
		smallest = 2 * i;
	}
	else {
		smallest = i;
	}

	if ((2 * i + 1 <= size) && (Queue[2 * i + 1].heuri < Queue[smallest].heuri)) {
		smallest = 2 * i + 1;
	}

	if (smallest != i) {
		swap(&Queue[i], &Queue[smallest]);
		MinHeapify(smallest);
	}
}

void BuildHeap() {
	int i;

	for (i = (size / 2); i > 0; i--) {
		MinHeapify(i);
	}
}


void Enqueue(struct N N) {
	Queue[++size] = N;
}


struct N Extract_Min() {
	struct N min;

	min = Queue[1];
	Queue[1] = Queue[size--];
	MinHeapify(1);

	return min;
}

struct N Dequeue() {
	time++;
	BuildHeap();
	return Extract_Min();
}



void cal(int y, int x) {
	int i, j;
	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (N[i][j].val == 2) {
				H[i][j] = MIN(H[i][j], abs(i - y) + abs(j - x));
			}
		}
	}
}


void Make_heuristic() {
	int i, j;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (N[i][j].val == 4) {
				cal(i, j);
			}
		}
	}

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			N[i][j].heuri = H[i][j];
		}
	}
}

void Optimal_Pass(int x, int y) {
	while (N[y][x].val != 3) {
		switch (N[y][x].pre) {
		case UP:
			N[y][x].val = 5;
			y -= 1;
			break;
		case DOWN:
			N[y][x].val = 5;
			y += 1;
			break;
		case RIGHT:
			N[y][x].val = 5;
			x += 1;
			break;
		case LEFT:
			N[y][x].val = 5;
			x -= 1;
			break;
		}

	}
}

void Erase_Pass() {
	int i, j;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (N[i][j].val == PASS)
				N[i][j].val = 2;
		}
	}
}


void GBS(int x, int y) {
	struct N tmp;
	int cnt = 0;
	Enqueue(N[y][x]);

	while (size != 0) {
		tmp = Dequeue();
		cnt += 1;

		if (N[tmp.ypos + 1][tmp.xpos].val == 2) {
			Enqueue(N[tmp.ypos + 1][tmp.xpos]);
			N[tmp.ypos + 1][tmp.xpos].val = PASS;
			N[tmp.ypos + 1][tmp.xpos].pre = UP;
		}
		if (N[tmp.ypos - 1][tmp.xpos].val == 2) {
			Enqueue(N[tmp.ypos - 1][tmp.xpos]);
			N[tmp.ypos - 1][tmp.xpos].val = PASS;
			N[tmp.ypos - 1][tmp.xpos].pre = DOWN;
		}
		if (N[tmp.ypos][tmp.xpos + 1].val == 2) {
			Enqueue(N[tmp.ypos][tmp.xpos + 1]);
			N[tmp.ypos][tmp.xpos + 1].val = PASS;
			N[tmp.ypos][tmp.xpos + 1].pre = LEFT;

		}
		if (N[tmp.ypos][tmp.xpos - 1].val == 2) {
			Enqueue(N[tmp.ypos][tmp.xpos - 1]);
			N[tmp.ypos][tmp.xpos - 1].val = PASS;
			N[tmp.ypos][tmp.xpos - 1].pre = RIGHT;
		}

		if (N[tmp.ypos + 1][tmp.xpos].val == 4 ||
			N[tmp.ypos - 1][tmp.xpos].val == 4 ||
			N[tmp.ypos][tmp.xpos + 1].val == 4 ||
			N[tmp.ypos][tmp.xpos - 1].val == 4) {
			Optimal_Pass(tmp.xpos, tmp.ypos);
			Erase_Pass();
			break;
		}
	}
}



void Print() {
	int i, j;
	int count = 0;

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			printf("%d", N[i][j].val);
			if (N[i][j].val == 5)
				count++;
		}
		printf("\n");
	}
	printf("---\n");

	printf("length = %d\n", count);
	printf("time = %d\n", time);
}


int main() {
	int i, j;
	int startx, starty;

	freopen("input.txt", "r", stdin);
	freopen("output.txt", "w", stdout);



	scanf("%d %d", &row, &col);

	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			scanf("%d", &N[i][j].val);
			if (N[i][j].val == 3) {
				starty = i;
				startx = j;
			}
			N[i][j].xpos = j;
			N[i][j].ypos = i;
		}
	}


	for (i = 0; i < row; i++) {
		for (j = 0; j < col; j++) {
			if (N[i][j].val == 2) {
				H[i][j] = INF;
			}
		}
	}

	Make_heuristic();
	GBS(startx, starty);
	Print();
}