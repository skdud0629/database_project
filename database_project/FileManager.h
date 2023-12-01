// ���� ������ : ���Ͽ� ����, ���� ���� ���� ���� ��� ����
#include <fstream>
#include <string>
#include "Map.h"

#ifndef FILEMANAGER_H
#define FILEMANAGER_H

using namespace std;

class FileManager 
{
private :
	fstream file;
	string filename;
public :
	FileManager();
	~FileManager();

	void inputFile(string firstFile);
	void outputFile(fstream *fs, string str);
	void outputFile(fstream *fs, Map* map[], int size);
	void deleteFile(int fileNum);
};

#endif FILEMANAGER_H