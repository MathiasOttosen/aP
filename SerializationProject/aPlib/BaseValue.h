#pragma once
#include <string>
#include <vector>
#include <tuple>
#include <list>
#include "TypeDeductionHelper.h"
#include "EConceptType.h"
#include "EBaseType.h"

namespace jserialization {

	class BaseValue {

		int id;
		const std::string name_;
		const std::string value_;
		const ConceptType concept_type_;
		BaseType base_type_; 

		std::vector<BaseValue*> valueList_;

		std::list<int> valueIdList_;

	public:

		BaseValue(const std::tuple<std::string, std::string, ConceptType, BaseType>& concept) noexcept;

		BaseValue(const std::tuple<std::string, std::string, ConceptType, BaseType>& concept, std::list<int>& subobjects) noexcept;

		virtual ~BaseValue();

		template <typename Visitor>
		void accept(Visitor& v) {
			CallAcceptByBaseType(v);

		}

		template <typename Visitor>
		void CallAcceptByBaseType(Visitor& v) noexcept {
			switch (base_type_) {
			case btype::CHAR:
				CallAcceptByConceptType<int8_t>(v);
				break;
			case btype::UCHAR:
				CallAcceptByConceptType<uint8_t>(v);
				break;
			case btype::INT:

				CallAcceptByConceptType<int16_t>(v);
				break;
			case btype::UINT:

				CallAcceptByConceptType<uint16_t>(v);
				break;
			case btype::LONG:

				CallAcceptByConceptType<int32_t>(v);
				break;
			case btype::ULONG:

				CallAcceptByConceptType<uint32_t>(v);
				break;
			case btype::LONGLONG:

				CallAcceptByConceptType<int64_t>(v);
				break;
			case btype::ULONGLONG:

				CallAcceptByConceptType<uint64_t>(v);
				break;
			case btype::FLOAT:

				CallAcceptByConceptType<float>(v);
				break;
			case btype::DOUBLE:

				CallAcceptByConceptType<double>(v);
				break;
			case btype::LONGDOUBLE:

				CallAcceptByConceptType<long double>(v);
				break;
			case btype::STRING:
				CallAcceptByConceptType<std::string>(v);
				break;
			case btype::BOOL:
				CallAcceptByConceptType<bool>(v);
				break;
			case btype::CUSTOM:
				CallAcceptByConceptType<BaseValue*>(v);
				break;
			case btype::UID:
				//attempt to find an approriate type 
				break;
			}
		}

		template <typename Visitor, typename T>
		void CallSimpleSpecificAccept(Visitor& v) {
			JsonObject<T>* derived = dynamic_cast<JsonObject<T>*>(this);
			if (derived) {
				derived->accept(v);
			}
		}

		template<typename Visitor, typename T>
		void CallInitializerSpecificAccept(Visitor& v) {
			CallSimpleSpecificAccept<std::initializer_list<T>();
		}

		template<typename Visitor, typename T>
		void CallArraySpecificAccept(Visitor& v) {
			CallSimpleSpecificAccept<T[]>(v);
		}

		template<typename Visitor, typename T>
		void CallVectorSpecificAccept(Visitor& v) {
			CallSimpleSpecificAccept<std::vector<T>>(v);
		}

		template <typename Visitor, typename T>
		void CallAcceptByConceptType(Visitor& v) noexcept {
			switch (concept_type_) {
			case ConceptType::STRING:
			case ConceptType::NUMBER:
				CallSimpleSpecificAccept<T>(v);
				break;
			case ConceptType::ARRAY:
				CallArraySpecificAccept<T>(v);
				break;
			case ConceptType::INITIALIZERLIST:
				CallInitializerSpecificAccept<T>(v);
				break;
			case ConceptType::CONTAINER:
				CallSimpleSpecificAccept<BaseValue*>(v);
				break;
			}
		}

		int GetBaseId() const noexcept;

		std::string GetBaseName() const noexcept;

		std::string GetBaseValue() const noexcept;

		ConceptType GetConceptType() const noexcept;

		std::string GetConceptTypeAsStr() const noexcept;

		BaseType GetBaseType() noexcept;

		BaseType GetCBaseType() const noexcept;

		std::string GetBaseTypeAsStr() const noexcept;

		int GetSize() const noexcept;

		std::list<int> GetValueIdList() const noexcept;

		void InsertValueId(int& id) noexcept;

		std::string IdListStr() const noexcept;

		std::string ToStr() noexcept;

		std::list<std::string> GetMemberList() const noexcept;

		std::list<std::string> GetEmptyMemberList() const noexcept;

		bool operator<(const BaseValue& other) noexcept;

		bool compare(std::string& name, std::string& value, std::string& concept_type,
			std::string& base_type, int& size, std::list<int>& subobjects) noexcept;
	private:

		void DetermineBaseType() noexcept;
	};
}