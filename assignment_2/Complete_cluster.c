/*
case 1. Create cluster
case 2. Nothing happen
case 3. Intering cluster
case 4. Merge cluster
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define Word_num 338
#define Pos_num 300
#define Class_num 8

#define Normalization 35


#define Rel (Word_num * (Word_num - 1)) / 2

double threshold; 
int sflag = 0;


struct Word {
	char name[20];
	double pos[Pos_num];
	int idx;
	int cluster;
	int class_num;
};

struct Relation {
	int idx1;
	int idx2;
	double sim;
};

struct Word word[Word_num];
struct Relation relation[Rel];

int S[Word_num][Word_num] = { 0, };
int is_cluster[Word_num] = { 0, };
int cluster = 2;

// for clustering
int cluster_num = 0;
int tmp = -1; 

// for classification
int class_number = 0;

// for calculate Entropie
double C[Class_num + 1] = { 0, };
double mem_num = 0;
double En_sum = 0;



int static compare_cl(const void* a, const void* b)
{
	struct Word *pa = (struct Word *)a;
	struct Word *pb = (struct Word *)b;

	if (pa->cluster < pb->cluster)
		return 1;
	else if (pa->cluster > pb->cluster)
		return -1;
	else
		return 0;
}

int static compare_idx(const void* a, const void* b)
{
	struct Word *pa = (struct Word *)a;
	struct Word *pb = (struct Word *)b;

	if (pa->idx > pb->idx)
		return 1;
	else if (pa->idx < pb->idx)
		return -1;
	else
		return 0;
}

int compare_sim(const void *a, const void *b)
{
	struct Relation *pa = (struct Relation *)a;
	struct Relation *pb = (struct Relation *)b;

	if (pa->sim < pb->sim) {
		return 1;
	}
	else if (pa->sim == pb->sim) {
		return 0;
	}
	else {
		return -1;
	}
}


void clustering() {
	int i, cnt;

	for (i = 0; i < Word_num; i++) {
		word[i].cluster = is_cluster[i];
	}

	qsort(word, Word_num, sizeof(struct Word), compare_cl);

	cnt = 0;
	while (1) {
		if (cnt == Word_num)
			break;

		if (word[cnt].cluster == 0) {
			word[cnt].cluster = ++cluster_num;
		}
		else {
			if (word[cnt].cluster == tmp) {
				word[cnt].cluster = cluster_num;
			}
			else {
				tmp = word[cnt].cluster;
				word[cnt].cluster = ++cluster_num;
			}

		}
		cnt++;
	}

	qsort(word, Word_num, sizeof(struct Word), compare_idx);
}



double Similarity_Cosine(struct Word x, struct Word y) {
	int i;
	double tmp1 = 0;
	double tmp2 = 0;
	double tmp3 = 0;

	for (i = 0; i < Pos_num; i++) {
		tmp1 += (x.pos[i])*(y.pos[i]);
	}

	for (i = 0; i < Pos_num; i++) {
		tmp2 += (x.pos[i])*(x.pos[i]);
	}

	for (i = 0; i < Pos_num; i++) {
		tmp3 += (y.pos[i])*(y.pos[i]);
	}

	return tmp1 / (sqrt(tmp2) * sqrt(tmp3));
}

double Similarity_Euclidean(struct Word x, struct Word y) {
	int i;
	double tmp1 = 0;
	
	for (i = 0; i < Pos_num; i++) {
		tmp1 += (x.pos[i] - y.pos[i]) * (x.pos[i] - y.pos[i]);
	}

	return sqrt(tmp1) / sqrt(Normalization);
}


void print() {
	int i, j;

	freopen("output.txt", "w", stdout);
	for (i = 0; i < Word_num; i++) {
		printf("%s\n", word[i].name);
		
		for (j = 0; j < Pos_num-1; j++) {
			printf("%e,", word[i].pos[j]);
		}
		printf("%.8e\n", word[i].pos[Pos_num - 1]);

		printf("%d\n", word[i].cluster);

	}
}

double logB(double x, double base) {
	return log(x) / log(base);
}

void cal_En() {
	int i;
	for (i = 1; i <= Class_num; i++) {
		if (C[i] > 0) {
			En_sum += ((-1)*(C[i] / mem_num) * (logB((C[i] / mem_num), 2.0)))*(mem_num / Word_num);
		}
	}
}

void Entropie() {
	int i, j, k;

	for (i = 1; i <= cluster_num; i++) {
		mem_num = 0;
		for (k = 1; k <= Class_num; k++) {
			C[k] = 0;
		}

		for (j = 0; j < Word_num; j++) {
			if (word[j].cluster == i) {
				mem_num += 1;
				C[word[j].class_num] += 1;
			}
		}

		cal_En();
	}
}

int main(int argc, char* argv[]) {
	int i, j, k = 0;
	int tmp1, tmp2;
	int cflag; // Cluster flag
	int clnum; // Cluster number;
	int count; // Used for case 1

	int cnt = 0;

	if(argc != 3){
		printf("Error:// usage = ./file similarity threshold\n");
		exit(1);
	}

	if(!strcmp(argv[1],"Euclidean")){
		sflag = 1;
	}else if(!strcmp(argv[1],"Cosine")){
		sflag = 2;
	}else{
		printf("Error:// argv[1] should be [Euclidean / Cosine]\n");
		exit(1);
	}

	threshold = atof(argv[2]);
	
	printf("threshold : %lf\n",threshold);
	

	/*			
			Calculate Similarity
									*/

	freopen("WordEmbedding.txt", "r", stdin);

	for (i = 0; i < Word_num; i++) {
		scanf("%s", word[i].name);
		word[i].idx = i;
		for (j = 0; j < Pos_num - 1; j++) {
			scanf("%lf,", &word[i].pos[j]);
		}
		scanf("%lf", &word[i].pos[Pos_num - 1]);
	}

	for (i = 0; i < Word_num; i++) {
		for (j = i + 1; j < Word_num; j++) {
			relation[k].idx1 = i;
			relation[k].idx2 = j;
			if(sflag == 1){
				relation[k++].sim = Similarity_Euclidean(word[i], word[j]);
			}
			else if(sflag == 2){
				relation[k++].sim = Similarity_Cosine(word[i], word[j]);
			}else{
				printf("Error\n");
				exit(1);
			}
		}
	}

	/*
			Class Classification
										*/


	freopen("WordTopic.txt", "r", stdin);

	char tmp[20];

	cnt = 0;
	for (i = 0; i < Word_num + 20; i++) {
		scanf("%s",tmp);

		if (tmp[0] == '[') {
			class_number += 1;
		}else{
			// not case sensitive 
			for(j = 0; j < Word_num; j++){		
				if (!strcasecmp(word[j].name, tmp)) { 
					word[j].class_num = class_number;
				}
			}
		}
	}
	
	for(i = 0; i < Word_num; i++)
	
	qsort(relation, Rel, sizeof(struct Relation), compare_sim);


	while (relation[cnt].sim >= threshold && cnt < Rel) {

		// initialize.
		cflag = 0;
		clnum = -1;
		count = 0;


		S[relation[cnt].idx1][relation[cnt].idx2] = 1;
		S[relation[cnt].idx2][relation[cnt].idx1] = 1;


		if (is_cluster[relation[cnt].idx1] != 0 && is_cluster[relation[cnt].idx2] != 0
			&& is_cluster[relation[cnt].idx2] != is_cluster[relation[cnt].idx1]) {

			for (i = 0; i < Word_num; i++) { // Merge possible?
				if (is_cluster[i] == is_cluster[relation[cnt].idx1]) {
					for (j = 0; j < Word_num; j++) {
						if (is_cluster[j] == is_cluster[relation[cnt].idx2]) {
							if (S[i][j] != 1) {
								cflag = 1;
							}
						}
					}
				}
			}

			if (cflag == 0) { // can Merge
				tmp1 = is_cluster[relation[cnt].idx1];
				tmp2 = is_cluster[relation[cnt].idx2];
				if (is_cluster[relation[cnt].idx1] > is_cluster[relation[cnt].idx2]) {

					for (i = 0; i < Word_num; i++) {
						if (is_cluster[i] == is_cluster[relation[cnt].idx1]) {
							for (j = 0; j < Word_num; j++) {
								if (is_cluster[j] == is_cluster[relation[cnt].idx2]) {
									S[i][j] = is_cluster[relation[cnt].idx1];
									S[j][i] = is_cluster[relation[cnt].idx1];
								}
							}
						}
					}

					for (i = 0; i < Word_num; i++) {
						if (is_cluster[i] == tmp2) {
							is_cluster[i] = tmp1;
						}
					}

					for (i = 0; i < Word_num; i++) {
						for (j = 0; j < Word_num; j++) {
							if (S[i][j] == tmp2) {
								S[i][j] = tmp1;
							}
						}
					}


				}
				else {
					tmp1 = is_cluster[relation[cnt].idx1];
					tmp2 = is_cluster[relation[cnt].idx2];

					for (i = 0; i < Word_num; i++) {
						if (is_cluster[i] == is_cluster[relation[cnt].idx1]) {
							for (j = 0; j < Word_num; j++) {
								if (is_cluster[j] == is_cluster[relation[cnt].idx2]) {
									S[i][j] = is_cluster[relation[cnt].idx2];
									S[j][i] = is_cluster[relation[cnt].idx2];
								}
							}
						}
					}

					for (i = 0; i < Word_num; i++) {
						if (is_cluster[i] == tmp1) {
							is_cluster[i] = tmp2;
						}
					}

					for (i = 0; i < Word_num; i++) {
						for (j = 0; j < Word_num; j++) {
							if (S[i][j] == tmp1) {
								S[i][j] = tmp2;
							}
						}
					}

				}
			}
		}

		else if (is_cluster[relation[cnt].idx1] != 0 && is_cluster[relation[cnt].idx2] != 0
			&& is_cluster[relation[cnt].idx2] == is_cluster[relation[cnt].idx1]) {

			printf("Error\n");
			return;
		}

		else {

			for (i = 0; i < Word_num; i++) {
				if (S[relation[cnt].idx1][i] > 1) {
					clnum = S[relation[cnt].idx1][i];
					if (S[i][relation[cnt].idx2] != 1) {
						cflag = 1;
					}
					else {
						S[i][relation[cnt].idx2] = -1;
					}
					count++;
				}
			}

			for (i = 0; i < Word_num; i++) {
				if (S[relation[cnt].idx2][i] > 1) {
					clnum = S[relation[cnt].idx2][i];
					if (S[i][relation[cnt].idx1] != 1) {
						cflag = 1;
					}
					else {
						S[i][relation[cnt].idx1] = -1;
					}
					count++;
				}
			}

			if (cflag == 0) {
				if (count == 0) {
					S[relation[cnt].idx1][relation[cnt].idx2] = cluster;
					S[relation[cnt].idx2][relation[cnt].idx1] = cluster;
					is_cluster[relation[cnt].idx1] = cluster;
					is_cluster[relation[cnt].idx2] = cluster++;
				}
				else {
					S[relation[cnt].idx1][relation[cnt].idx2] = clnum;
					S[relation[cnt].idx2][relation[cnt].idx1] = clnum;
					for (i = 0; i < Word_num; i++) {
						for (j = 0; j < Word_num; j++) {
							if (S[i][j] == -1) {
								S[i][j] = clnum;
								S[j][i] = clnum;
							}
						}
					}
					is_cluster[relation[cnt].idx1] = clnum;
					is_cluster[relation[cnt].idx2] = clnum;
				}
			}
			else {
				for (i = 0; i < Word_num; i++) {
					for (j = 0; j < Word_num; j++) {
						if (S[i][j] == -1) {
							S[i][j] = 1;
						}
					}
				}
			}
		}
		
		cnt++;
	}


	clustering();

	Entropie();

	printf("%lf\n", En_sum);

	print();
}
