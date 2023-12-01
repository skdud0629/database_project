#define _CRT_SECURE_NO_WARNINGS
#include <algorithm>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "Map.h"
#include "FileManager.h"
#include "Semaphore.h"
#include "Split.h"

auto comp = [](const Map& a, const Map& b) { return a.getKey() > b.getKey(); };
/*
Split::Split() : sema(10), mapList(comp) {
	// 생성
	this->fileManager = new FileManager();
}*/

Split::Split() : sema(10) {
	// 생성
	this->fileManager = new FileManager();
}

Split::~Split() {
	// 소멸자
}

int Split::fileSplit(int strNum, string filename) {
	fstream fs;
	fstream fout;
	string str;
	int index = 0;
	int nowStrNum = 0;

	fs.open("./folder/" + filename + ".txt", ios::in);

	if (!fs.is_open()) {
		cout << "[ERROR] file is not open <Split::wordSplit>" << endl;
		exit(-1);
	}

	fout.open("./folder/file" + to_string(index) + ".txt", ios::out);

	while (!fs.eof()) {
		getline(fs, str);
		fout << str << endl;
		nowStrNum += 1;

		if (nowStrNum == strNum) {
			fout.close();
			index += 1;
			nowStrNum = 0;

			fout.open("./folder/file" + to_string(index) + ".txt", ios::out);
		}
	}

	fout.close();
	fs.close();
	return index;		// 스레드 개수
}
/*
int Split::wordSplit(int threadNum, string filename) {
	// 단어 분할 실행
	fstream fs;
	fstream outputFile;
	string str;
	int index = 0;
	int fileNum = 0;

	fs.open("./folder/" + filename + ".txt", ios::in);

	if (!fs.is_open()) {
		cout << "[ERROR] file is not open <Split::wordSplit>" << endl;
		exit(-1);
	}

	//sema.wait();

	Map** mapList = new Map*[MAXMAP];

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
				wordSort(&mapList, MAXMAP);
				outputFile.open("./folder/word" + to_string(fileNum) + "_thread" + to_string(threadNum) + ".txt", ios::out);
				fileManager->outputFile(&outputFile, mapList, MAXMAP);
				index = 0;
				for (int i = 0; i < MAXMAP; i++) {
					if (mapList[i] != nullptr) {
						delete mapList[i];  // 메모리 해제
						mapList[i] = nullptr;
					}
				}
				outputFile.close();

				fileNum++;
			}
			token = strtok(nullptr, ",.?!: ");
		}
	}

	if (index != 0) {
		wordSort(&mapList, index);
		outputFile.open("./folder/word" + to_string(fileNum) + "_thread" + to_string(threadNum) + ".txt", ios::out);
		fileManager->outputFile(&outputFile, mapList, index);
		outputFile.close();
		fileNum++;
	}

	for (int i = 0; i < MAXMAP; i++) {
		if (mapList[i] != nullptr) {
			delete mapList[i];  // 각 Map 객체에 대한 메모리 해제
		}
	}

	delete[] mapList;
	fs.close();

	//sema.notify();

	fileManager->deleteFile("./folder/" + filename + ".txt");

	return fileNum;
}*/

int Split::wordSplit(int threadNum, string filename) {
	// 단어 분할 실행
	ifstream fs("./folder/" + filename + ".txt", ios::binary);
	fstream outputFile;
	int index = 0;
	int fileNum = 0;
	const int CHUNK_SIZE = 1024 * 1024; // 1MB

	if (!fs.is_open()) {
		cout << "[ERROR] file is not open <Split::wordSplit>" << endl;
		exit(-1);
	}

	Map** mapList = new Map * [MAXMAP];
	vector<char> buffer(CHUNK_SIZE + 1);

	string lastWord; // 마지막으로 읽은 단어를 저장

	while (fs.read(buffer.data(), CHUNK_SIZE) || fs.gcount()) {
		buffer[fs.gcount()] = '\0'; // Null-terminate the string

		string chunk = lastWord + buffer.data(); // 마지막 단어를 청크 앞에 추가
		char* token = strtok(buffer.data(), ",.?!: ");

		while (token != nullptr) {
			mapList[index] = new Map(token, 1);
			mapList[index]->setValue();
			index++;

			if (index == MAXMAP) {
				wordSort(&mapList, MAXMAP);
				outputFile.open("./folder/word" + to_string(fileNum) + "_thread" + to_string(threadNum) + ".txt", ios::out);
				fileManager->outputFile(&outputFile, mapList, MAXMAP);
				outputFile.close();

				for (int i = 0; i < MAXMAP; i++) {
					delete mapList[i];
					mapList[i] = nullptr;
				}

				index = 0;
				fileNum++;
			}

			lastWord = token; // 마지막으로 읽은 단어를 저장

			token = strtok(nullptr, ",.?!: ");
		}
	}

	if (index != 0) {
		wordSort(&mapList, index);
		outputFile.open("./folder/word" + to_string(fileNum) + "_thread" + to_string(threadNum) + ".txt", ios::out);
		fileManager->outputFile(&outputFile, mapList, index);
		outputFile.close();

		fileNum++;
	}

	for (int i = 0; i < index; i++) {
		delete mapList[i];
	}

	delete[] mapList;
	fs.close();

	fileManager->deleteFile("./folder/" + filename + ".txt");

	return fileNum;
}


void merge(Map*** mapList, int left, int mid, int right) {
	int i = left;
	int j = mid + 1;
	int k = left;

	Map** temp = new Map * [right + 1];

	while (i <= mid && j <= right) {
		if ((*mapList)[i]->getKey() <= (*mapList)[j]->getKey()) {
			temp[k++] = (*mapList)[i++];
		}
		else {
			temp[k++] = (*mapList)[j++];
		}
	}

	while (i <= mid) {
		temp[k++] = (*mapList)[i++];
	}

	while (j <= right) {
		temp[k++] = (*mapList)[j++];
	}

	for (int t = left; t <= right; t++) {
		(*mapList)[t] = temp[t];
	}

	delete[] temp;
}

void mergeSort(Map*** mapList, int left, int right) {
	if (left < right) {
		int mid = (left + right) / 2;
		mergeSort(mapList, left, mid);
		mergeSort(mapList, mid + 1, right);
		merge(mapList, left, mid, right);
	}
}

void Split::wordSort(Map*** mapList, int index) {
	// 단어를 사전 순으로 정렬
	mergeSort(mapList, 0, index - 1);
}

/*
void Split::wordSort(Map*** mapList, int index) {
	// 단어를 사전 순으로 정렬
	for (int i = 0; i < index; i++) {
		for (int j = i + 1; j < index; j++) {
			if ((*mapList)[i]->getKey() > (*mapList)[j]->getKey()) {
				Map* temp = (*mapList)[i];
				(*mapList)[i] = (*mapList)[j];
				(*mapList)[j] = temp;
			}
		}
	}
}*/