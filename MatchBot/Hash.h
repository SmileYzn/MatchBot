#pragma once

//
// http://www.isthe.com/chongo/tech/comp/fnv/
//
namespace Hash
{
	namespace internal
	{
		constexpr unsigned int Basis{ 0x811c9dc5 };
		constexpr unsigned int Prime{ 0x1000193 };
	}

	static inline constexpr unsigned int GetConst(const char* string, const unsigned int value = internal::Basis) noexcept
	{
		return (string[ 0 ] == '\0') ? value : GetConst(&string[1], (value ^ static_cast<unsigned int>(string[0])) * internal::Prime);
	}

	static inline unsigned int Get(const std::string_view& string)
	{
		auto result { internal::Basis };

		for ( auto i{ 0u }; i < string.size( ); ++i )
		{
			result ^= string[ i ];
			result *= internal::Prime;
		}

		return result;
	}
}
