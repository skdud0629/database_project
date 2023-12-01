// Merge.h : 파일을 병합
#include <string>]
#include <vector>
#include "Map.h"
#include "Semaphore.h"
#include "FileManager.h"

#ifndef MERGE_H
#define MERGE_H

using namespace std;

class Merge
{
private:
	SemaPhore sema;
	Map** mapList;
	FileManager* fileManager;
	//string filename;
	//fstream fs1;
	//fstream fs2;
	//fstream outputFile;
	//int fileNum;
public:
	//Merge(int fileNum);
	Merge();
	~Merge();

	void splitWord(Map* (&mapList)[2], int index, fstream* fs);
	//void splitWord(unique_ptr<Map>& map, fstream* fs);
	int thread_fileMerge(int fileNum, int threadNum);
	int fileMerge(vector<int> fileNums, int threadNum);
	//int fileMerge(int fileNum, int threadNum);
	//int wordCount(int fileNum);
	void wordCount();
};

#endif MERGE_H