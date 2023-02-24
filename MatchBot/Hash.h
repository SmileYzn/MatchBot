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

	inline constexpr std::uint32_t GetConst( const std::string_view& stringText, const std::uint32_t value = internal::Basis ) noexcept
	{
		return ( stringText[ 0 ] == '\0' ) ? value : GetConst( &stringText[ 1 ], ( value ^ static_cast< std::uint32_t >( stringText[ 0 ] ) ) * internal::Prime );
	}

	inline std::uint32_t Get( const std::string_view& stringValue )
	{
		auto ret{ internal::Basis };

		for ( auto i{ 0u }; i < stringValue.size( ); ++i )
		{
			ret ^= stringValue[ i ];
			ret *= internal::Prime;
		}

		return ret;
	}
}
