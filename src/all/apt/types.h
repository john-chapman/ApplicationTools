#pragma once
#ifndef apt_types_h
#define apt_types_h

#include <cfloat>
#include <cstddef>
#include <cstdint>
#include <limits>

namespace apt { namespace refactor {

enum DataType
{
	DataType_Invalid,

 // integer types
	DataType_Sint8,
	DataType_Uint8,
	DataType_Sint16,
	DataType_Uint16,
	DataType_Sint32,
	DataType_Uint32,
	DataType_Sint64,		
	DataType_Uint64,

 // normalized integer types
	DataType_Sint8N,
	DataType_Uint8N,
	DataType_Sint16N,
	DataType_Uint16N,
	DataType_Sint32N,
	DataType_Uint32N,
	DataType_Sint64N,
	DataType_Uint64N,

 // float types
	DataType_Float16,
	DataType_Float32,
	DataType_Float64,

	DataType_Count,
	DataType_Sint  = DataType_Sint64,
	DataType_Uint  = DataType_Uint64,
	DataType_Float = DataType_Float32,
};


inline constexpr bool DataTypeIsInt(DataType _type)
{
	return _type >= DataType_Sint8 && _type <= DataType_Uint64N;
}
inline constexpr bool DataTypeIsFloat(DataType _type)
{
	return _type >= DataType_Float16 && _type <= DataType_Float64;
}
inline constexpr bool DataTypeIsSigned(DataType _type)
{
	return (_type % 2) != 0 || _type >= DataType_Float16;
}
inline constexpr bool DataTypeIsNormalized(DataType _type)
{
	return _type >= DataType_Sint8N && _type <= DataType_Uint64N;
}


namespace internal {

template <typename tBase, DataType kEnum>
struct DataTypeBase
{
	typedef tBase BaseType;
	static const DataType Enum = kEnum;
	static const DataTypeBase<tBase, kEnum> kMin;
	static const DataTypeBase<tBase, kEnum> kMax;

	DataTypeBase() = default;

	template <typename tValue>
	DataTypeBase(tValue _value): m_val(_value) {}

