#pragma once

#include "./types.h"

namespace klambda {

    typedef std::queue<std::string> tokens_t;

    tokens_t tokenize( std::string &inputs );
    GC::ptr<sexpr_t> makeSexpr( tokens_t &tokens );
    std::string SexprToString( GC::ptr<sexpr_t> sexpr, bool is_car );


}