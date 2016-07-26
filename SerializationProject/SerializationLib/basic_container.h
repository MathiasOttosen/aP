#pragma once
#include <string>
#include <tuple>
namespace jserialization {
	template <typename TYPE>
	class basic_container {

	public:

		typedef TYPE valueType;

		const std::string name;

		const std::string rawValue;

		const valueType value;

		basic_container(std::string valueString, valueType v) : name(""), rawValue(valueString), value(v) {
		}

		basic_container(std::tuple<std::string, std::string, valueType> metaInfo)
			: name(std::get<0>(metaInfo)),
			rawValue(std::get<1>(metaInfo)),
			value(std::get<2>(metaInfo)) {

		}

		template <typename Visitor>
		void accept(Visitor& v) {
			v.visit(*this);
		}
	};
}