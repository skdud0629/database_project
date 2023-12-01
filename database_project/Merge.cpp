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


Merge::Merge() : sema(10) {
	//this->fileNum = fileNum;
	this->fileManager = new FileManager();
}

Merge::~Merge() {
	// 소멸자(사용 X)
}

void Merge::splitWord(Map* (&mapList)[2], int index, fstream* fs) {
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
/*
void Merge::splitWord(unique_ptr<Map>& map, fstream* fs) {
	// ','를 중심으로 단어 자르기
	string word;
	string key;
	int count = 0;

	getline(*fs, word);

	if (word.empty()) {
		map = nullptr;
		return;
	}

	if (fs->eof()) {
		map = nullptr;
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

	map = make_unique<Map>(key, count);
	map->setValue();
}*/

int Merge::thread_fileMerge(int fileNum, int threadNum) {
	// 스레드용 파일 병합
	fstream fs1;
	fstream fs2;
	fstream outputFile;
	string str;
	Map* mapList[2];
	Map* outMap[1];
	//unique_ptr<Map*> mapList[2];
	//unique_ptr<Map*> outMap[1];

	for (int i = 0; i < fileNum; i += 2) {
		fs1.open("./folder/word" + to_string(i) + "_thread" + to_string(threadNum) + ".txt", ios::in);
		fs2.open("./folder/word" + to_string(i + 1) + "_thread" + to_string(threadNum) + ".txt", ios::in);

		if (!fs1.is_open() && !fs2.is_open()) {
			cout << "[ERROR] file is not open <Mergge::fileMerge>" << endl;
			exit(-1);
		}

		if (!fs1.is_open()) {
			fs1.close();
			fs2.close();
			break;
		}
		if (!fs2.is_open()) {
			fs1.close();
			fs2.close();
			break;
		}

		outputFile.open("./folder/word" + to_string(fileNum) + "_thread" + to_string(threadNum) + ".txt", ios::out);

		//sema.wait();

		splitWord(mapList, 0, &fs1);
		splitWord(mapList, 1, &fs2);

		//splitWord(mapList[0], &fs1);
		//splitWord(mapList[1], &fs2);

		while (1) {
			int index = 0;

			if (mapList[0] == nullptr && mapList[1] == nullptr) {
				break;
			}
			// 둘 중 하나의 파일이 모두 작성된 후일 경우
			else if (mapList[0] == nullptr) {
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
					splitWord(mapList, 0, &fs1);
					splitWord(mapList, 1, &fs2);
				}
				// fs1 파일이 사전 순으로 앞일 때 출력 후 불러오기
				else if (mapList[index]->getKey() < mapList[index + 1]->getKey()) {
					outMap[0] = new Map(mapList[index]->getKey(), mapList[index]->getCount());
					outMap[0]->setValue();
					fileManager->outputFile(&outputFile, outMap, 1);
					splitWord(mapList, 0, &fs1);
				}
				// fs2 파일이 사전 순으로 앞일 때 출력 후 불러오기
				else {
					outMap[0] = new Map(mapList[index + 1]->getKey(), mapList[index + 1]->getCount());
					outMap[0]->setValue();
					fileManager->outputFile(&outputFile, outMap, 1);
					splitWord(mapList, 1, &fs2);
				}
			}
		}

		fileNum += 1;

		fs1.close();
		fs2.close();
		outputFile.close();

		fileManager->deleteFile("./folder/word" + to_string(i) + "_thread" + to_string(threadNum) + ".txt");
		fileManager->deleteFile("./folder/word" + to_string(i + 1) + "_thread" + to_string(threadNum) + ".txt");
	}

	if (fileNum != 0) {
		fs1.open("./folder/word" + to_string(fileNum - 1) + "_thread" + to_string(threadNum) + ".txt", ios::in);
		outputFile.open("./folder/word" + to_string(threadNum) + ".txt", ios::out);

		while (getline(fs1, str)) {
			outputFile << str << endl;
		}

		fs1.close();
		outputFile.close();

		fileManager->deleteFile("./folder/word" + to_string(fileNum - 1) + "_thread" + to_string(threadNum) + ".txt");
	}


	//sema.notify();

	return fileNum;
}

