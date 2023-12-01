// Split.h : 문자열을 단어로 분리하여 Map 객체로 생성
#include <string>
#include "Map.h"
#include "ConcurrentQueue.h"
#include "Semaphore.h"
#include "FileManager.h"

#ifndef SPLIT_H
#define SPLIT_H

#define MAXMAP 1000	// 단어의 개수

using namespace std;

class Split
{
private:
	SemaPhore sema;
	//Map* mapList[MAXMAP];
	//ConcurrentPriorityQueue<Map> mapList;
	FileManager* fileManager;
	//fstream fs;
	//fstream outputFile;
public:
	Split();
	~Split();

	int fileSplit(int strNum, string filename);
	int wordSplit(int threadNum, string filename);
	void wordSort(Map*** mapList, int index);
};

#endif SPLIT_H