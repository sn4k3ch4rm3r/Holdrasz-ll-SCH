#ifndef FILEHANDLER_H
#define FILEHANDLER_H

#include <stdio.h>

typedef struct Score {
	int score;
	char nev[15];
	double time;
	double fuel;
	double quality;
} Score;

void append_score(Score *score);
#endif