int Merge::fileMerge(vector<int> fileNums, int threadNum) {
	// 파일 병합
	fstream fs1;
	fstream fs2;
	fstream outputFile;
	string str;
	Map* mapList[2];
	Map* outMap[1];
	int last = threadNum;

	for (int i = 0; i <= threadNum; i += 2) {
		fs1.open("./folder/word" + to_string(i) + ".txt", ios::in);
		fs2.open("./folder/word" + to_string(i + 1) + ".txt", ios::in);

		if (!fs1.is_open() && !fs2.is_open()) {
			cout << "[ERROR] file is not open <Merge::fileMerge>" << endl;
			exit(-1);
		}

		if (!fs1.is_open()) {
			fs1.close();
			fs2.close();
			threadNum = i;
			break;
		}
		if (!fs2.is_open()) {
			fs1.close();
			fs2.close();
			threadNum = i;
			break;
		}
		if (fileNums[last] != 0) {
			outputFile.open("./folder/word" + to_string(threadNum + 1) + ".txt", ios::out);
		}
		else {
			outputFile.open("./folder/word" + to_string(threadNum) + ".txt", ios::out);
		}


		splitWord(mapList, 0, &fs1);
		splitWord(mapList, 1, &fs2);

		while (1) {
			int index = 0;

			if (mapList[0] == nullptr && mapList[1] == nullptr) {
				break;
			}
			// 둘 중 하나의 파일이 모두 작성된 후일 경우
			else if (mapList[0] == nullptr) {
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
					splitWord(mapList, 0, &fs1);
					splitWord(mapList, 1, &fs2);
				}
				// fs1 파일이 사전 순으로 앞일 때 출력 후 불러오기
				else if (mapList[index]->getKey() < mapList[index + 1]->getKey()) {
					outMap[0] = new Map(mapList[index]->getKey(), mapList[index]->getCount());
					outMap[0]->setValue();
					fileManager->outputFile(&outputFile, outMap, 1);
					splitWord(mapList, 0, &fs1);
				}
				// fs2 파일이 사전 순으로 앞일 때 출력 후 불러오기
				else {
					outMap[0] = new Map(mapList[index + 1]->getKey(), mapList[index + 1]->getCount());
					outMap[0]->setValue();
					fileManager->outputFile(&outputFile, outMap, 1);
					splitWord(mapList, 1, &fs2);
				}
			}
		}

		threadNum += 1;

		fs1.close();
		fs2.close();
		outputFile.close();

		fileManager->deleteFile("./folder/word" + to_string(i) + ".txt");
		fileManager->deleteFile("./folder/word" + to_string(i + 1) + ".txt");
	}

	fs1.open("./folder/word" + to_string(threadNum) + ".txt", ios::in);
	outputFile.open("./folder/final.txt", ios::out);

	while (getline(fs1, str)) {
		outputFile << str << endl;
	}

	fs1.close();
	outputFile.close();

	fileManager->deleteFile("./folder/word" + to_string(threadNum) + ".txt");

	return threadNum;
}
/*
int Merge::fileMerge(int fileNum, int threadNum) {
	// 파일 병합
	string str;
	Map* outMap[1];

	for (int i = 0; i < fileNum - 1; i += 2) {
		fs1.open("./folder/" + filename + to_string(i + 1) + ".txt", ios::in);
		fs2.open("./folder/" + filename + to_string(i + 2) + ".txt", ios::in);
		outputFile.open("./folder/" + filename + to_string(fileNum + 1) + ".txt", ios::out);

		if (!fs1.is_open() || !fs2.is_open()) {
			cout << "[ERROR] file is not open <Mergge::fileMerge>" << endl;
			exit(-1);
		}

		splitWord(0, &fs1);
		splitWord(1, &fs2);

		while (1) {
			int index = 0;

			if (mapList[0] == nullptr && mapList[1] == nullptr) {
				break;
			}
			// 둘 중 하나의 파일이 모두 작성된 후일 경우
			else if (mapList[0] == nullptr) {
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
}*/
/*
int Merge::wordCount(int fileNum) {
	// 단어 개수 세기
	fstream fs1;
	fstream outputFile;

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

	fileManager->deleteFile("./folder/" + filename + to_string(fileNum) + ".txt");
	fileNum++;

	return fileNum;
}*/

void Merge::wordCount() {
	// 단어 개수 세기
	fstream fs1;
	fstream outputFile;
	Map* mapList[2];

	fs1.open("./folder/final.txt", ios::in);
	outputFile.open("./folder/result.txt", ios::out);

	splitWord(mapList, 0, &fs1);

	while (mapList[0] != nullptr) {
		cout << mapList[0]->getKey() << "," << mapList[0]->getCount() << endl;
		outputFile << mapList[0]->getKey() << "," << mapList[0]->getCount() << endl;
		splitWord(mapList, 0, &fs1);
	}

	fs1.close();
	outputFile.close();

	fileManager->deleteFile("./folder/final.txt");
}