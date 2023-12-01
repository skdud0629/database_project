// map.h : key값과 value 값을 묶어서 저장하기 위한 헤더 파일
#include <string>

#ifndef MAP_H
#define MAP_H

#define MAXCOUNT 100

using namespace std;

class Map
{
private:
	int count;
	string key;
	int value[MAXCOUNT];
public:
	Map(string key, int count);
	~Map();

	void setKey(string key);
	void setValue();
	void setCount(int count);
	string getKey();
	int getCount();
	int* getValue();
};

#endif MAP_H
