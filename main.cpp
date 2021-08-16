#include <array>
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#include "misc.h"

int main(void) {
	using namespace misc;
	using std::cout;
	using std::endl;

#ifdef __linux__
	constexpr auto str_ex = "エンコーディングが判定できるくらい長い文章を入れる";
	auto results = chardet(str_ex);
	cout << str_ex << endl;
	cout << "Detected encoding: " << results.enc << ", " << results.confidence << endl;

	std::ofstream ofs("utf8-sig.txt");
	add_utf8_bom(ofs);
	ofs << "日本語は認識できる？？" << endl;
	ofs << "ファイルストリーム" << endl;

	cout << sjis_to_utf8(utf8_to_sjis("ほげほげ")) << endl;

	const std::array<std::string, 2> filenames{"file_utf8.txt", "file_sjis.txt"};
	for (auto& filename : filenames) {
		std::ifstream input_file(filename);
		if (!input_file.is_open()) {
			std::cerr << "Could not open the file - '" << filename << "'" << endl;
			return EXIT_FAILURE;
		}
		std::stringstream ss;
		std::string enc = "UTF-8";
		for (std::string line; getline_rtrim(input_file, line);) {
			rtrim(line);
			ss << line;
			auto results = chardet(ss.str());
			if (results.confidence > 50) {
				enc = results.enc;
				cout << "Detected encoding: " << enc << ", " << results.confidence << endl;
				break;
			}
		}
		input_file.clear();					 // clear fail and eof bits
		input_file.seekg(0, std::ios::beg);	 // back to the start!

		std::function<std::string(std::string &&)> conv;
		if (enc == "Shift_JIS") {
			conv = [](std::string&& src) { return sjis_to_utf8(src); };
		} else if (enc == "UTF-8") {
			conv = [](std::string&& src) { return std::move(src); };
		} else {
			throw "Invalid encoding: " + enc;
		}

		for (std::string line; getline_rtrim(input_file, line);) {
			line = conv(std::move(line));
			rtrim(line);
			cout << line << endl;
		}
	}
#elif _WIN32
	cout << UTF8_TO_TERM_ENC(sjis_to_utf8("ほげほげ")) << endl;
	cout << UTF8_TO_TERM_ENC(sjis_to_utf8("日本医療政策機構")) << endl;
	std::ifstream input_file("file_utf8.txt");
	if (!input_file.is_open()) {
		std::cerr << "Could not open the file - '"
				  << "file_utf8.txt"
				  << "'" << endl;
		return EXIT_FAILURE;
	}
	std::string line;
	while (getline_rtrim(input_file, line)) {
		line = UTF8_TO_TERM_ENC(line);
		rtrim(line);
		cout << line << endl;
	}
#endif
	return 0;
}