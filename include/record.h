#ifndef RECORD_H
#define RECORD_H

struct Record{
	char name[100],durationText[100];
	int duration;

	void assign(Record rec);
};

void writeData(char path[]);

void initRecord(char path[],int neutralDuration);

void insertRecord(int pos,char name[],char durationText[],int duration);

void renderRecord();

#endif
