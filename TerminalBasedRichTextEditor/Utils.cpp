#include "Utils.h"

wstring Utils::stows(const string& str) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

string Utils::wstos(const wstring& str) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.to_bytes(str);
}

void Utils::printWide(const std::wstring& text) {
    _setmode(_fileno(stdout), _O_U16TEXT);
    std::wcout << text;
    _setmode(_fileno(stdout), _O_TEXT);
}

string Utils::strip(const string& str) {
    if (str == "")
        return str;
    string stripped = "";
    int start = 0;
    while (str[start] == ' ' || str[start] == '\t' || str[start] == '\n')
        start++;
    int end = str.length() - 1;
    while (str[end] == ' ' || str[end] == '\t' || str[end] == '\n')
        end--;
    for (int i = start; i <= end; i++)
        stripped += str[i];
    return stripped;
}

vector<string> Utils::split(const string& str, const char& delimiter) {
    vector<string> tokens;
    string token = "";
    for (char c : str) {
        if (c == delimiter) {
            tokens.push_back(token);
            token = "";
        } else
            token += c;
    }
    tokens.push_back(token);
    return tokens;
}

string Utils::join(const vector<string> arr, const string& delemeter) {
    string str = "";
    for (int i = 0; i < arr.size(); i++) {
        str += arr[i];
        if (i != arr.size() - 1)
            str += delemeter;
    }
    return str;
}

int Utils::accumulate(vector<int>& arr, int start, int end) {
    int sum = 0;
    for (int i = start; i < end; i++)
        sum += arr[i];
    return sum;
}

int Utils::max_column_width(const vector<string>& arr) {
    int column_width = 0;
    for (string str : arr)
        if (str.length() > column_width)
            column_width = str.length();
    return column_width;
}

void Utils::save_csv(const string& filename, const vector<vector<string>>& data, const vector<string>& headers, const string& delimiter) {
    ofstream file(filename);
    if (headers.size() > 0) {
        for (int i = 0; i < headers.size(); i++) {
            file << headers[i];
            if (i < headers.size() - 1)
                file << delimiter;
        }
        file << endl;
    }
    for (int i = 0; i < data.size(); i++) {
        for (int j = 0; j < data[i].size(); j++) {
            file << data[i][j];
            if (j < data[i].size() - 1)
                file << delimiter;
        }
        file << endl;
    }
    file.close();
}

vector<vector<string>> Utils::load_csv(const string& filename, const bool include_first_row, const string& delimiter) {
    vector<vector<string>> data;
    ifstream file(filename);
    if (!file.is_open()) {
        throw runtime_error("File not found");
    }
    string line;
    if (!include_first_row)
        getline(file, line);
    while (getline(file, line)) {
        vector<string> row = split(line, ',');
        data.push_back(row);
    }
    file.close();
    return data;
}
