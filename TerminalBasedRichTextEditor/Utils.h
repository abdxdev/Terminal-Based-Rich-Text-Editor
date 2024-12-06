#pragma once
#include <codecvt>
#include <fcntl.h>
#include <fstream>
#include <io.h>
#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

namespace Utils {

wstring stows(const string& str);
string wstos(const wstring& str);
void printWide(const wstring& text);
string strip(const string& str);
vector<string> split(const string& str, const char& delimiter);
string join(const vector<string> arr, const string& delemeter);
int accumulate(vector<int>& arr, int start, int end);
int max_column_width(const vector<string>& arr);
void save_csv(const string& filename, const vector<vector<string>>& data, const vector<string>& headers = {}, const string& delimiter = ",");
vector<vector<string>> load_csv(const string& filename, const bool include_first_row = true, const string& delimiter = ",");

template <typename K, typename V>
vector<K> get_keys(const unordered_map<K, V>& m) {
    vector<K> keys;
    for (const auto& pair : m) {
        keys.push_back(pair.first);
    }
    return keys;
}

template <typename K, typename V>
vector<V> get_values(const unordered_map<K, V>& m) {
    vector<V> values;
    for (const auto& pair : m) {
        values.push_back(pair.second);
    }
    return values;
}

template <typename T>
void print_vector(const vector<T>& arr) {
    cout << "{ ";
    for (int i = 0; i < arr.size(); i++) {
        cout << arr[i] << ((i < arr.size() - 1) ? ", " : "");
    }
    cout << " }" << endl;
}

}; // namespace Utils
