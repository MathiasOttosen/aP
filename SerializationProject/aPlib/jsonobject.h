#pragma once
#include <string>
#include <memory>
#include <tuple>
#include "EBaseType.h"
#include "TypeDeductionHelper.h"
#include "EConceptType.h"
#include "BaseValue.h"
#include "DataStore.h"

namespace jserialization {
	template <typename TYPE>
	class JsonObject : public BaseValue {

		TYPE jvalue_;

	public:

	    using is_array = std::is_array<TYPE>;
		using is_container = is_container<TYPE>;
		
		template<typename T = TYPE>
		JsonObject(std::string& name, std::string& valueString, ConceptType& conceptType, BaseType& baseType, T& v) noexcept
			: BaseValue(std::make_tuple(name, valueString, conceptType, baseType)), jvalue_(v) {}

		template<typename T = TYPE>
		JsonObject(std::string& name, std::string& valueString, ConceptType& conceptType, BaseType& baseType, std::list<int>& subobjects) noexcept
			: BaseValue(std::make_tuple(name, valueString, conceptType, baseType), subobjects) {}

		template<typename T = TYPE>
		JsonObject(std::string& valueString, ConceptType& conceptType, BaseType& baseType, T& v) noexcept
			: BaseValue(std::make_tuple("", valueString, conceptType, baseType)), jvalue_(v) {}

		template<typename T = TYPE>
		JsonObject(const std::tuple<std::string, std::string, ConceptType, BaseType>& metaInfo, T& value) noexcept
			: BaseValue(metaInfo), jvalue_(value) {}

		~JsonObject() {
		}

		template <typename Visitor>
		void accept(Visitor& v) {	
			v.visit(*this);
		}

		TYPE GetValue() {
			return jvalue_;
		}

		template<typename T = TYPE>
		std::enable_if_t<is_container::value && std::is_same<BaseValue, std::remove_reference_t<T>>::value, std::list<std::string>> GetValueAsStr() {
			return std::list<std::string>;
		}


		template<typename T = TYPE>
		std::enable_if_t<is_container::value && !std::is_same<BaseValue, std::remove_reference_t<T>>::value, std::list<std::string>> GetValueAsStr() {
			std::list<std::string> lst;
			for (int i = 0; i < GetSize(); i++) {
				lst.push_back(std::to_string(jvalue_));
			}
			return lst;
		}

		//default
		template<typename T = TYPE>
		std::list<std::string> GetValueAsStr() {
			std::list<std::string> lst;
			lst.push_back(std::to_string(jvalue_));
			return lst;
		}

		//container with BaseValues
		template<typename T = TYPE>
		std::enable_if_t<is_container::value && std::is_same<BaseValue, std::remove_reference_t<T>>::value, void>
			insert(T item) {
			InsertValueId(findOrInsert(ToStr(), item->GetMemberList(), GetEmptyMemberList()));
		}
		
		//container without BaseValues
		template<typename T = TYPE>
		std::enable_if_t<is_container::value && !std::is_same<BaseValue, std::remove_reference_t<T>>::value, void>
			insert(T item) {
			//jvalue_.push_back(item);
			int i = 0;
			InsertValueId(i);
		}

		//noncontainer 
		template<typename T = TYPE>
		std::enable_if_t<!is_container::value && !std::is_same<BaseValue, std::remove_reference_t<T>>::value, void> 
			insert(T item) {
			jvalue_ = item;
		}

		template<typename T = TYPE>
		std::enable_if_t<is_container::value && std::is_same<BaseValue, std::remove_reference_t<T>>::value, void>
			Serialize(std::ofstream& fs, int id) {
			const std::string name = GetBaseName();
			const std::string value = GetBaseValue();
			const std::string concepttype = CONCEPTTYPE_STRING[GetConceptType()];
			const std::string basetype = BASETYPE_STRING[GetBaseType()];
			const int size = GetSize();
			const std::list<int> subobjects = GetValueIDList();
			fs << id << name << value << concepttype << basetype << size;
			for (auto i = subobjects.begin(); i < subobjects.end(); i++) {
				fs << subobjects[i];
			}
		}

		template<typename T = TYPE>
		std::enable_if_t<!is_container::value && std::is_same<BaseValue, std::remove_reference_t<T>>::value, void>
			Serialize(std::ofstream& fs, int id) {
			const std::string name = GetBaseName();
			const std::string value = GetBaseValue();
			const std::string concepttype = CONCEPTTYPE_STRING[GetConceptType()];
			const std::string basetype = BASETYPE_STRING[GetBaseType()];
			const int size = GetSize();
			const std::list<std::string> subobjects = GetValueAsStr();
			fs << id << name << value << concepttype << basetype << size;
			for (auto i = subobjects.begin(); i < subobjects.end(); i++) {
				fs << subobjects[i];
			}
		}

		template<typename T = TYPE>
		std::enable_if_t<!is_container::value && !std::is_same<BaseValue, std::remove_reference_t<T>>::value, void>
			Serialize(std::ofstream& fs, int id) {
			const std::string name = GetBaseName();
			const std::string value = GetBaseValue();
			const std::string concepttype = CONCEPTTYPE_STRING[GetConceptType()];
			const std::string basetype = BASETYPE_STRING[GetBaseType()];
			const int size = GetSize();
			const std::string subobject = GetValueAsStr();
			fs << id << name << value << concepttype << basetype << size << subobject;
		}

		template<typename T>
		std::enable_if_t<is_container::value && std::is_same<BaseValue, std::remove_reference_t<T>>::value, JsonObject<T>> 
			Deserialize(std::ifstream& fs) {
			int id;
			std::string name;
			std::string value;
			std::string concepttype;
			std::string basetype;
			int size;
			std::list<int> subobjects;
			fs >> id >> name >> value >> concepttype >> basetype >> size;
			for (auto i = subobjects.begin(); i < subobjects.end(); i++) {
				fs >> subobjects[i];
			}
			return new JsonObject<TYPE>(name, value, CONCEPTTYPE_STRING[concepttype], BASETYPE_STRING[basetype], std::list<int> subobjects);
		}

		template<typename T>
		std::enable_if_t<!is_container::value && !std::is_same<BaseValue, std::remove_reference_t<T>>::value, JsonObject<T>> 
			Deserialize(std::ifstream& fs) {
			int id;
			std::string name;
			std::string value;
			std::string concepttype;
			std::string basetype;
			int size;
			fs >> id >> name >> value >> concepttype >> basetype >> size;
		}

	};
}