	operator BaseType&()                       { return m_val; }
	operator const BaseType&() const           { return m_val; }
private:
	BaseType m_val;
};

} // namespace internal


// Sized integer types.
typedef internal::DataTypeBase<std::int8_t,   DataType::DataType_Sint8>   sint8;
typedef internal::DataTypeBase<std::uint8_t,  DataType::DataType_Uint8>   uint8;
typedef internal::DataTypeBase<std::int16_t,  DataType::DataType_Sint16>  sint16;
typedef internal::DataTypeBase<std::uint16_t, DataType::DataType_Uint16>  uint16;
typedef internal::DataTypeBase<std::int32_t,  DataType::DataType_Sint32>  sint32;
typedef internal::DataTypeBase<std::uint32_t, DataType::DataType_Uint32>  uint32;
typedef internal::DataTypeBase<std::int64_t,  DataType::DataType_Sint64>  sint64;
typedef internal::DataTypeBase<std::uint64_t, DataType::DataType_Uint64>  uint64;

// Sized normalized integer types (use DataTypeConvert() for conversion to/from floating point types).
typedef internal::DataTypeBase<std::int8_t,   DataType::DataType_Sint8N>  sint8N;
typedef internal::DataTypeBase<std::uint8_t,  DataType::DataType_Uint8N>  uint8N;
typedef internal::DataTypeBase<std::int16_t,  DataType::DataType_Sint16N> sint16N;
typedef internal::DataTypeBase<std::uint16_t, DataType::DataType_Uint16N> uint16N;
typedef internal::DataTypeBase<std::int32_t,  DataType::DataType_Sint32N> sint32N;
typedef internal::DataTypeBase<std::uint32_t, DataType::DataType_Uint32N> uint32N;
typedef internal::DataTypeBase<std::int64_t,  DataType::DataType_Sint64N> sint64N;
typedef internal::DataTypeBase<std::uint64_t, DataType::DataType_Uint64N> uint64N;

// Sized floating point types.
// \note float16 is a storage type only and has no arithmetic operators.
typedef internal::DataTypeBase<std::uint16_t, DataType::DataType_Float16> float16;
typedef internal::DataTypeBase<float,         DataType::DataType_Float32> float32;
typedef internal::DataTypeBase<double,        DataType::DataType_Float64> float64;

typedef std::ptrdiff_t sint;
typedef std::size_t    uint;


namespace internal {

// Instantiate _macro for all type/datatype pairs
#define APT_DataType_decl_refactor(_macro) \
	_macro(apt::refactor::sint8,   apt::refactor::DataType_Sint8  ) \
	_macro(apt::refactor::uint8,   apt::refactor::DataType_Uint8  ) \
	_macro(apt::refactor::sint16,  apt::refactor::DataType_Sint16 ) \
	_macro(apt::refactor::uint16,  apt::refactor::DataType_Uint16 ) \
	_macro(apt::refactor::sint32,  apt::refactor::DataType_Sint32 ) \
	_macro(apt::refactor::uint32,  apt::refactor::DataType_Uint32 ) \
	_macro(apt::refactor::sint64,  apt::refactor::DataType_Sint64 ) \
	_macro(apt::refactor::uint64,  apt::refactor::DataType_Uint64 ) \
	_macro(apt::refactor::sint8N,  apt::refactor::DataType_Sint8N ) \
	_macro(apt::refactor::uint8N,  apt::refactor::DataType_Uint8N ) \
	_macro(apt::refactor::sint16N, apt::refactor::DataType_Sint16N) \
	_macro(apt::refactor::uint16N, apt::refactor::DataType_Uint16N) \
	_macro(apt::refactor::sint32N, apt::refactor::DataType_Sint32N) \
	_macro(apt::refactor::uint32N, apt::refactor::DataType_Uint32N) \
	_macro(apt::refactor::sint64N, apt::refactor::DataType_Sint64N) \
	_macro(apt::refactor::uint64N, apt::refactor::DataType_Uint64N) \
	_macro(apt::refactor::float16, apt::refactor::DataType_Float16) \
	_macro(apt::refactor::float32, apt::refactor::DataType_Float32) \
	_macro(apt::refactor::float64, apt::refactor::DataType_Float64)

template <DataType kEnum> struct DataType_EnumToType {};
	#define APT_DataType_EnumToType(_type, _enum) \
		template<> struct DataType_EnumToType<_enum> { typedef _type Type; };
	APT_DataType_decl_refactor(APT_DataType_EnumToType)
	#undef APT_DataType_EnumToType
	template<> struct DataType_EnumToType<DataType_Invalid> { typedef apt::refactor::sint8 Type; }; // required so that ToType<(DataType::Enum)(kSint8 - 1)> will compile

#define APT_DATA_TYPE_FROM_ENUM(_enum) typename apt::refactor::internal::DataType_EnumToType<(_enum)>::Type


// Conversion helpers
template <typename tDst, typename tSrc>
inline tDst DataType_FloatToIntN(tSrc _src)
{
	APT_ASSERT(DataTypeIsNormalized(tDst::Enum));
	APT_ASSERT(DataTypeIsFloat(tSrc::Enum));
	_src = APT_CLAMP(_src, (tSrc)-1, (tSrc)1);
	return _src < 0 ? (tDst)-(_src * (tSrc)tDst::kMin)
	                : (tDst) (_src * (tSrc)tDst::kMax);
}
template <typename tDst, typename tSrc>
inline tDst DataType_IntNToFloat(tSrc _src)
{
	APT_ASSERT(DataTypeIsFloat(tDst::Enum));
	APT_ASSERT(DataTypeIsNormalized(tSrc::Enum));
	return _src < 0 ? -((tDst)_src / (tDst)tSrc::kMin)
	                :  (tDst)_src / (tDst)tSrc::kMax;
}
template <typename tDst, typename tSrc>
inline tDst DataType_IntNPrecisionChange(tSrc _src)
{
	APT_ASSERT(DataTypeIsSigned(tSrc::Enum) == DataTypeIsSigned(tDst::Enum)); // perform signed -> unsigned conversion before precision change
	if (sizeof(tSrc) > sizeof(tDst)) {
		return (tDst)(_src < 0 ? -(_src / (tSrc::kMin / tDst::kMin))
		                       :   _src / (tSrc::kMax / tDst::kMax));
	} else if (sizeof(tSrc) < sizeof(tDst)) {
		return (tDst)(_src < 0 ? -(_src * (tDst::kMin / tSrc::kMin))
	                           :   _src * (tDst::kMax / tSrc::kMax));
	} else {
		return _src;
	}
}
template <typename tDst, typename tSrc>
inline tDst DataType_IntNToIntN(tSrc _src)
{
	if (DataTypeIsSigned(tSrc::Enum) && !DataTypeIsSigned(tDst::Enum)) { // signed -> unsigned
		typedef APT_DATA_TYPE_FROM_ENUM((DataType)(tSrc::Enum + 1)) tUnsigned;
		tUnsigned tmp = (tUnsigned)(_src * 2);
		return DataType_IntNPrecisionChange<tDst, tUnsigned>(tmp);
	} else if (!DataTypeIsSigned(tSrc::Enum) && DataTypeIsSigned(tDst::Enum)) { // unsigned -> signed
		typedef APT_DATA_TYPE_FROM_ENUM((DataType)(tSrc::Enum - 1)) tSigned;
		tSigned tmp = (tSigned)(_src / 2);
		return DataType_IntNPrecisionChange<tDst, tSigned>(tmp);
	} else {
		return DataType_IntNPrecisionChange<tDst, tSrc>(_src);
	}
}

} // namespace internal

// Convert from tSrc -> tDst.
// \note float32/64 -> *32N/*64N conversion has poor precision.
template <typename tDst, typename tSrc>
inline tDst DataTypeConvert(tSrc _src)
{
	if (tSrc::Enum == tDst::Enum) {
		return _src;
	}
	if (DataTypeIsNormalized(tSrc::Enum) && DataTypeIsNormalized(tDst::Enum)) {
		return internal::DataType_IntNToIntN<tDst>(_src);
	} else if (DataTypeIsFloat(tSrc::Enum) && DataTypeIsNormalized(tDst::Enum)) {
		return internal::DataType_FloatToIntN<tDst, tSrc>(_src);
	} else if (DataTypeIsNormalized(tSrc::Enum) && DataTypeIsFloat(tDst::Enum)) {
		return internal::DataType_IntNToFloat<tDst, tSrc>(_src);
	} else {
		return (tDst)_src;
	}
	APT_ASSERT(false);
	return (tDst)0;
}

} } // namespace apt

