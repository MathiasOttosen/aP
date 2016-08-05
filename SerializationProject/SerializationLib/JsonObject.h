#pragma once
#include <string>
#include <tuple>
#include "EBaseType.h"
#include "TypeDeductionHelper.h"
#include "EConceptType.h"
#include "BaseValue.h"

namespace jserialization {
	template <typename TYPE>
	class JsonObject : BaseValue {

		const TYPE jvalue_;

	public:

	    using is_array = std::is_array<TYPE>;
		using is_container = is_container<TYPE>;
		

		JsonObject(std::string name, std::string valueString, ConceptType conceptType, BaseType baseType, TYPE v) noexcept
			: BaseValue(std::make_tuple(name, valueString, conceptType, baseType)), jvalue_(v) {}

		JsonObject(std::string valueString, ConceptType conceptType, BaseType baseType, TYPE v) noexcept 
			: BaseValue(std::make_tuple("", valueString, conceptType, baseType)), jvalue_(v) {}

		JsonObject(std::tuple<std::string, std::string, ConceptType, BaseType> metaInfo, TYPE value) noexcept
			: BaseValue(metainfo), jvalue_(value) {}
		
		template <typename Visitor>
		void accept(Visitor& v) {
			v.visit(*this);
		}
		
		TYPE GetValue() {
			return jvalue_;
		}

		template <typename = std::enable_if_t<is_container::value>>
		void insert(BaseValue* item) {
			jvalue_.push_back(item);
		}
	};
}