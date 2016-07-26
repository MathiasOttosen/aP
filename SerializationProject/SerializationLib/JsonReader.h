#pragma once
#include "JsonObject.h"
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>

namespace jserialization {
	class JsonReader {

	private:
		std::ifstream inf;

	public:
		void startFileStream(std::string filename) {
			try {
				inf.open(filename);
			}
			catch (std::ifstream::failure e) {
				std::cout << "Exception opening/reading file";
			}

			// While there's still stuff left to read
			/*while (inf)
			{
				// read stuff from the file into a string and print it
				std::string strInput;
				inf >> strInput;
				std::cout << strInput << std::endl;
			}*/
		}

		void closeFileStream() {
			try {
				inf.close();
			}
			catch (std::ifstream::failure e) {
				std::cout << "Failed to close file";
			}
		}

		

		template <typename TYPE, typename = std::enable_if_t<is_container<TYPE>>>
		void visit(JsonObject<TYPE>* jo) {
			std::string tmp;
			std::string key = "";
			Vector<void*> elementList = new Vector<void*>();

			inf >> tmp;
			//we need to identify what json object type we are looking at
			if (tmp.Equals("{")) { //it is assumed that the first symbol is always removed thus "{" would mean we are looking at an array
				while (expect(tmp,"{")) {
					std::string subkey;
					inf >> subkey;
					inf >> tmp;
					if (expect(tmp, ":")) {
						inf >> tmp;
						if (expect(tmp, "{") || expect(tmp, "[")) {

							//make container json object with key as name
							//it is expected that we end with "}" is read and thus not available when we return
						}
						else
						{
							//type id tmp
							//make json object with that type
						}
					}
					inf >> tmp; //2 possiblilties, "," is stored and we continues the loop, "]" is stored and we end the loop
					if (expect(tmp, ",")) {
						inf >> tmp; //skip to '{' in order to continue loop
					}
				}
			} 
			else { //if we dont encounter "{" then we can assume we are looking at a name
				key = tmp;
				inf >> tmp; // expect ":"
				if (expect(tmp, ":")) {
					do
					{
						inf >> tmp;
						if (tmp.Equals("{") || tmp.Equals("[")) {
							//make container json object with key as name
						}
						else
						{
							//type id tmp
							//make json object with that type
						}
					} while (expect(tmp, ","));
				}
				else
				{
					//bad json object
				}
			}

			
		}

		template <typename TYPE, typename = std::enable_if_t<!is_container<TYPE>>>
		void visit(JsonObject<TYPE>* jo) {

		}

		template <typename TYPE, typename = std::enable_if_t<is_array<TYPE>>>
		std::vector<Holder*> extractContainerInfo() {

		}

		bool expect(std::string expectee, std::string comparee) {
			return expectee.compare(comparee);
		}

		
	};
}