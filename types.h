#pragma once

#include <iostream>
#include <map>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <variant>
#include <vector>

#include "../../../cpp-gc/GarbageCollection.h"

#include "./status_codes.h"

namespace klambda {

    class sexpr_t;

    typedef std::pair<GC::ptr<sexpr_t>, GC::ptr<sexpr_t>> list_t;
    typedef std::map<std::string, GC::ptr<sexpr_t>> dictionary_t;     
    typedef std::vector<GC::ptr<sexpr_t>> vector_t;

    typedef std::unordered_map<std::string, GC::ptr<sexpr_t>> symbol_table_t;
    typedef std::vector<symbol_table_t> env_t;  

    extern env_t envs;
    
    class empty_list_t {
      public:
        bool empty = true;
    };

    class closure_t {
      public:
        void *context;
    };

    class continuation_t {
      public:
        void *context;        
    };

    class exception_t {
        void *stack_frame;
    };
    
    class function_t {
        std::string name;
        sexpr_t *parameters;
        sexpr_t *body;
        symbol_table_t *local_vars;
    };

    class primitive_t {
        std::string name;
        sexpr_t (*native_function)( sexpr_t * params );
    };

    class stream_t {
      public:
        std::iostream ios;
    };

    class symbol_t {
      public:
        std::string symbol;

        symbol_t( std::string a_symbol ) : symbol( a_symbol ) {}
    };



    typedef std::variant<
        std::string,
        symbol_t,        
        int64_t,
        double,
        bool,
        stream_t,
        exception_t,
        vector_t, 
        function_t,
        primitive_t,
        list_t, empty_list_t,
        dictionary_t,
        closure_t,
        continuation_t
    > node_t;

    class sexpr_t {
      public:

        node_t node;

        sexpr_t( const symbol_t &a_node )       : node( a_node ) { }
        sexpr_t( std::string &a_node )          : node( a_node ) { }
        sexpr_t( int64_t a_node)                : node( a_node ) { }
        sexpr_t( double a_node )                : node( a_node ) { }
        sexpr_t( bool a_node )                  : node( a_node ) { }
        //sexpr_t( const stream_t &a_node )       { node = a_node; }
        sexpr_t( const exception_t &a_node )    : node( a_node ) { }
        sexpr_t( const vector_t &a_node )       : node( a_node ) { }
        sexpr_t( const function_t &a_node)      : node( a_node ) { }
        sexpr_t( const list_t &a_node )         : node( a_node ) { }
        sexpr_t( const empty_list_t &a_node )   : node( a_node ) { }        
        sexpr_t( const dictionary_t &a_node )   : node( a_node ) { }
        sexpr_t( const closure_t &a_node )      : node( a_node ) { }
        sexpr_t( const continuation_t &a_node ) : node( a_node ) { }                        
    };


    static inline bool isAtom( GC::ptr<sexpr_t> sexpr ) {
        if( std::holds_alternative<symbol_t>( sexpr->node ) ||
            std::holds_alternative<std::string>( sexpr->node ) ||
            std::holds_alternative<int64_t>( sexpr->node ) ||
            std::holds_alternative<double>( sexpr->node ) ||
            std::holds_alternative<bool>( sexpr->node ) ) {

            return true;
        }
        return false;
    }    

    static inline GC::ptr<sexpr_t> car( GC::ptr<sexpr_t> sexpr ) {
      auto pv = std::get_if<list_t>( &sexpr->node );
      if( pv == nullptr ) throw error_t{ ERROR_SEXPR_NOT_A_LIST, 0 };

      return (*pv).first;
    }

    static inline GC::ptr<sexpr_t> cdr( GC::ptr<sexpr_t> sexpr ) {
      auto pv = std::get_if<list_t>( &sexpr->node );
      if( pv == nullptr ) throw error_t{ ERROR_SEXPR_NOT_A_LIST, 0 };

      return (*pv).second;
    }


}