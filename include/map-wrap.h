#pragma once

#include <unordered_map>
#include <string>
#include <stdexcept>
#include <initializer_list>

class BiMap {//helper class
private:
	std::unordered_map<std::string, int> forward;
	std::unordered_map<int, std::string> reverse;
public:
	BiMap() = default;
	BiMap(std::initializer_list<std::pair<std::string, int>> list) {
		for (const auto& [key, value] : list) { insert(key, value); }
	}
	void insert(const std::string& key, int value) {
		if (forward.count(key) || reverse.count(value)) { return; }
		forward[key] = value;
		reverse[value] = key;
	}
	int at(const std::string& key) const { if (!forward.count(key)){ return -1; } return forward.at(key); }
	std::string at(int value) const { if (!reverse.count(value)){ return ""; } return reverse.at(value); }
	bool has(const std::string& key) const { return forward.count(key) > 0; }
	bool has(int value) const { return reverse.count(value) > 0; }
	void operator[](const std::pair<std::string, int>& pair) { insert(pair.first, pair.second); }
	int operator[](const std::string& str) const{ return at(str); }
	std::string operator[](const int& val) const{ return at(val); }
	void clear() { forward.clear(); reverse.clear(); }
	size_t size() const { return forward.size(); }
};
template <typename T>
class Map {//helper class
private:
	std::unordered_map<std::string, T> map;
public:
	Map() = default;
	Map(std::initializer_list<std::pair<std::string, T>> list) {
		for (const auto& [key, value] : list) {
			insert(key, value);
		}
	}
	void insert(const std::string& key, T value) {
		if (map.count(key)) { return; }
		map.insert({key,value});
	}T& at(const std::string& key) {
		if (!map.count(key)) map[key] = T(); // ensure the key exists
		return map.at(key);
	}
	const T& at(const std::string& key) const {
		if (!map.count(key)) throw std::out_of_range("Key not found: " + key);
		return map.at(key);
	}
	T& operator[](const std::string& key) {
		return at(key); // will insert default if missing
	}
	const T& operator[](const std::string& key) const {
		return at(key); // throws if missing
	}
	bool has(const std::string& key) const { return map.count(key) > 0; }
	void clear() { map.clear(); }
	size_t size() const { return map.size(); }
	auto begin() { return map.begin(); }
	auto end() { return map.end(); }
	auto begin() const { return map.begin(); }
	auto end() const { return map.end(); }
};
