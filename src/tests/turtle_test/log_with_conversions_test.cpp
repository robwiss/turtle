//
//  Copyright Mathieu Champlon 2008
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#define MOCK_LOGGING_USE_CONVERSIONS
#include <turtle/log.hpp>
#include <boost/assign.hpp>
#include <vector>
#include <deque>
#include <list>
#include <map>
#include <set>

#include <boost/test/auto_unit_test.hpp>
#define BOOST_LIB_NAME boost_unit_test_framework
#include <boost/config/auto_link.hpp>

namespace
{
    template< typename T >
    std::string to_string( T t )
    {
        std::stringstream s;
        s << mock::format( t );
        return s.str();
    }
}

BOOST_AUTO_TEST_CASE( base_type_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "42", to_string( 42 ) );
}

BOOST_AUTO_TEST_CASE( booleans_are_serialized_as_bool_alpha_with_conversions )
{
    BOOST_CHECK_EQUAL( "true", to_string( true ) );
    BOOST_CHECK_EQUAL( "false", to_string( false ) );
}

BOOST_AUTO_TEST_CASE( strings_are_serialized_with_double_quotes_with_conversions )
{
    BOOST_CHECK_EQUAL( "\"string\"", to_string( "string" ) );
    BOOST_CHECK_EQUAL( "\"string\"", to_string( std::string( "string" ) ) );
}

namespace
{
    struct non_serializable
    {};
}

BOOST_AUTO_TEST_CASE( non_serializable_type_yields_an_interrogation_mark_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "?", to_string( non_serializable() ) );
}

namespace
{
    struct serializable
    {};
    std::ostream& operator<<( std::ostream& s, const serializable& )
    {
        return s << "serializable";
    }
}

BOOST_AUTO_TEST_CASE( serializable_type_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "serializable", to_string( serializable() ) );
}

namespace
{
    struct streamable
    {};
    std::ostream& operator<<( std::ostream& s, const streamable& )
    {
        BOOST_FAIL( "should not have been called" );
        return s;
    }
    mock::stream& operator<<( mock::stream& s, const streamable& )
    {
        return s << "streamable";
    }
}

BOOST_AUTO_TEST_CASE( streamable_type_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "streamable", to_string( streamable() ) );
}

namespace
{
    struct mock_streamable
    {};
    std::ostream& operator<<( std::ostream& s, const mock_streamable& )
    {
        BOOST_FAIL( "should not have been called" );
        return s;
    }
}
namespace mock
{
    stream& operator<<( stream& s, const mock_streamable& )
    {
        return s << "mock_streamable";
    }
}

BOOST_AUTO_TEST_CASE( mock_streamable_type_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "mock_streamable", to_string( mock_streamable() ) );
}

namespace
{
    struct derived_from_serializable : serializable
    {};
}

BOOST_AUTO_TEST_CASE( type_derived_from_serializable_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "serializable", to_string( derived_from_serializable() ) );
}

namespace
{
    struct derived_from_streamable : streamable
    {};
}

BOOST_AUTO_TEST_CASE( type_derived_from_streamable_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "streamable", to_string( derived_from_streamable() ) );
}

//namespace
//{
//    struct convertible_to_base
//    {
//        operator int() const
//        {
//            return 3;
//        }
//    };
//}
//
//BOOST_AUTO_TEST_CASE( type_convertible_to_base_yields_its_value_when_serialized_with_conversions )
//{
//    BOOST_CHECK_EQUAL( "3", to_string( convertible_to_base() ) );
//}

//namespace
//{
//    struct convertible_to_serializable
//    {
//        operator serializable() const;
//    };
//}
//
//BOOST_AUTO_TEST_CASE( type_convertible_to_serializable_yields_its_value_when_serialized_with_conversions )
//{
//    BOOST_CHECK_EQUAL( "serializable", to_string( convertible_to_serializable() ) );
//}

//namespace
//{
//    struct convertible_to_streamable
//    {
//        operator streamable() const;
//    };
//}
//
//BOOST_AUTO_TEST_CASE( type_convertible_to_streamable_yields_its_value_when_serialized_with_conversions )
//{
//    BOOST_CHECK_EQUAL( "streamable", to_string( convertible_to_streamable() ) );
//}

