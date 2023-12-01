// map.h : key���� value ���� ��� �����ϱ� ���� ��� ����
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
