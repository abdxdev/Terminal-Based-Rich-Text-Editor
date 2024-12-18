#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <string>

using namespace std;

class TrieNode {
public:
	unordered_map<char, TrieNode*> children;
	bool isEnd;

public:
	TrieNode();
};

class Trie {
public:
	static Trie words;

	Trie();
	vector<string> getSuggestions(const string& prefix, int limit);

private:
	TrieNode* rototo;
	static Trie getWords();
	void collectWords(TrieNode* node, string currentWord, vector<string>& words, int limit);
	void insert(const string& word);
};