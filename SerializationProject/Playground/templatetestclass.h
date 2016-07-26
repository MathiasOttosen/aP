#pragma once
#include <string>

template <typename T>
class TemplateTestClass<T> {
public:
	T t;

	TemplateTestClass() {}
	~TemplateTestClass() {}

	TemplateTestClass(T value) : t(value) {}
};