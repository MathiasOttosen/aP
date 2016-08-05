#include "BaseValue.h"

using namespace jserialization;

BaseValue::BaseValue(std::tuple<std::string, std::string, ConceptType, BaseType> concept)
	: name_(std::get<0>(concept)), value_(std::get<1>(concept)), concept_type_(std::get<2>(concept)), base_type_(std::get<3>(concept)) {}

BaseValue::~BaseValue() {};


std::string BaseValue::GetBaseName() const noexcept {
	return name_;
}

std::string BaseValue::GetBaseValue() const noexcept {
	return value_;
}

ConceptType BaseValue::GetConceptType() const noexcept {
	return concept_type_;
}

BaseType BaseValue::GetBaseType() noexcept {
	if (base_type_ == BaseType::UID) {
		DetermineBaseType();
	}
	return base_type_;
}

int BaseValue::GetSize() const noexcept {
	return valueList_.size();
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