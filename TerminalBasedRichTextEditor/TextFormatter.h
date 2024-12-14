#pragma once

#include <regex>
#include <string>
#include <vector>

using namespace std;

namespace TextFormatter {
//public:
	const string RESET = "\033[0m";

	const string BOLD = "\033[1m";
	const string UNDERLINE = "\033[4m";
	const string ITALIC = "\033[3m";
	const string STRIKETHROUGH = "\033[9m";
	const string INVERSE = "\033[7m";
	const string HIDE = "\033[8m";
	const string DIM = "\033[2m";
	const string BLINK = "\033[5m";

	const string N_BOLD = "\033[22m";
	const string N_UNDERLINE = "\033[24m";
	const string N_ITALIC = "\033[23m";
	const string N_STRIKETHROUGH = "\033[29m";
	const string N_INVERSE = "\033[27m";
	const string N_HIDE = "\033[28m";
	const string N_DIM = "\033[22m";
	const string N_BLINK = "\033[25m";

//private:
	vector<int> _parse_rgb(const string& rgb);

//public:
	string get_format(const string& fg = "", const string& bg = "", const bool bold = false, const bool underline = false, const bool italic = false, const bool strikethrough = false, const bool blink = false, const bool dim = false);

	string rgb_to_ansi(int r, int g, int b, const bool fg = true);
	string rgb_to_ansi(const vector<int>& rgb, const bool fg = true);
	string hsl_to_ansi(int h, int s, int l, const bool fg = true);
	string hsl_to_ansi(const vector<int>& hsl, const bool fg = true);
	string hex_to_ansi(const string& hex, const bool fg = true);

	vector<int> hsl_to_rgb(int h, int s, int l);
	vector<int> hsl_to_rgb(const vector<int>& hsl);
	vector<int> hex_to_rgb(const string& hex);

	vector<int> rgb_to_hsl(int r, int g, int b);
	vector<int> rgb_to_hsl(const vector<int>& rgb);
	vector<int> hex_to_hsl(const string& hex);

	string rgb_to_hex(int r, int g, int b);
	string rgb_to_hex(const vector<int>& rgb);
	string hsl_to_hex(int h, int s, int l);
	string hsl_to_hex(const vector<int>& hsl);
};