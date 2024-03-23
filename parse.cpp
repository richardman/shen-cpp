
#include <algorithm>
#include <assert.h>
#include <ctype.h>
#include <iostream>
#include <memory>
#include <queue>
#include <string.h>
#include <vector>

#include "./parse.h"
#include "./types.h"

namespace klambda {

    const std::string kl_alpha = { "=-*/+_?$!@~.><&%'#`" };
    
    static inline bool EndOfInput( const char c ) {
        return ( c == 0 || c == '\n' );
    } 

    // Token a string into multiple tokens
    tokens_t tokenize( std::string &inputs ) {
        tokens_t tokens;

        const char *next = inputs.c_str();
        
        while( !EndOfInput( *next ) ) {
            while( isspace( *next ) || !isprint( *next ) )
                next++;

            std::string str;
            // symbol
            if( isalpha( *next ) || kl_alpha.find_first_of( *next ) != std::string::npos ) {
                str += *next++;
                while( isalnum( *next ) || kl_alpha.find_first_of( *next ) != std::string::npos )
                    str += *next++;
            }             
            // integer
            else if( isdigit( *next ) || ( ( *next == '+' || *next == '-' ) && isdigit( next[1]) ) ) {
                if( *next == '+' ) 
                    next++;
                else if( *next == '-' )
                    str += *next++;

                while( isdigit( *next ) )
                    str += *next++;
                if( *next == '.' && isdigit( next[1] ) ) {
                    str += *next++;
                    while( isdigit( *next) )
                        str += *next++;
                }
            }
            // "quoted string"
            else if( *next == '"' ) {
                str += *next++;
                while( !EndOfInput( *next ) && *next != '"' ) {
                    str += *next;
                    // escape character
                    if( *next == '\'' && !EndOfInput( next[1] ) )
                        str += *++next;
                    next++;
                }

                if( *next != '"' ) {
                    std::cerr << "unclosed string" << str << ". Input ignored." << std::endl;
                    return {};
                }
                str += *next++;
            }
            else if( *next == '(' || *next == ')' ) {
                str += *next++;
            }

            if( str == "" ) {
                std::cerr << "unknown character '" << *next++ << "'. Input ignored." << std::endl;
                return {};
            }

            tokens.push( str );
        }

        return tokens;
    }


    GC::ptr<sexpr_t> makeSexpr( tokens_t &tokens ) {

        if( tokens.size() == 0 ) return nullptr;

        auto token = tokens.front();
        tokens.pop();

        //std::cout << "token: " << token << std::endl;        
        if( isdigit( token[0] ) ) {
            double ld = std::strtold( &token[0], nullptr );
            int64_t n = ld;
            if( ld == n ) 
                return GC::make<sexpr_t>( n );
            else
                return GC::make<sexpr_t>( ld );
        }

        if( token[0] == '"' ) {
            std::string non_quoted_string( &token[1] );
            non_quoted_string.resize( non_quoted_string.size() - 1 );
            return GC::make<sexpr_t>( non_quoted_string );
        }

        // a symbol
        if( token[0] != '(' ) {
            if( token == "true" )
                return GC::make<sexpr_t>( true );
            else if( token == "false" )
                return GC::make<sexpr_t>( false );

            return GC::make<sexpr_t>( (symbol_t){ token } );
        }

        if( tokens.front()[0] == ')' ) {
            tokens.pop();
            empty_list_t null_list;
            return GC::make<sexpr_t>( null_list );
        }

        auto list = GC::make<sexpr_t>( (list_t){ makeSexpr( tokens ), nullptr } );
        auto root_list = list;

        while( tokens.size() > 0 && tokens.front()[0] != ')' ) {        
            auto car = makeSexpr( tokens );
            std::get<list_t>( list->node ).second = GC::make<sexpr_t>( (list_t){ car, nullptr } );
            list = std::get<list_t>( list->node ).second;
        }

        if( tokens.size() == 0 ) {
            std::cerr << "Unclosed '('" << std::endl;
            return nullptr;
        }

        tokens.pop();       
        return root_list;
    }


    std::string SexprToString( GC::ptr<sexpr_t> sexpr, bool is_car ) {
        if( sexpr == nullptr ) return "";

        if( std::holds_alternative<symbol_t>( sexpr->node ) )       return std::get<symbol_t>( sexpr->node ).symbol;
        if( std::holds_alternative<std::string>( sexpr->node ) )    return "\"" + std::get<std::string>( sexpr->node ) + "\"";
        if( std::holds_alternative<int64_t>( sexpr->node ) )        return std::to_string( std::get<int64_t>( sexpr->node ) );
        if( std::holds_alternative<double>( sexpr->node ) )         return std::to_string( std::get<double>( sexpr->node ) );
        if( std::holds_alternative<bool>( sexpr->node ) )           return std::get<bool>( sexpr->node ) ? "true" : "false";

        std::string str;
        if( std::holds_alternative<list_t>( sexpr->node ) ) {
            auto list = std::get<list_t>( sexpr->node );
            
            if( is_car ) str += "( ";
            str += SexprToString( list.first, true );
            if( list.second != nullptr ) {
                if( std::holds_alternative<list_t>( list.second->node ) )
                    str += " ";
                else
                    str += " . ";
                str += SexprToString( list.second, false );
            }
            if( is_car ) str += " )";
            return str;
        }
        return "?#bad-sexpr";
    }


    bool isSymbol( const char *s, const char * &endp, char optional_delim ) {


        return false;
    }


}




