#pragma once
#include <memory>
#include <string>
class JsonValue {
	struct JsonValueConcept {
		virtual ~JsonValueConcept() {}
		virtual std::string name() const = 0;
		
		/*template <typename Visitor>
		virtual void accept(Visitor& v) {
			v.visit();
		}*/
	};

	template< typename T > struct JsonValueConcrete {
		JsonValueConcrete(const T& t) : jvalue(t) {}
		~JsonValueConcrete() {}
		std::string name() const
		{
			return typeid(jvalue).name;
		}
	private:
		T jvalue;
	};

	std::shared_ptr<JsonValueConcept> jvalue;

public:
	template< typename T > JsonValue(const T& obj) :
		jvalue(new JsonValueConcrete<T>(obj)) {}

	std::string name() const
	{
		return jvalue->name();
	}
};
