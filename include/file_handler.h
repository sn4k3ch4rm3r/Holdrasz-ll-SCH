#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>

typedef struct Score {
	int score;
	char name[15];
	double time;
	double fuel;
	double quality;
} Score;

void append_score(Score *score);
int read_scores(Score **scores);
#endif