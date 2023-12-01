// Merge.h : 파일을 병합
#include <string>
#include "Map.h"
#include "FileManager.h"

#ifndef MERGE_H
#define MERGE_H

using namespace std;

class Merge
{
private :
	Map* mapList[2];
	FileManager* fileManager;
	string filename;
	fstream fs1;
	fstream fs2;
	fstream outputFile;
	int fileNum;
public :
	Merge(int fileNum);
	~Merge();

	void splitWord(int index, fstream* fs);
	int fileMerge();
	int wordCount();
};
#endif MERGE_H