#pragma once
#include <string>
#include <vector>
#include <memory>
#include "JsonObject.h"
template <int SIZE = 1>
class Holder {
public:
	enum Type {
		CHAR,
		UCHAR,
		SHORT,
		USHORT,
		INT,
		UINT,
		LONG,
		ULONG,
		LONGLONG,
		ULONGLONG,
		FLOAT,
		DOUBLE,
		LONGDOUBLE,
		BOOL,
		STRING, 
		VECTOR,
		ARRAY,
		CHARARR,
		UCHARARR,
		SHORTARR,
		USHORTARR,
		INTARR,
		UINTARR,
		LONGARR,
		ULONGARR,
		LONGLONGARR,
		ULONGLONGARR,
		FLOATARR,
		DOUBLEARR,
		LONGDOUBLEARR,
		BOOLARR,
		STRINGARR,
	};

	template<typename T>
	Holder(Type type, T val) : holderconcept(val) {
		type_ = type;
	}

	struct HolderConcept {
		virtual ~Holder() {};

	};

	template< typename T > struct HolderStruct : HolderConcept {
		HolderStruct(const T& t) { impl_ = t; size_t_ = t.sizeof(); }
		virtual ~HolderStruct() {}
	private:
		size_t = size_t_;
		T jvalue;
	};

	std::shared_ptr<HolderConcept> jvalue;
	std::shared_ptr<HolderConcept> size_t_;

	~Holder() {
	}

	jvalue value() const{
		switch (type_) {
		case BOOL:
			return impl_.bool_;
		case INT:
			return impl_.int_;
			//and so on
		}
	}

private:

	union Impl {
		char char_;
		unsigned char u_char_;
		short int_8_;
		unsigned short u_int_8_;
		int int_16_;
		unsigned int u_int_16_;
		long int_32_;
		unsigned long u_int_32_;
		long long int_64_;
		unsigned long long u_int_64_;
		float float_;
		double double_;
		long double long_double_;
		bool bool_;
		std::string string_;
		std::vector<Holder*> vector_;
		std::array<Holder*, SIZE> array_;
		char[] char_array_;
		unsigned char[] u_char_array_;
		short[] int_8_array_;
		unsigned short[] u_int_8_array_;
		int[] int_16_array_;
		unsigned int[] u_int_16_array_;
		long[] int_32_array_;
		unsigned long[] u_int_32_array_;
		long long[] int_64_array_;
		unsigned long long[] u_int_64_array_;
		float[] float_array_;
		double[] double_array_;
		long double[] long_double_array_;
		bool[] bool_array_;
		std::string[] string_array_;

		Impl() { new(&string_) std::string; }
		~Impl() {}
	} impl_;

	HolderConcept holderconcept;
	Type type_;

};