#pragma once
#include <vector>
#include <string>
#include <utility>
#include <iostream>
#include <sstream>
#include <type_traits>
#include "EBaseType.h"
// for const iterator, all containers have a const iterator

namespace jserialization {

	template<typename T>
	struct has_const_iterator
	{
	private:
		typedef char one;

		template<typename C> static one test(typename C::const_iterator*);
	public:
		static const bool value = sizeof(test<T>(0)) == sizeof(one);
		typedef T type;
	};

	// for iterator begin and end, all containers have a const begin and end
	template <typename T>
	struct has_begin_end
	{
		struct Dummy { typedef void const_iterator; };
		typedef typename std::conditional<has_const_iterator<T>::value, T, Dummy>::type TType;
		typedef typename TType::const_iterator iter;

		struct Fallback { iter begin() const; iter end() const; };
		struct Derived : TType, Fallback { };

		template<typename C, C> struct ChT;

		template<typename C> static char(&f(ChT<iter(Fallback::*)() const, &C::begin>*))[1];
		template<typename C> static char(&f(...))[2];
		template<typename C> static char(&g(ChT<iter(Fallback::*)() const, &C::end>*))[1];
		template<typename C> static char(&g(...))[2];

		static bool const beg_value = sizeof(f<Derived>(0)) == 2;
		static bool const end_value = sizeof(g<Derived>(0)) == 2;
	};

	template <typename T>
	struct is_container
	{
		static const bool value = has_const_iterator<T>::value &&
			has_begin_end<T>::beg_value && has_begin_end<T>::end_value
			&& !is_string<T>::value;
	};

	template <typename target_type, typename actual_type>
	bool test_upper_bound(const actual_type n)
	{
		typedef typename std::common_type<target_type, actual_type>::type common_type;
		const auto c_n = static_cast<common_type>(n);
		const auto t_max = static_cast<common_type>(std::numeric_limits<target_type>::max());
		return (c_n <= t_max);
	}

	// the lower bound is only needed to be checked explicitely in non-trivial cases, see the next to functions
	template <typename target_type, typename actual_type>
	typename std::enable_if<!(std::is_unsigned<target_type>::value), bool>::type
		test_lower_bound(const actual_type n)
	{
		typedef typename std::common_type<target_type, actual_type>::type common_type;
		const auto c_n = static_cast<common_type>(n);
		const auto t_min = static_cast<common_type>(std::numeric_limits<target_type>::lowest());
		return (c_n >= t_min);
	}

	// for unsigned target types, the sign of n musn't be negative
	// but that's not an issue with unsigned actual_type
	template <typename target_type, typename actual_type>
	typename std::enable_if<std::is_integral<target_type>::value &&
		std::is_unsigned<target_type>::value &&
		std::is_integral<actual_type>::value &&
		std::is_unsigned<actual_type>::value, bool>::type
		test_lower_bound(const actual_type)
	{
		return true;
	}

	// for unsigned target types, the sign of n musn't be negative
	template <typename target_type, typename actual_type>
	typename std::enable_if<std::is_integral<target_type>::value &&
		std::is_unsigned<target_type>::value &&
		(!std::is_integral<actual_type>::value ||
			!std::is_unsigned<actual_type>::value), bool>::type
		test_lower_bound(const actual_type n)
	{
		return (n >= 0);
	}

	// value may be integral if the target type is non-integral
	template <typename target_type, typename actual_type>
	typename std::enable_if<!std::is_integral<target_type>::value, bool>::type
		test_integrality(const actual_type)
	{
		return true;
	}

	// value must be integral if the target type is integral
	template <typename target_type, typename actual_type>
	typename std::enable_if<std::is_integral<target_type>::value, bool>::type
		test_integrality(const actual_type n)
	{
		return ((std::abs(n - std::floor(n)) < 1e-8) || (std::abs(n - std::ceil(n)) < 1e-8));
	}

	// perform check only if non-trivial
	template <typename target_type, typename actual_type>
	const typename std::enable_if<!std::is_same<target_type, actual_type>::value, bool>::type
		CanTypeFitValue(const actual_type n)
	{
		return test_upper_bound<target_type>(n) &&
			test_lower_bound<target_type>(n) &&
			test_integrality<target_type>(n);
	}


	// trivial case: actual_type == target_type
	template <typename actual_type>
	const bool CanTypeFitValue(const actual_type)
	{
		return true;
	}

	template<typename T>
	inline bool StrToNumber(const std::string& str, T& val)
	{
		std::istringstream i(str);
		if (!(i >> std::boolalpha >> val)) {
			return false;
		}
		return true;
	};

	bool AttemptNumberFitting(const std::string& str, BaseType base_type);

	template<typename T>
	struct base_array_type {
		typedef std::remove_pointer<std::decay<T>> type;
	};

	template<typename T, typename = std::enable_if_t<std::is_array<T>>>
	struct array_properties {
		static int const size = sizeof(T) / sizeof(base_array_type<T>::type);
	};

	template<typename T>
	struct is_text {
		typedef std::conditional_t<
			std::is_same<std::remove_const_t<T>, char*>::value || std::is_same<T, std::string>::value, 
			std::true_type, std::false_type> value;
	};
}