//namespace
//{
//    struct ambiguous_convertible
//    {
//        operator float() const;
//        operator int() const;
//        operator serializable() const;
//        operator streamable() const;
//        template< typename T > operator T() const;
//    };
//}
//
//BOOST_AUTO_TEST_CASE( type_ambiguous_convertible_yields_an_interrogation_mark_when_serialized_with_conversions )
//{
//    BOOST_CHECK_EQUAL( "?", to_string( ambiguous_convertible() ) );
//}

//namespace
//{
//    struct ambiguous_convertible_serializable
//    {
//        operator float() const;
//        operator int() const;
//        operator serializable() const;
//        operator streamable() const;
//        template< typename T > operator T() const;
//    };
//    std::ostream& operator<<( std::ostream& s, const ambiguous_convertible_serializable& )
//    {
//        return s << "ambiguous_convertible_serializable";
//    }
//}
//
//BOOST_AUTO_TEST_CASE( type_ambiguous_convertible_serializable_yields_its_value_when_serialized_with_conversions )
//{
//    BOOST_CHECK_EQUAL( "ambiguous_convertible_serializable", to_string( ambiguous_convertible_serializable() ) );
//}

namespace
{
    struct ambiguous_convertible_streamable
    {
        operator float() const;
        operator int() const;
        operator serializable() const;
        operator streamable() const;
        template< typename T > operator T() const;
    };
    std::ostream& operator<<( std::ostream& s, const ambiguous_convertible_streamable& )
    {
        BOOST_FAIL( "should not have been called" );
        return s;
    }
    mock::stream& operator<<( mock::stream& s, const ambiguous_convertible_streamable& )
    {
        return s << "ambiguous_convertible_streamable";
    }
}

BOOST_AUTO_TEST_CASE( type_ambiguous_convertible_streamable_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "ambiguous_convertible_streamable", to_string( ambiguous_convertible_streamable() ) );
}

namespace
{
    struct ambiguous_convertible_mock_streamable
    {
        operator float() const;
        operator int() const;
        operator serializable() const;
        operator streamable() const;
        template< typename T > operator T() const;
    };
    std::ostream& operator<<( std::ostream& s, const ambiguous_convertible_mock_streamable& )
    {
        BOOST_FAIL( "should not have been called" );
        return s;
    }
}
namespace mock
{
    stream& operator<<( stream& s, const ambiguous_convertible_mock_streamable& )
    {
        return s << "ambiguous_convertible_mock_streamable";
    }
}

BOOST_AUTO_TEST_CASE( type_ambiguous_convertible_mock_streamable_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "ambiguous_convertible_mock_streamable", to_string( ambiguous_convertible_mock_streamable() ) );
}

namespace
{
    template< typename T >
    struct template_serializable
    {};
    template< typename T >
    std::ostream& operator<<( std::ostream& s, const template_serializable< T >& )
    {
        return s << "template_serializable";
    }
}

BOOST_AUTO_TEST_CASE( template_type_serializable_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "template_serializable", to_string( template_serializable< int >() ) );
}

namespace
{
    template< typename T >
    struct template_streamable
    {};
    template< typename T >
    std::ostream& operator<<( std::ostream& s, const template_streamable< T >& )
    {
        BOOST_FAIL( "should not have been called" );
        return s;
    }
    template< typename T >
    mock::stream& operator<<( mock::stream& s, const template_streamable< T >& )
    {
        return s << "template_streamable";
    }
}

BOOST_AUTO_TEST_CASE( template_streamable_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "template_streamable", to_string( template_streamable< int >() ) );
}

namespace
{
    template< typename T >
    struct template_mock_streamable
    {};
    template< typename T >
    std::ostream& operator<<( std::ostream& s, const template_mock_streamable< T >& )
    {
        BOOST_FAIL( "should not have been called" );
        return s;
    }
}
namespace mock
{
    template< typename T >
    stream& operator<<( stream& s, const template_mock_streamable< T >& )
    {
        return s << "template_mock_streamable";
    }
}

BOOST_AUTO_TEST_CASE( template_mock_streamable_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "template_mock_streamable", to_string( template_mock_streamable< int >() ) );
}

BOOST_AUTO_TEST_CASE( std_pairs_are_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "(3,42)", to_string( std::make_pair( 3, 42.f ) ) );
}

