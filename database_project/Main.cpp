// Main ���� : ��ü ���α׷� ����
#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <thread>
#include <time.h>
#include "Split.h"
#include "Map.h"
#include "FileManager.h"
#include "Merge.h"

using namespace std;

int main() {
	/*test*/
	clock_t full_start, full_end;
	clock_t split_start, split_end;
	clock_t merge_start, merge_end;
	double duration;
	int fileNum = 0;
	int threadNum = 0;
	int strNum = 0;
	string fileName;
	FileManager* fileManager = new FileManager();

	cout << "filename : ";
	cin >> fileName;
	cout << "str split num : ";
	cin >> strNum;
	cout << "total thread number:";//40�� ���������� �Է¹޴°� ���ɸ鿡�� ����.
	cin >> threadNum;
	cout << endl;
	
	full_start = clock();
	split_start = clock();
	cout << "Split Start" << endl;

	Split* split = new Split();
	//threadNum = split->fileSplit(strNum, fileName);
	split->splitFileByThread(threadNum,fileName);//������ ���� ������ ����.


	vector<thread> threads(threadNum + 1);
	vector<int> fileNums(threadNum + 1);

	for (int i = 0; i < threadNum; i++) {
		threads[i] = thread([&split, &fileNums, i]() {
			fileNums[i] = split->wordSplit(i, "_thread"  + to_string(i)); });
		cout<<fileNums[i]<<endl;

	}

	// �� �������� �۾��� ���� ������ ���
	for (auto& th : threads) {
		th.join();
	}

	cout << "Split End" << endl;
	split_end = clock();

	cout << endl;
	duration = (double)(split_end - split_start) / CLOCKS_PER_SEC;
	cout << "split_time : " << duration << endl;
	cout << endl;
	
	merge_start = clock();
	cout << "Merge Start" << endl;

	Merge* merge = new Merge();

	for (int i = 0; i <= threadNum; i++) {
		threads[i] = thread([&merge, &fileNums, i]() {
			fileNums[i] = merge->thread_fileMerge(fileNums[i], i);
			});
	}

	// �� �������� �۾��� ���� ������ ���
	for (auto& th : threads) {
		th.join();
	}

	merge->fileMerge(fileNums, threadNum);

	cout << "Merge End" << endl;
	merge_end = clock();

	cout << endl;
	duration = (double)(merge_end - merge_start) / CLOCKS_PER_SEC;
	cout << "merge_time : " << duration << endl;
	cout << endl;

	//fileManager->deleteFile("./folder/file" + to_string(threadNum) + ".txt");

	cout << "=============[word count]============" << endl;
	merge->wordCount();
	cout << "=====================================" << endl;

	full_end = clock();
	duration = (double)(full_end - full_start) / CLOCKS_PER_SEC;

	cout << "time : " << duration << endl;*/

	return 0;
}