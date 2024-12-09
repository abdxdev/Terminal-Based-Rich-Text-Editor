#include "Trie.h"

TrieNode::TrieNode() : isEnd(false) {}

Trie Trie::words = getWords();

Trie Trie::getWords() {
	Trie trie;
	ifstream file("words.txt");
	string word;
	while (file >> word)
		trie.insert(word);
	file.close();
	return trie;
}

void Trie::collectWords(TrieNode* node, string currentWord, vector<string>& words, int limit) {
	if (node->isEnd)
		words.push_back(currentWord);
	for (auto& pair : node->children) {
		if (words.size() >= limit && limit != -1)
			return;
		collectWords(pair.second, currentWord + pair.first, words, limit);
	}
}

void Trie::insert(const string& word) {
	TrieNode* current = root;
	for (char c : word) {
		if (current->children.find(c) == current->children.end())
			current->children[c] = new TrieNode();
		current = current->children[c];
	}
	current->isEnd = true;
}

Trie::Trie() : root(new TrieNode()) {}


vector<string> Trie::getSuggestions(const string& prefix, int limit = -1) {
	TrieNode* current = root;
	vector<string> suggestions;
	for (char c : prefix) {
		if (current->children.find(c) == current->children.end())
			return suggestions;
		current = current->children[c];
	}
	collectWords(current, prefix, suggestions, limit);
	return suggestions;
}