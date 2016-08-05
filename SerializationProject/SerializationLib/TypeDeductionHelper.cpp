#include "TypeDeductionHelper.h"

bool jserialization::AttemptNumberFitting(const std::string& str, BaseType base_type) {
	double tmpVal;
	base_type = BaseType::UID;
	auto converted = StrToNumber<double>(str, tmpVal);

	if (!converted) {
		return false;
	}
	auto fitted = false;

	if (converted && !fitted) {
		fitted = CanTypeFitValue<int8_t>(tmpVal);
		if (fitted) {
			base_type = BaseType::CHAR;
		}
	}
	if (converted && !fitted) {
		fitted = CanTypeFitValue<uint8_t>(tmpVal);
		if (fitted) {
			base_type = BaseType::UCHAR;
		}
	}
	if (converted && !fitted) {
		fitted = CanTypeFitValue<int16_t>(tmpVal);
		if (fitted) {
			base_type = BaseType::INT;
		}
	}
	if (converted && !fitted) {
		fitted = CanTypeFitValue<uint16_t>(tmpVal);
		if (fitted) {
			base_type = BaseType::UINT;
		}
	}
	if (converted && !fitted) {
		fitted = CanTypeFitValue<int32_t>(tmpVal);
		if (fitted) {
			base_type = BaseType::LONG;
		}
	}
	if (converted && !fitted) {
		fitted = CanTypeFitValue<uint32_t>(tmpVal);
		if (fitted) {
			base_type = BaseType::ULONG;
		}
	}
	if (converted && !fitted) {
		fitted = CanTypeFitValue<int64_t>(tmpVal);
		if (fitted) {
			base_type = BaseType::LONGLONG;
		}
	}
	if (converted && !fitted) {
		fitted = CanTypeFitValue<uint64_t>(tmpVal);
		if (fitted) {
			base_type = BaseType::ULONGLONG;
		}
	}
	if (converted && !fitted) {
		fitted = CanTypeFitValue<float>(tmpVal);
		if (fitted) {
			base_type = BaseType::FLOAT;
		}
	}
	if (converted && !fitted) {
		fitted = CanTypeFitValue<double>(tmpVal);
		if (fitted) {
			base_type = BaseType::DOUBLE;
		}
	}
	if (converted && !fitted) {
		fitted = CanTypeFitValue<long double>(tmpVal);
		if (fitted) {
			base_type = BaseType::LONGDOUBLE;
		}
	}
	return fitted;
}