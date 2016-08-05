#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <cstdlib>
#include "JsonObject.h"
#include "JsonParser.h"

namespace jserialization {
	namespace stdvisitor {
		class JsonReader {
			using ctype = jserialization::ConceptType;
			using btype = jserialization::BaseType;
			using Parser = jserialization::Parser;

			const Parser* parser;

		public:

			JsonReader(std::string filename);

			~JsonReader();


			template <typename TYPE, typename = std::enable_if_t<is_container<TYPE>>>
			void visit(JsonObject<TYPE>* jo) {
				while (!parser->EndOfObject()) {

					auto tuple = parser->ParseNextObject(); //tuple<string name, string value, Concept_Type, Base_Type>

					switch (std::get<2>(tuple)) {
					case ctype::STRING:
						auto js = new JsonObject<std::string>(tuple, std::get<1>(tuple));
						jo.insert(js);
						break;
					case ctype::NUMBER:
						createAndInsertJsonObject(jo, tuple);
						break;
					case ctype::ARRAY:
					case ctype::INITIALIZERLIST:
					case ctype::CONTAINER:
						auto js = new JsonObject<std::vector<BaseValue*>>(tuple, std::get<1>(tuple));
						js.accept(*this);
						jo.insert(js);
						break;
					}
				}
			}

			template <typename TYPE>
			void visit(JsonObject<TYPE>* jo) {

			}

			template <typename TYPE>
			void createAndInsertJsonObject(JsonObject<TYPE> jo, std::tuple<std::string, std::string, ctype, btype> tuple) {
				switch (base_type) {
				case btype::BOOL:
					auto js = new JsonObject<bool>(tuple, StrToNumber<bool>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::CHAR:
					auto js = new JsonObject<int8_t>(tuple, StrToNumber<int8_t>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::UCHAR:
					auto js = new JsonObject<uint8_t>(tuple, StrToNumber<uint8_t>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::INT:
					auto js = new JsonObject<int16_t>(tuple, StrToNumber<int16_t>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::UINT:
					auto js = new JsonObject<uint16_t>(tuple, StrToNumber<uint16_t>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::LONG:
					auto js = new JsonObject<int32_t>(tuple, StrToNumber<int32_t>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::ULONG:
					auto js = new JsonObject<uint32_t>(tuple, StrToNumber<uint32_t>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::LONGLONG:
					auto js = new JsonObject<int64_t>(tuple, StrToNumber<int64_t>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::ULONGLONG:
					auto js = new JsonObject<uint64_t>(tuple, StrToNumber<uint64_t>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::FLOAT:
					auto js = new JsonObject<float>(tuple, StrToNumber<float>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::DOUBLE:
					auto js = new JsonObject<double>(tuple, StrToNumber<double>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				case btype::LONGDOUBLE:
					auto js = new JsonObject<long double>(tuple, StrToNumber<long double>(std::get<1>(tuple)));
					jo.insert(js);
					break;
				}
			}


		};
	}
}