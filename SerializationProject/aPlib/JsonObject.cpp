#include "jsonobject.h"

template<typename T>
inline jserialization::JsonObject<T>::JsonObject(std::string name, std::string valueString, ConceptType conceptType, BaseType baseType, T v) noexcept
	: BaseValue(std::make_tuple(name, valueString, conceptType, baseType)), jvalue_(v) {}

template<typename T>
inline jserialization::JsonObject<T>::JsonObject(std::string valueString, ConceptType conceptType, BaseType baseType, T v) noexcept
	: BaseValue(std::make_tuple("", valueString, conceptType, baseType)), jvalue_(v) {}

template<typename T>
inline jserialization::JsonObject<T>::JsonObject(std::tuple<std::string, std::string, ConceptType, BaseType> metaInfo, T value) noexcept
	: BaseValue(metaInfo), jvalue_(value) {}

template<typename T>
inline jserialization::JsonObject<T>::~JsonObject(){
	~BaseValue();
}