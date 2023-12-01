#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include "Map.h"

using namespace std;

Map::Map(string key, int count) {
	this->count = count;
	this->key = key;
}

Map::~Map() {
	// 소멸자(따로 사용 X)
}

void Map::setKey(string key) {
	this->key = key;
}

void Map::setValue() {
	for (int i = 0; i < count; i++) {
		this->value[i] = 1;
	}
}

void Map::setCount(int count) {
	this->count = count;
}

string Map::getKey() {
	return this->key;
}

int Map::getCount() {
	return this->count;
}

int* Map::getValue() {
	return this->value;
}