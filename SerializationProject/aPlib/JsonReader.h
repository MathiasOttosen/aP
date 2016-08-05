#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <tuple>
#include <vector>
#include <cstdlib>
#include "JsonObject.h"
#include "JsonParser.h"

namespace jserialization {
	namespace stdvisitor {
		class  JsonReader {
			using ctype = jserialization::ConceptType;
			using btype = jserialization::BaseType;
			using Parser = jserialization::Parser;

			Parser* parser;

		public:

			JsonReader(std::string filename);

			~JsonReader();


			template <typename TYPE>
			std::enable_if_t<jserialization::is_container<TYPE>::value, void> visit(JsonObject<TYPE>& jo) {

				std::cout << "Reading: " << jo.GetBaseName() << std::endl;
				while (!parser->EndOfObject()) {

					auto tuple = parser->ParseNextObject(); //tuple<string name, string value, Concept_Type, Base_Type>

					switch (std::get<2>(tuple)) {
					case ctype::STRING:
						CreateJsonObject(jo, tuple, std::get<1>(tuple));
						break;
					case ctype::NUMBER:
						createAndInsertJsonObject(jo, tuple);
						break;
					case ctype::ARRAY:
					case ctype::INITIALIZERLIST:
					case ctype::CONTAINER:
						CreateJsonObject(jo, tuple, std::vector<BaseValue*>());
						break;
					}
				}
			}

			template <typename TYPE>
			std::enable_if_t<!jserialization::is_container<TYPE>::value, void> visit(JsonObject<TYPE>& jo) {
				std::cout << "Reading: " << jo.GetBaseName() << std::endl;
			}

			template <typename TYPE, typename SUBTYPE>
			std::enable_if_t<jserialization::is_container<SUBTYPE>::value, void> 
				CreateJsonObject(JsonObject<TYPE>& jo, 
					const std::tuple<std::string, std::string, 
					ConceptType, BaseType> tuple,
					const SUBTYPE value) {
				auto js = new JsonObject<SUBTYPE>(tuple, value);
				js->accept(*this);
				jo.insert(js);
			}

			template <typename TYPE, typename SUBTYPE>
			std::enable_if_t<!jserialization::is_container<SUBTYPE>::value, void> 
				CreateJsonObject(JsonObject<TYPE>& jo, 
					const std::tuple<std::string, std::string, ConceptType, BaseType> tuple,
					const SUBTYPE value) {
				auto js = new JsonObject<SUBTYPE>(tuple, value);
				jo.insert(js);
			}

			template <typename TYPE>
			void createAndInsertJsonObject(JsonObject<TYPE>& jo, std::tuple<std::string, std::string, ctype, btype> tuple) {
				switch (std::get<3>(tuple)) {
				case btype::STRING:
					jo.insert(new JsonObject<std::string>(tuple, std::get<1>(tuple)));
					break;
				case btype::BOOL:
					ConvertAndCreate < TYPE, bool > (jo, tuple);
					break;
				case btype::CHAR:
					ConvertAndCreate<TYPE, int8_t>(jo, tuple);
					break;
				case btype::UCHAR:
					ConvertAndCreate<TYPE, uint8_t>(jo, tuple);
					break;
				case btype::INT:
					ConvertAndCreate<TYPE, int16_t>(jo, tuple);
					break;
				case btype::UINT:
					ConvertAndCreate<TYPE, uint16_t>(jo, tuple);
					break;
				case btype::LONG:
					ConvertAndCreate<TYPE, int32_t>(jo, tuple);
					break;
				case btype::ULONG:
					ConvertAndCreate<TYPE, uint32_t>(jo, tuple);
					break;
				case btype::LONGLONG:
					ConvertAndCreate<TYPE, int64_t>(jo, tuple);
					break;
				case btype::ULONGLONG:
					ConvertAndCreate<TYPE, uint64_t>(jo, tuple);
					break;
				case btype::FLOAT:
					ConvertAndCreate<TYPE, float>(jo, tuple);
					break;
				case btype::DOUBLE:
					ConvertAndCreate<TYPE, double>(jo, tuple);
					break;
				case btype::LONGDOUBLE:
					ConvertAndCreate<TYPE, long double>(jo, tuple);
					break;
				}
			}

			template <typename TYPE, typename SUBTYPE>
			void ConvertAndCreate(JsonObject<TYPE>& jo, const std::tuple<std::string, std::string, ctype, btype> tuple) {
				SUBTYPE val;
				if (StrToNumber<SUBTYPE>(std::get<1>(tuple),val)) {
					auto js = new JsonObject<SUBTYPE>(tuple, val);
					jo.insert(js);
				}
				else {
					throw std::runtime_error("Could not convert");
				}
			}


		};
	}
}