// --
#include <cfloat>
#include <cstdint>
#include <cmath>
#include <limits>

#ifdef _MSC_VER
	#pragma warning(push)
	#pragma warning(disable: 4244) // possible loss of data
#endif

namespace apt {

typedef std::int8_t     sint8;
typedef std::uint8_t    uint8;
typedef std::int16_t    sint16;
typedef std::uint16_t   uint16;
typedef std::int32_t    sint32;
typedef std::uint32_t   uint32;
typedef std::int64_t    sint64;
typedef std::uint64_t   uint64;
typedef float           float32;
typedef double          float64;
typedef std::ptrdiff_t  sint;
typedef std::size_t     uint;


struct float16 
{ 
	uint16 m_val;
};

namespace internal {
	template <typename tType>
	class normalized_int
	{
		tType m_val;
	public:
		typedef tType BaseType;

		normalized_int() {}		
		template <typename tSrc>
		normalized_int(tSrc _src): m_val(_src) {}


		operator tType&() { return m_val; }

	}; // class normalized_int

} // namespace internal

typedef internal::normalized_int<sint8>  sint8N;
typedef internal::normalized_int<uint8>  uint8N;
typedef internal::normalized_int<sint16> sint16N;
typedef internal::normalized_int<uint16> uint16N;
typedef internal::normalized_int<sint32> sint32N;
typedef internal::normalized_int<uint32> uint32N;
typedef internal::normalized_int<sint64> sint64N;
typedef internal::normalized_int<uint64> uint64N;

struct DataType
{
	enum Enum {
		InvalidType,

		Sint8,
		Uint8,
		Sint16,
		Uint16,
		Sint32,
		Uint32,
		Sint64,		
		Uint64,
		
		Sint8N,
		Uint8N,
		Sint16N,
		Uint16N,
		Sint32N,
		Uint32N,
		Sint64N,
		Uint64N,

		Float16,
		Float32,
		Float64
	};

	// helper macro; instantiate _macro for all enum-type pairs
	#define APT_DataType_decl(_macro) \
		_macro(Sint8,   sint8)   \
		_macro(Uint8,   uint8)   \
		_macro(Sint16,  sint16)  \
		_macro(Uint16,  uint16)  \
		_macro(Sint32,  sint32)  \
		_macro(Uint32,  uint32)  \
		_macro(Sint64,  sint64)  \
		_macro(Uint64,  uint64)  \
		_macro(Sint8N,  sint8N)  \
		_macro(Uint8N,  uint8N)  \
		_macro(Sint16N, sint16N) \
		_macro(Uint16N, uint16N) \
		_macro(Sint32N, sint32N) \
		_macro(Uint32N, uint32N) \
		_macro(Sint64N, sint64N) \
		_macro(Uint64N, uint64N) \
		_macro(Float16, float16) \
		_macro(Float32, float32) \
		_macro(Float64, float64)

