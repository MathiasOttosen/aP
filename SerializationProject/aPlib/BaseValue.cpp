#include "BaseValue.h"
#include <map>

using namespace jserialization;

BaseValue::BaseValue(const std::tuple<std::string, std::string, ConceptType, BaseType>& concept) noexcept
	: name_(std::get<0>(concept)), value_(std::get<1>(concept)), concept_type_(std::get<2>(concept)), base_type_(std::get<3>(concept))  {}

BaseValue::BaseValue(const std::tuple<std::string, std::string, ConceptType, BaseType>& concept, std::list<int>& subobjects) noexcept
	: name_(std::get<0>(concept)), value_(std::get<1>(concept)), concept_type_(std::get<2>(concept)), base_type_(std::get<3>(concept)) {
	valueIdList_ = { subobjects };
}

BaseValue::~BaseValue() {
	for (auto v = valueList_.begin(); v != valueList_.end(); v++) {
		delete *v;
	}
	/*for (auto it = (*valueIdList_).begin(); it != (*valueIdList_).end(); it++) {
		delete *it;
	}*/
};

int BaseValue::GetBaseId() const noexcept {
	return id;
}

std::string BaseValue::GetBaseName() const noexcept {
	return name_;
}

std::string BaseValue::GetBaseValue() const noexcept {
	return value_;
}

ConceptType BaseValue::GetConceptType() const noexcept {
	return concept_type_;
}

std::string BaseValue::GetConceptTypeAsStr() const noexcept {
	return CONCEPTTYPE_STRING.at(concept_type_);
}

BaseType BaseValue::GetBaseType() noexcept {
	if (base_type_ == BaseType::UID) {
		DetermineBaseType();
	}
	return base_type_;
}

BaseType BaseValue::GetCBaseType() const noexcept {
	return base_type_;
}

std::string BaseValue::GetBaseTypeAsStr() const noexcept {
	return BASETYPE_STRING.at(base_type_);
}

std::list<int> BaseValue::GetValueIdList() const noexcept {
	return valueIdList_;
}

void BaseValue::InsertValueId(int& id) noexcept {
	valueIdList_.push_back(id);
}

int BaseValue::GetSize() const noexcept {
	return valueIdList_.size();
}

std::list<std::string> BaseValue::GetMemberList() const noexcept {
	return std::list<std::string> { GetBaseName(), GetBaseValue(),
		GetConceptTypeAsStr(), GetBaseTypeAsStr(), std::to_string(GetSize()) };
}

std::list<std::string> BaseValue::GetEmptyMemberList() const noexcept {
	std::list<std::string> lst = { "", "", "", "", "" }; //name, value, concepttype, basetype, size
	for (auto it = valueIdList_.begin(); it != valueIdList_.end(); it++) {
		lst.push_back("");
	}
	return lst;
}
std::string BaseValue::IdListStr() const noexcept {
	std::string str = "";
	for (auto i = valueIdList_.begin(); i != valueIdList_.end(); i++) {
		str += std::to_string(*i);
		str += ',';
	}
	return str;

}

std::string BaseValue::ToStr() noexcept{
	BaseType bt = GetBaseType();
	std::string bs = BASETYPE_STRING.at(bt);
	return "JsonObject" + std::string(bs) + GetBaseValue() + IdListStr();
}

bool BaseValue::operator<(const BaseValue& other) noexcept{
	return other.GetBaseName().compare(GetBaseName()) == 0 && other.GetBaseValue().compare(GetBaseValue()) == 0 &&
		other.GetConceptType() == GetConceptType() && other.GetCBaseType() == GetCBaseType() &&
		GetSize() == GetSize() && std::equal(std::begin(GetValueIdList()), std::end(GetValueIdList()), std::begin(other.GetValueIdList()));
}

bool BaseValue::compare(std::string& name, std::string& value, std::string& concept_type,
	std::string& base_type, int& size, std::list<int>& subobjects) noexcept{
	return name.compare(GetBaseName()) == 0 && value.compare(GetBaseValue()) == 0 &&
		compareWithConceptEnum(concept_type, GetConceptType()) && compareWithBaseEnum(base_type, GetBaseType()) &&
		size == GetSize() && std::equal(std::begin(subobjects), std::end(subobjects), std::begin(GetValueIdList()));

}

void BaseValue::DetermineBaseType() noexcept {
	if (concept_type_ == ConceptType::STRING) {
		base_type_ = BaseType::STRING;
	}
	if (concept_type_ == ConceptType::NUMBER) {
		AttemptNumberFitting(value_, base_type_);
	}
	if (concept_type_ == ConceptType::ARRAY) {
		base_type_ = BaseType::CUSTOM;
	}
	if (concept_type_ == ConceptType::INITIALIZERLIST) {
		if (!valueList_.empty()) {
			base_type_ = valueList_.front()->GetBaseType();
		}
		else
			base_type_ = BaseType::UID;
	}
	if (concept_type_ == ConceptType::CONTAINER) {
		base_type_ = BaseType::CUSTOM;
	}
}