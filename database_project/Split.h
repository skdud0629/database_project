// Split.h : ���ڿ��� �ܾ�� �и��Ͽ� Map ��ü�� ����
#include <string>
#include "Map.h"
#include "FileManager.h"

#ifndef SPLIT_H
#define SPLIT_H

#define MAXMAP 10	// �ܾ��� ����

using namespace std;

class Split
{
private :
	Map* mapList[MAXMAP];
	FileManager* fileManager;
	fstream fs;
	fstream outputFile;
	int fileNum;
public :
	Split(int fileNum);
	~Split();

	void fileSplit(string filename);
	int wordSplit(string filename);
	void wordSort(int index);
};

#endif SPLIT_H