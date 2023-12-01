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
	// ������
	this->fileNum = fileNum;
	this->fileManager = new FileManager();
}

Split::~Split() {
	// �Ҹ���
}

void Split::fileSplit(string filename) {
	// ���� ����(��Ƽ ������ ��� �� ����)
}

int Split::wordSplit(string filename) {
	// �ܾ� ���� ����
	string str;
	int index = 0;

	fs.open("./folder/" + filename + ".txt", ios::in);

	// ���Ͽ��� �� �پ� �о �ܾ�� ����
	while (!fs.eof()) {
		getline(fs, str);

		vector<char> cstr(str.c_str(), str.c_str() + str.size() + 1);
		char* token = strtok(&cstr[0], ",.?!: ");

		while (token != nullptr) {
			mapList[index] = new Map(token, 1);
			mapList[index]->setValue();
			index++;
			// �ܾ ���� ������ �Ǿ��� �� ���Ϸ� ������
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
	// �ܾ ���� ������ ����
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