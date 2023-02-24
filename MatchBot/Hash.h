#pragma once

//
// http://www.isthe.com/chongo/tech/comp/fnv/
//
namespace Hash
{
	namespace internal
	{
		constexpr std::uint32_t Basis{ 0x811c9dc5 };
		constexpr std::uint32_t Prime{ 0x1000193 };
	}

	static inline constexpr std::uint32_t GetConst( const char* string, const std::uint32_t value = internal::Basis ) noexcept
	{
		return ( string[ 0 ] == '\0' ) ? value : Hash::GetConst( &string[ 1 ], ( value ^ static_cast< std::uint32_t >( string[ 0 ] ) ) * internal::Prime );
	}

	static inline std::uint32_t Get( const std::string_view& string )
	{
		auto ret{ internal::Basis };

		for ( auto i{ 0u }; i < string.size( ); ++i )
		{
			ret ^= string[ i ];
			ret *= internal::Prime;
		}

		return ret;
	}
}
