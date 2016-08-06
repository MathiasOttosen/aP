#include "DataStore.h"

bool jserialization::ReadSingleInput(std::ifstream& fs, std::string& data) {
	fs >> data;
	return fs.eof();
}

bool jserialization::Search(std::ifstream& fs, std::list<std::string>& data) {
	auto eof = true;
	for (auto it = data.begin(); it != data.end(); it++) {
		eof = ReadSingleInput(fs, *it);
		if (eof) {
			break;
		}
	}
	return eof;
}

int jserialization::findOrInsert(const std::string objectSpecifier, const std::list<std::string> query, std::list<std::string> datalist) {
	std::ifstream fs(objectSpecifier);
	int counter = 0;
	while (Search(fs, datalist)) {

		if (std::equal(std::begin(query), std::end(query), std::begin(datalist))) {
			int id;
			StrToNumber<int>(datalist.front(), id);
			return id;
		}
		counter++;
	}
	fs.close();
	return counter;
}