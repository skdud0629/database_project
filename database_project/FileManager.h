// 파일 관리자 : 파일에 쓰기, 파일 삭제 역할 수행 헤더 파일
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