// ���� ������ ����
#include <iostream>
#include <fstream>
#include <string>
#include <stdio.h>
#include "FileManager.h"
#include "Map.h"

using namespace std;

FileManager::FileManager() {
	// ������
	filename = "word";
}

FileManager::~FileManager() {
	// �Ҹ���
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
	// ���� ��������(���ڿ� ������ ���)
	*fs << str << endl;
}

void FileManager::outputFile(fstream *fs, Map* map[], int size) {
	// ���� ��������
	for (int i = 0; i < size - 1; i++) {
		// �ߺ��Ǵ� �ܾ�� �� �ٿ� ���� ���� 1�� ���
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

	// map[size - 1]�� ���� ó��
	*fs << map[size - 1]->getKey() << ",";
	for (int j = 0; j < map[size - 1]->getCount(); j++) {
		*fs << map[size - 1]->getValue()[j] << ",";
	}
	*fs << endl;
}

void FileManager::deleteFile(int fileNum) {
	// ���� �����ϱ�
	filename = "./folder/" + filename + to_string(fileNum) + ".txt";

	int delFile = remove(filename.c_str());

	if (delFile != 0) {
		cout << "[ERROR] failure to delete file" << endl;
		exit(-1);
	}

	filename = "word";
}