	// Implicit conversions to/from Enum (pass and store DataType, not DataType::Enum).
	DataType(Enum _enum = InvalidType): m_val(_enum)  {}
	operator Enum() const { return m_val; }

	template <typename tType>
	DataType(tType _val): m_val((Enum)_val) {}

	static uint GetSizeBytes(DataType _type)
	{
		#define APT_DataType_case_enum(_enum, _typename) \
			case _enum : return sizeof(_typename);
		switch (_type) {
			APT_DataType_decl(APT_DataType_case_enum)
			default: return 0;
		};
		#undef APT_DataType_case_enum
	}

	static bool IsNormalized(DataType _type) { return (_type >= Sint8N) && (_type <= Uint64N);  }
	static bool IsFloat(DataType _type)      { return (_type >= Float16) && (_type <= Float64); }
	static bool IsInt(DataType _type)        { return !IsFloat(_type); }
	static bool IsSigned(DataType _type)     { return IsFloat(_type) || (_type % 2 != 0); }

	template <typename tType> struct ToEnum {};
		#define APT_DataType_ToEnum(_enum, _typename) \
			template<> struct ToEnum<_typename> { static const DataType::Enum Enum = _enum; };
		APT_DataType_decl(APT_DataType_ToEnum)
		#undef APT_DataType_ToEnum

	template <Enum kEnum> struct ToType {};
		#define APT_DataType_ToType(_enum, _typename) \
			template<> struct ToType<_enum> { typedef _typename Type; };
		APT_DataType_decl(APT_DataType_ToType)
		#undef APT_DataType_ToType
		template<> struct ToType<InvalidType> { typedef sint8 Type; }; // required so that ToType<(DataType::Enum)(kSint8 - 1)> will compile

	/// Copy _count objects from _src to _dst converting from _srcType to _dstType.
	static void Convert(DataType _srcType, DataType _dstType, const void* _src, void* dst_, uint _count = 1);

	/// Convert _src to tDst.
	template <typename tSrc, typename tDst>
	static tDst Convert(tSrc _src);


private:
	Enum m_val;

	#undef APT_DataType_decl

}; // struct DataType

namespace Bitfield {
	// Create a bit mask covering _count bits.
	template <typename tType>
	inline tType Mask(int _count) 
	{ 
		return (1 << _count) - 1; 
	}

	// Create a bit mask covering _count bits starting at _offset.
	template <typename tType>
	inline tType Mask(int _offset, int _count) 
	{ 
		return ((1 << _count) - 1) << _offset; 
	}
	
	// Insert _count least significant bits from _insert into _base at _offset.
	template <typename tType>
	inline tType Insert(tType _base, tType _insert, int _offset, int _count) 
	{ 
		tType mask = Mask<tType>(_count);
		return (_base & ~(mask << _offset)) | ((_insert & mask) << _offset);
	}

	// Extract _count bits from _base starting at _offset into the _count least significant bits of the result.
	template <typename tType>
	inline tType Extract(tType _base, int _offset, int _count) 
	{ 
		tType mask = Mask<tType>(_count) << _offset;
		return (_base & mask) >> _offset;
	}
}

} // namespace apt

namespace std {

/// numeric_limits specializations for normalized int types.
template <> class numeric_limits<apt::sint8N>:  public std::numeric_limits<apt::sint8N::BaseType>  {};
template <> class numeric_limits<apt::uint8N>:  public std::numeric_limits<apt::uint8N::BaseType>  {};
template <> class numeric_limits<apt::sint16N>: public std::numeric_limits<apt::sint16N::BaseType> {};
template <> class numeric_limits<apt::uint16N>: public std::numeric_limits<apt::uint16N::BaseType> {};
template <> class numeric_limits<apt::sint32N>: public std::numeric_limits<apt::sint32N::BaseType> {};
template <> class numeric_limits<apt::uint32N>: public std::numeric_limits<apt::uint32N::BaseType> {};
template <> class numeric_limits<apt::sint64N>: public std::numeric_limits<apt::sint64N::BaseType> {};
template <> class numeric_limits<apt::uint64N>: public std::numeric_limits<apt::uint64N::BaseType> {};

} // namespace std

#ifdef _MSC_VER
	#pragma warning(pop)
#endif

#endif // apt_types_h
