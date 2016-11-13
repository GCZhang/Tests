/*    
  convertions between string and numbers.
*/

#include <string>
#include <iostream>
#include <sstream>
#include <boost/lexical_cast.hpp>

/****************************************************
    Macro based on ostringstream.
Explanation:

The std::dec is a side-effect-free way to make the anonymous 
ostringstream into a generic ostream so operator<<() function 
lookup works correctly for all types. (You get into trouble 
otherwise if the first argument is a pointer type.)

The dynamic_cast returns the type back to ostringstream so you can call str() on it.

Use:

#include <string>

int main()
{
    int i = 42;
    std::string s1 = SSTR( i );

    int x = 23;
    std::string s2 = SSTR( "i: " << i << ", x: " << x );
    return 0;
}

*****************************************************/
#define SSTR( x ) static_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()
/****************************************************/

/****************************************************
    template function based on ostringstream.
*****************************************************/
template<typename T>
std::string to_string(const T& value)
{
    std::ostringstream oss;
    oss << value;
    return oss.str();
}
/****************************************************/

/****************************************************
template function String to number based on stringstream.
*****************************************************/
template<typename T>
T string_to_num(const std::string &text, T defValue=T())
{
    std::stringstream ss(text);
    T result;

    for(std::string::const_iterator i = text.begin(); i != text.end(); ++i){
        if(isdigit(*i) || *i=='e' || *i=='+' || *i=='-' || *i=='.'){
            ss << *i;
        }
    }

    return ss >> result ? result: defValue;
}
/****************************************************/

int main(int argc, char const *argv[])
{
    
    int number = 1986;
    float real = 4.2;
    double exp = 1.3e6;
    std::string pre = "fort.";
// +----------------------------------------------------------------+
// |                   NUMBER TO STRING                             |
// +----------------------------------------------------------------+

    /****************************************************
        C++ 11 features: to_string().
        should include <string>
        only works with -std=c++11
    *****************************************************/
    std::cout << std::endl;
    std::cout << "using std::to_string(): ";
    std::string name = pre + std::to_string(number);
    std::cout << name << std::endl;
    /****************************************************/

    /****************************************************
        boost::lexical_cast
        need boost library
    *****************************************************/  
    name = pre + boost::lexical_cast<std::string>(number);
    std::cout << std::endl;
    std::cout << "using boos::lexical_cast: ";
    std::cout << name << std::endl;
    /****************************************************/

    /****************************************************
        using ostringstream.
        only ostringstream header is needed.
        C++98 compatible.
    *****************************************************/
    std::ostringstream oss;
    oss << number;
    std::string number_s = oss.str();
    name = pre + number_s;
    std::cout << std::endl;
    std::cout << "using ostringstream: ";
    std::cout << name << std::endl;
    /****************************************************/

    /****************************************************
        Macro based on ostringstream.
    *****************************************************/
    name = pre + SSTR(number);
    std::cout << std::endl;
    std::cout << "using macro based on ostringstream: ";
    std::cout << name << std::endl;

    std::string s2 = SSTR( "i: " << 42 << ", x: " << 24 );
    std::cout << "using macro based on ostringstream (daisy-chain outputs): ";
    std::cout << s2 << std::endl;

    /****************************************************/

    /****************************************************
    template function to_string() based on ostringstream.
    *****************************************************/    
    name = pre + to_string(number);
    std::cout << std::endl;
    std::cout << "using template function based on ostringstream: ";
    std::cout << name << std::endl;

    name = pre + to_string(real);
    std::cout << "using template function based on ostringstream (float number): ";
    std::cout << name << std::endl;

    name = pre + to_string(exp);
    std::cout << "using template function based on ostringstream (exp number): ";
    std::cout << name << std::endl;
    /****************************************************/

// ==================================================================    

// +----------------------------------------------------------------+
// |                   STRING TO NUMBER                             |
// +----------------------------------------------------------------+

    std::cout << std::endl;
    std::cout << "-------------------------" << std::endl;
    std::cout << "Convert String To Number." << std::endl;
    std::cout << "-------------------------" << std::endl;
    using std::string;

    /****************************************************
    template function string_to_num() based on stringstream.
    *****************************************************/    
    string num_s = "12456";
    string real_s = "1.0e-3";
    string sss = "3 ran 8 dom 9 chars 0.45e+3";
    std::cout << "using template function based on stringstream(int, 12456): ";
    std::cout << string_to_num<int>(num_s) << std::endl;
    std::cout << "using template function based on stringstream(float, 1.0e-3): ";
    std::cout << string_to_num<float>(real_s) << std::endl;
    std::cout << "using template function based on stringstream(string," << sss << " ): ";
    std::cout << string_to_num<float>(sss) << std::endl;


    return 0;
}
