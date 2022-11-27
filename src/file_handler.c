#include <stdio.h>
#include "file_handler.h"

void append_score(Score *score) {
	FILE *fp = fopen("leaderboard.csv", "a");
	fprintf(fp, "%d;%s;%lf;%lf;%lf\n", score->score, score->nev, score->time, score->fuel, score->quality);
	fclose(fp);
}
