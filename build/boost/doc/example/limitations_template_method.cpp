#define BOOST_AUTO_TEST_MAIN
#include <boost/test/auto_unit_test.hpp>
#include <turtle/mock.hpp>

namespace limitations_template_method_problem
{
//[ limitations_template_method_problem
class concept
{
public:
    template< typename T >
    void method( T t )
    {}
};

template< typename T >
void function_under_test( T t ) // T is supposed to model the previous concept
{
    t.method( 42 );
    t.method( "string" );
}
//]
 
//[ limitations_template_method_solution
MOCK_CLASS( mock_concept )
{
    MOCK_METHOD( method, 1, void( int ), method_int )
    MOCK_METHOD( method, 1, void( const char* ), method_string )
};
//]
}

namespace limitations_template_method_problem_2
{
//[ limitations_template_method_problem_2
class concept
{
public:
    template< typename T >
    T create()
    {
        return T();
    }
};

template< typename T >
void function_under_test( T t ) // T is supposed to model the previous concept
{
    t.create< int >();
    t.create< std::string >();
}
//]
 
//[ limitations_template_method_solution_2
MOCK_CLASS( mock_concept )
{
    template< typename T >
    T create();

    template<>
    int create< int >()
    {
        return create_int();
    }
    template<>
    std::string create< std::string >()
    {
        return create_string();
    }

    MOCK_METHOD( create_int, 0, int(), create_int )
    MOCK_METHOD( create_string, 0, std::string(), create_string )
};
//]
}