#pragma once
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include "JsonObject.h"

#ifdef SERIALIZATION_LIB_EXPORTS
#define SERIALIZATION_LIB __declspec(dllexport) 
#else
#define SERIALIZATION_LIB __declspec(dllimport) 
#endif

namespace jserialization {
	namespace stdvisitor {
		class SERIALIZATION_LIB JsonWriter {

			const std::string filename_;

			std::ofstream file_;
			
		public:
			JsonWriter(const std::string& filename) : filename_(filename) {
				file_.open(filename);

			}
			~JsonWriter() {	
				file_.close();
			}
			
			template <typename TYPE, typename = std::enable_if_t < std::is_array<TYPE> && std::is_fundamental<base_array_type<TYPE>::type>>>
			void visit(JsonObject<TYPE>* jo) {
				
				auto arr = jo->GetValue();
				using ArraySize = array_properties<decltype(arr)>;

				file_ << "[";

				for (int i = 0; i < ArraySize::size; i++) {
					file_ << arr[i];
					if (i + 1 < ArraySize::size) {
						file_ << ",";
					}
				}

				file_ << "]";
			}

			template <typename TYPE, typename = std::enable_if_t <std::is_array<TYPE> && !std::is_fundamental<base_array_type<TYPE>::type>>>
			void visit(JsonObject<TYPE>* jo) {

				auto arr = jo->GetValue();
				using ArraySize = array_properties<decltype(arr)>;

				file_ << "[";

				for (int i = 0; i < ArraySize::size; i++) {
					arr[i].accept();
					if (i + 1 < ArraySize::size) {
						file_ << ",";
					}
				}

				file_ << "]";
			}

			template <typename TYPE, typename = std::enable_if_t<is_container<TYPE>>>
			void visit(JsonObject<TYPE>* jo) {

				file_ << "{";
				file_ << jo->GetBaseName();
				file_ << ": {";
				auto obj = jo->GetValue();

				for (auto subobj = obj.begin(); subobj < obj.end(); subobj++) {
					subobj->accept();
					if (subobj + 1 < obj.end()) {
						file_ << ",";
					}
				}

				file_ << "}";
			}

			template <typename TYPE, typename = std::enable_if_t<is_text<TYPE>::value>>
			void visit(JsonObject<const char*>* jo) {
				file_ << jo->GetBaseName;
				file_ << ": \"";
				file_ << jo->GetValue;
				file_ << "\"";
			}

			template <typename TYPE>
			void visit(JsonObject<TYPE>* jo) {
				file_ << jo->GetBaseName;
				file_ << ":"
				file_ << std::boolalpha << jo->GetValue;
			}
		};
} }