// map.h : key���� value ���� ��� �����ϱ� ���� ��� ����
#include <string>

#ifndef MAP_H
#define MAP_H

#define MAXCOUNT 10000

using namespace std;

class Map
{
private:
	int count;
	string key;
	int* value;
public:
	Map();
	Map(string key, int count);
	~Map();

	void setKey(string key);
	void setValue();
	void setCount(int count);
	string getKey() const;
	int getCount() const;
	int* getValue();
	bool operator<(const Map& other) const;
};

#endif MAP_H
