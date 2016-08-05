#pragma once
#include <string>
#include <tuple>
#include "EBaseType.h"
#include "TypeDeductionHelper.h"
#include "EConceptType.h"
#include "BaseValue.h"

namespace jserialization {
	template <typename TYPE>
	class JsonObject : public BaseValue {

		TYPE jvalue_;

	public:

	    using is_array = std::is_array<TYPE>;
		using is_container = is_container<TYPE>;
		

		template<typename T = TYPE>
		JsonObject(std::string name, std::string valueString, ConceptType conceptType, BaseType baseType, T v) noexcept
			: BaseValue(std::make_tuple(name, valueString, conceptType, baseType)), jvalue_(v) {}

		template<typename T = TYPE>
		JsonObject(std::string valueString, ConceptType conceptType, BaseType baseType, T v) noexcept
			: BaseValue(std::make_tuple("", valueString, conceptType, baseType)), jvalue_(v) {}

		template<typename T = TYPE>
		JsonObject(std::tuple<std::string, std::string, ConceptType, BaseType> metaInfo, T value) noexcept
			: BaseValue(metaInfo), jvalue_(value) {}

		~JsonObject() {
			for (auto it = std::begin(jvalue_); it != std::end(jvalue_); it++) {
				delete *it;
			}
			~BaseValue();
		}

		template <typename Visitor>
		void accept(Visitor& v) {
			v.visit(*this);
		}
		
		TYPE GetValue() {
			return jvalue_;
		}

		//container with BaseValues
		template<typename T>
		std::enable_if_t<is_container::value && std::is_same<BaseValue, std::remove_reference_t<T>>::value, void>
			insert(T* item) {
			valueIdList_.push_back(findOrInsert(item));
		}

		//container without BaseValues
		template<typename T>
		std::enable_if_t<is_container::value && !std::is_same<BaseValue, std::remove_reference_t<T>>::value, void> 
			insert(T* item) {
			valueIdList_.push_back(findOrInsert(item));
		}

		//noncontainer 
		template<typename T>
		std::enable_if_t<!is_container::value, void>
			insert(T* item) {
			valueIdList_.push_back(findOrInsert(item));
		}


		template<typename T>
		std::enable_if_t<is_container::value && std::is_same<BaseValue, std::remove_reference_t<T>>::value, void>
			Serialize(std::ofstream& fs) {
			const std::string name = GetBaseName();
			const std::string value = GetBaseValue();
			const std::string concepttype = CONCEPTTYPE_STRING[GetConceptType()];
			const std::string basetype = BASETYPE_STRING[GetBaseType()];
			const int size = GetSize();
			const std::list<int> subobjects = GetValueIDList();
			fs << name << value << concepttype << basetype << size;
			for (auto i = subobjects.begin(); i < subobjects.end(); i++) {
				fs << subobjects[i];
			}
		}

		template<typename T>
		std::enable_if_t<!is_container::value && !std::is_same<BaseValue, std::remove_reference_t<T>>::value, void>
			Serialize(std::ofstream& fs) {
			const std::string name = GetBaseName();
			const std::string value = GetBaseValue();
			const std::string concepttype = CONCEPTTYPE_STRING[GetConceptType()];
			const std::string basetype = BASETYPE_STRING[GetBaseType()];
			const int size = GetSize();
			fs << name << value << concepttype << basetype << size;
		}

		template<typename T>
		std::enable_if_t<is_container::value && std::is_same<BaseValue, std::remove_reference_t<T>>::value, void>
			Deserialize(std::ifstream& fs) {
			std::string name;
			std::string value;
			std::string concepttype;
			std::string basetype;
			int size;
			std::list<int> subobjects;
			fs >> name >> value >> concepttype >> basetype >> size;
			for (auto i = subobjects.begin(); i < subobjects.end(); i++) {
				fs >> subobjects[i];
			}
		}

		template<typename T>
		std::enable_if_t<!is_container::value && !std::is_same<BaseValue, std::remove_reference_t<T>>::value, void>
			Deserialize(std::ifstream& fs) {
			std::string name;
			std::string value;
			std::string concepttype;
			std::string basetype;
			int size;
			fs >> name >> value >> concepttype >> basetype >> size;
		}



		bool compare(std::string& name, std::string& value, std::string& concept_type, std::string& base_type, int& size, std::list<int> subobjects) {
			if (name.compare(GetBaseName()) == 0 && value.compare(GetBaseValue()) == 0 &&
				compareWithConceptEnum(concept_type, GetConceptType()) && compareWithBaseEnum(base_type, GetBaseType()) &&
				size == GetSize() && std::equal(std::begin(subobjects), std::end(subobjects), std::begin(GetValueIDList()))) {
				return true;
			}
			return false;
		}
	};
}