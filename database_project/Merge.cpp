#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Map.h"
#include "FileManager.h"
#include "Merge.h"

using namespace std;

Merge::Merge(int fileNum) {
	this->filename = "word";
	this->fileNum = fileNum;
	this->fileManager = new FileManager();
}

Merge::~Merge() {
	// 소멸자(사용 X)
}

void Merge::splitWord(int index, fstream* fs) {
	// ','를 중심으로 단어 자르기
	string word;
	string key;
	int count = 0;

	getline(*fs, word);

	if (word.empty()) {
		mapList[index] = nullptr;
		return;
	}

	if (fs->eof()) {
		mapList[index] = nullptr;
		return;
	}

	vector<char> cstr(word.c_str(), word.c_str() + word.size() + 1);
	char* token = strtok(&cstr[0], ",");
	key = token;

	while (token != nullptr) {
		token = strtok(nullptr, ",");
		if (token != nullptr) {
			count += 1;
		}
	}

	mapList[index] = new Map(key, count);
	mapList[index]->setValue();
}

int Merge::fileMerge() {
	// 파일 병합
	string str;
	Map* outMap[1];

	for (int i = 0; i < fileNum - 1; i += 2) {
		fs1.open("./folder/" + filename + to_string(i + 1) + ".txt", ios::in);
		fs2.open("./folder/" + filename + to_string(i + 2) + ".txt", ios::in);
		outputFile.open("./folder/" + filename + to_string(fileNum + 1) + ".txt", ios::out);

		splitWord(0, &fs1);
		splitWord(1, &fs2);

		while (1) {
			int index = 0;

			// 둘 중 하나의 파일이 모두 작성된 후일 경우
			if (mapList[0] == nullptr) {
				outMap[0] = new Map(mapList[1]->getKey(), mapList[1]->getCount());
				outMap[0]->setValue();
				fileManager->outputFile(&outputFile, outMap, 1);

				while (!fs2.eof()) {
					getline(fs2, str);
					fileManager->outputFile(&outputFile, str);
				}

				break;
			}
			else if (mapList[1] == nullptr) {
				outMap[0] = new Map(mapList[0]->getKey(), mapList[0]->getCount());
				outMap[0]->setValue();
				fileManager->outputFile(&outputFile, outMap, 1);

				while (!fs1.eof()) {
					getline(fs1, str);
					fileManager->outputFile(&outputFile, str);
				}
				break;
			}
			else {
				// 두 파일 첫 번째 단어가 같으면 각 개수 더해서 출력
				if (mapList[index]->getKey().compare(mapList[index + 1]->getKey()) == 0) {
					outMap[0] = new Map(mapList[index]->getKey(),
						mapList[index]->getCount() + mapList[index + 1]->getCount());
					outMap[0]->setValue();
					fileManager->outputFile(&outputFile, outMap, 1);
					splitWord(0, &fs1);
					splitWord(1, &fs2);
				}
				// fs1 파일이 사전 순으로 앞일 때 출력 후 불러오기
				else if (mapList[index]->getKey() < mapList[index + 1]->getKey()) {
					outMap[0] = new Map(mapList[index]->getKey(), mapList[index]->getCount());
					outMap[0]->setValue();
					fileManager->outputFile(&outputFile, outMap, 1);
					splitWord(0, &fs1);
				}
				// fs2 파일이 사전 순으로 앞일 때 출력 후 불러오기
				else {
					outMap[0] = new Map(mapList[index + 1]->getKey(), mapList[index + 1]->getCount());
					outMap[0]->setValue();
					fileManager->outputFile(&outputFile, outMap, 1);
					splitWord(1, &fs2);
				}
			}			
		}

		fileNum += 1;
		
		fs1.close();
		fs2.close();
		outputFile.close();

		fileManager->deleteFile(i + 1);
		fileManager->deleteFile(i + 2);
	}

	return fileNum;
}

int Merge::wordCount() {
	// 단어 개수 세기
	fs1.open("./folder/" + filename + to_string(fileNum) + ".txt", ios::in);
	outputFile.open("./folder/" + filename + to_string(fileNum + 1) + ".txt", ios::out);

	splitWord(0, &fs1);

	while (mapList[0] != nullptr) {
		cout << mapList[0]->getKey() << "," << mapList[0]->getCount() << endl;
		outputFile << mapList[0]->getKey() << "," << mapList[0]->getCount() << endl;
		splitWord(0, &fs1);
	}

	fs1.close();
	outputFile.close();

	fileManager->deleteFile(fileNum);
	fileNum++;

	return fileNum;
}