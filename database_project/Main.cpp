// Main 파일 : 전체 프로그램 실행
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include "Split.h"
#include "Map.h"
#include "FileManager.h"
#include "Merge.h"

using namespace std;

int main() {
	int fileNum = 0;
	string fileName;
	FileManager* fileManager = new FileManager();

	cout << "filename : ";
	cin >> fileName;
	cout << endl;

	cout << "============[full sentence]=============" << endl;
	fileManager->inputFile(fileName);
	cout << "========================================" << endl;

	cout << "Split Start" << endl;
	Split* split = new Split(fileNum);
	fileNum = split->wordSplit(fileName);
	cout << "Split End" << endl;

	cout << "Merge Start" << endl;
	Merge* merge = new Merge(fileNum);
	fileNum = merge->fileMerge();
	cout << "Merge End" << endl;
	cout << endl;

	cout << "=============[word count]============" << endl;
	fileNum = merge->wordCount();
	cout << "=====================================" << endl;
	
	return 0;
}