#include <fstream>
#include <iostream>
#include <string>
#include <cstdlib> // for exit()
#include "JsonReader.h"
#include "JsonObject.h"

/*auto NoWSPeek(std::ifstream& inf)  {
	while (inf.peek() <= 10 || inf.peek() == 32) {
		inf.get();
	}
	return inf.peek();
}

char NoWSPeekChar(std::ifstream& inf) {
	while (inf.peek() <= 10 || inf.peek() == 32) {
		inf.get();
	}
	return inf.peek();
}*/

int main()
{
	
	using namespace jserialization;

	auto jr = stdvisitor::JsonReader("hourly_16.json");

	std::tuple<std::string, std::string, ConceptType, BaseType> tuple = 
		std::make_tuple(std::string(""),std::string(""), ConceptType(ConceptType::CONTAINER), BaseType(BaseType::CUSTOM));

	auto jo = JsonObject<std::vector<BaseValue*>>( tuple, std::vector<BaseValue*>());

	jo.accept(jr);

	//std::cin.get();

	/*using namespace std;

	// ifstream is used for reading files
	// We'll read from a file called Sample.dat
	ifstream inf("file.json");

	// If we couldn't open the output file stream for reading
	if (!inf)
	{
		// Print an error and exit
		cerr << "Uh oh, file.json could not be opened for reading!" << endl;
		exit(1);
	}

	// While there's still stuff left to read
	while (inf)
	{
		// read stuff from the file into a string and print it
		std::string strInput;
		inf >> strInput;
		cout << strInput << endl;
		cout << "peek: " << inf.peek() << endl;
		cout << "NoWSPeek: " << NoWSPeek(inf) << endl;
		cout << "NoWSPeekChar: " << NoWSPeekChar(inf) << endl;
		std::cin.get();
	}*/

	return 1;

	// When inf goes out of scope, the ifstream
	// destructor will close the file
}