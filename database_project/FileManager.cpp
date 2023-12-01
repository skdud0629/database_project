// 파일 관리자 정의
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "FileManager.h"
#include "Map.h"

using namespace std;

FileManager::FileManager() {
	// 생성자
	filename = "word";
}

FileManager::~FileManager() {
	// 소멸자
}

void FileManager::inputFile(string firstFile) {
	string str;
	file.open("./folder/" + firstFile + ".txt", ios::in);

	while (!file.eof()) {
		getline(file, str);
		cout << str << endl;
	}
}

void FileManager::outputFile(fstream *fs, string str) {
	// 파일 내보내기(문자열 형식일 경우)
	*fs << str << endl;
}

void FileManager::outputFile(fstream *fs, Map* map[], int size) {
	// 파일 내보내기
	for (int i = 0; i < size - 1; i++) {
		// 중복되는 단어는 한 줄에 여러 개의 1로 출력
		if (map[i]->getKey().compare(map[i + 1]->getKey()) == 0) {
			map[i + 1]->setCount(map[i]->getCount() + 1);
			map[i + 1]->setValue();
			continue;
		}

		*fs << map[i]->getKey() << ",";
		for (int j = 0; j < map[i]->getCount(); j++) {
			*fs << map[i]->getValue()[j] << ",";
		}
		*fs << endl;
	}

	// map[size - 1]에 대한 처리
	*fs << map[size - 1]->getKey() << ",";
	for (int j = 0; j < map[size - 1]->getCount(); j++) {
		*fs << map[size - 1]->getValue()[j] << ",";
	}
	*fs << endl;
}

void FileManager::deleteFile(int fileNum) {
	// 파일 삭제하기
	filename = "./folder/" + filename + to_string(fileNum) + ".txt";

	int delFile = remove(filename.c_str());

	if (delFile != 0) {
		cout << "[ERROR] failure to delete file" << endl;
		exit(-1);
	}

	filename = "word";
}
