#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <vector>
#include "Map.h"

using namespace std;

Map::Map() {
	//기본 생성자
}

Map::Map(string key, int count) {
	this->count = count;
	this->key = key;
	this->value = nullptr; // 초기화
}

Map::~Map() {
	if (this->value != nullptr) {
		delete[] this->value;
	}
}

void Map::setKey(string key) {
	this->key = key;
}

void Map::setValue() {
	if (this->value != nullptr) {
		delete[] this->value;
	}
	this->value = new int[count];

	for (int i = 0; i < count; i++) {
		this->value[i] = 1;
	}
}

void Map::setCount(int count) {
	this->count = count;
}

string Map::getKey() const {
	return this->key;
}

int Map::getCount() const {
	return this->count;
}

int* Map::getValue() {
	return this->value;
}

bool Map::operator<(const Map& other) const {
	return this->key < other.key;
}