BOOST_AUTO_TEST_CASE( std_deques_are_serialized_with_conversions )
{
    std::deque< int > d;
    d.push_back( 12 );
    d.push_back( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", to_string( d ) );
}

BOOST_AUTO_TEST_CASE( std_lists_are_serialized_with_conversions )
{
    std::list< int > l;
    l.push_back( 12 );
    l.push_back( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", to_string( l ) );
}

BOOST_AUTO_TEST_CASE( std_vectors_are_serialized_with_conversions )
{
    std::vector< int > v;
    v.push_back( 12 );
    v.push_back( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", to_string( v ) );
}

BOOST_AUTO_TEST_CASE( std_maps_are_serialized_with_conversions )
{
    std::map< int, std::string > m;
    m[ 12 ] = "12";
    m[ 42 ] = "42";
    BOOST_CHECK_EQUAL( "((12,\"12\"),(42,\"42\"))", to_string( m ) );
}

BOOST_AUTO_TEST_CASE( std_multimaps_are_serialized_with_conversions )
{
    std::multimap< int, std::string > m;
    m.insert( std::make_pair( 12, "12" ));
    m.insert( std::make_pair( 42, "42" ));
    BOOST_CHECK_EQUAL( "((12,\"12\"),(42,\"42\"))", to_string( m ) );
}

BOOST_AUTO_TEST_CASE( std_sets_are_serialized_with_conversions )
{
    std::set< int > s;
    s.insert( 12 );
    s.insert( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", to_string( s ) );
}

BOOST_AUTO_TEST_CASE( std_multisets_are_serialized_with_conversions )
{
    std::multiset< int > s;
    s.insert( 12 );
    s.insert( 42 );
    BOOST_CHECK_EQUAL( "(12,42)", to_string( s ) );
}

BOOST_AUTO_TEST_CASE( boost_assign_list_of_are_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "(12,42)", to_string( boost::assign::list_of( 12 )( 42 ) ) );
}

BOOST_AUTO_TEST_CASE( boost_assign_map_list_of_are_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "((12,\"12\"),(42,\"42\"))", to_string( boost::assign::map_list_of( 12, "12" )( 42, "42" ) ) );
}

namespace
{
    void callable_builtin()
    {}
}

BOOST_AUTO_TEST_CASE( callable_builtin_yields_an_interrogation_mark_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "?", to_string( callable_builtin ) );
    BOOST_CHECK_EQUAL( "?", to_string( &callable_builtin ) );
}

namespace
{
    struct serialized_using_format
    {};
    std::ostream& operator<<( std::ostream& s, const serialized_using_format& )
    {
        return s << mock::format( "string" );
    }
}

BOOST_AUTO_TEST_CASE( type_can_use_format_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "\"string\"", to_string( serialized_using_format() ) );
}

namespace
{
    struct streamed_using_format
    {};
    mock::stream& operator<<( mock::stream& s, const streamed_using_format& )
    {
        return s << mock::format( "string" );
    }
}

BOOST_AUTO_TEST_CASE( type_can_use_format_when_streamed_with_conversions )
{
    BOOST_CHECK_EQUAL( "\"string\"", to_string( streamed_using_format() ) );
}

namespace
{
    struct std_string_streamed
    {};
    mock::stream& operator<<( mock::stream& s, const std_string_streamed& )
    {
        return s << std::string( "string" );
    }
}

BOOST_AUTO_TEST_CASE( std_string_streamed_is_not_a_container_with_conversions )
{
    BOOST_CHECK_EQUAL( "string", to_string( std_string_streamed() ) );
}

namespace mock
{
namespace detail
{
    template< typename T >
    struct template_serializable
    {};
    template< typename T >
    std::ostream& operator<<( std::ostream& s, const template_serializable< T >& )
    {
        return s << "mock::detail::template_serializable";
    }
}
}

BOOST_AUTO_TEST_CASE( mock_detail_template_type_serializable_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "mock::detail::template_serializable", to_string( mock::detail::template_serializable< int >() ) );
}

namespace mock
{
namespace detail
{
    template< typename T >
    struct template_streamable
    {};
    template< typename T >
    std::ostream& operator<<( std::ostream& s, const template_streamable< T >& )
    {
        BOOST_FAIL( "should not have been called" );
        return s;
    }
    template< typename T >
    mock::stream& operator<<( mock::stream& s, const template_streamable< T >& )
    {
        return s << "mock::detail::template_streamable";
    }
}
}

BOOST_AUTO_TEST_CASE( mock_detail_template_template_streamable_yields_its_value_when_serialized_with_conversions )
{
    BOOST_CHECK_EQUAL( "mock::detail::template_streamable", to_string( mock::detail::template_streamable< int >() ) );
}
