#include <stdlib.h>
#include <stdio.h>

#include "file_handler.h"
#include "debugmalloc.h"

void append_score(Score *score) {
	FILE *fp = fopen("leaderboard.csv", "a");
	fprintf(fp, "%d;%lf;%lf;%lf;%s\n", score->score, score->time, score->fuel, score->quality, score->name);
	fclose(fp);
}

int read_scores(Score **scores) {
	FILE *fp = fopen("leaderboard.csv", "r");

	Score *leaderboard = malloc(sizeof(Score) * 10);
	int count = 0;
	int max = 10;
	while(fscanf(fp, "%d;%lf;%lf;%lf;%s\n", &leaderboard[count].score, &leaderboard[count].time, &leaderboard[count].fuel, &leaderboard[count].quality, leaderboard[count].name) == 5) {
		count++;
		if(count == max) {
			max += 10;
			leaderboard = realloc(leaderboard, sizeof(Score) * max);
		}
	}

	fclose(fp);

	for (int i = 0; i < count-1; i++)
	{
		Score *max = leaderboard + i;
		for (int j = i+1; j < count; j++)
		{
			if(leaderboard[j].score > max->score){
				max = leaderboard + j;
			}
		}
		Score tmp = leaderboard[i];
		leaderboard[i] = *max;
		*max = tmp;
	}
	

	*scores = leaderboard;
	return count;
}