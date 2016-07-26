#pragma once
#include <string>
#include <vector>
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
	};

	template<typename T, typename = std::enable_if_t<is_container<T>>>
	Holder(Type type, T val) {
		impl_ = val;
		size_ = val.size_t();
	}

	~Holder() {
	}

	operator bool() const {
		if (type_ != BOOL) {
			//throw SomeException();
		}
		return impl_.bool_;
	}
	// Do the same for other operators
	// Or maybe use templates?

private:

	union Impl {
		char char_;
		unsigned char u_char_;
		short int_8_;
		unsigned short u_int_8_;
		int    int_16_;
		unsigned int u_int_16_;
		long int_32_;
		unsigned long u_int_32_;
		long long int_64_;
		unsigned long long u_int_64_;
		float float_;
		double double_;
		long double long_double_;
		bool   bool_;
		std::string string_;
		std::vector<Holder*> vector_;
		std::array<Holder*, SIZE> array_;

		Impl() { new(&string_) std::string; }
		~Impl() {}
	} impl_;

	Type type_;

};