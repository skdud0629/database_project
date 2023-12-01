#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Map.h"
#include "FileManager.h"
#include "Split.h"

Split::Split(int fileNum) {
	// 생성자
	this->fileNum = fileNum;
	this->fileManager = new FileManager();
}

Split::~Split() {
	// 소멸자
}

void Split::fileSplit(string filename) {
	// 파일 분할(멀티 스레드 사용 시 구현)
}

int Split::wordSplit(string filename) {
	// 단어 분할 실행
	string str;
	int index = 0;

	fs.open("./folder/" + filename + ".txt", ios::in);

	// 파일에서 한 줄씩 읽어서 단어로 분할
	while (!fs.eof()) {
		getline(fs, str);

		vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
		char* token = strtok(&cstr[0], ",.?!: ");

		while (token != nullptr) {
			mapList[index] = new Map(token, 1);
			mapList[index]->setValue();
			index++;
			// 단어가 일정 개수가 되었을 때 파일로 내보냄
			if (index == MAXMAP) {
				wordSort(MAXMAP);
				outputFile.open("./folder/word" + to_string(fileNum + 1) + ".txt", ios::out);
				fileManager->outputFile(&outputFile, mapList, MAXMAP);
				index = -1;
				for (int i = 0; i < MAXMAP; i++) {
					mapList[i] = nullptr;
				}
				outputFile.close();

				fileNum++;
			}
			token = strtok(nullptr, ",.?!: ");
		}
	}
	
	if (index != 0) {
		wordSort(index);
		outputFile.open("./folder/word" + to_string(fileNum + 1) + ".txt", ios::out);
		fileManager->outputFile(&outputFile, mapList, index);
		outputFile.close();
		fileNum++;
	}

	return fileNum;
}

void Split::wordSort(int index) {
	// 단어를 사전 순으로 정렬
	for (int i = 0; i < index; i++) {
		for (int j = i + 1; j < index; j++) {
			if (mapList[i]->getKey() > mapList[j]->getKey()) {
				Map* temp = mapList[i];
				mapList[i] = mapList[j];
				mapList[j] = temp;
			}
		}
	}
}