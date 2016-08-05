#pragma once
#include <list>
#include <iostream>
#include <string>
#include <fstream>
#include "TypeDeductionHelper.h"
#include "jsonobject.h"
#include "EBaseType.h"
#include "EConceptType.h"

namespace jserialization {


	//atomic function
	template < typename TYPE>
	void input(TYPE& jo, int id) {
		std::ofstream fs(jo.ToStr(), ios::app);
		jo.Serialize(fs);
	}

	//async function
	template<typename TYPE>
	int findOrInsert(JsonObject<TYPE>& jo) {
		std::ifstream fs(jo.ToStr());

		jo.Deserialize(fs);

		int id;
		std::string name;
		std::string value;
		std::string concepttype;
		std::string basetype;
		int size;
		std::list<int> subobjects;
		int counter = 0;
		while (fs >> id >> name >> value >> concepttype >> basetype >> size) {
			for (int i = 0; i < size; i++) {
				int val;
				fs >> val;
				subobjects.push_back(val);
			}
			if (jo.compare(name, value, concepttype, basetype, size, subobjects)) {
				return id;
			}
			counter++;
		}
		input(jo, counter++);
		return counter;
	}
}