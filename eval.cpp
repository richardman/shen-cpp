
#include "./eval.h"

namespace klambda {


    env_t envs;
    
    typedef GC::ptr<sexpr_t> (*native_function)( env_t &env, GC::ptr<sexpr_t>, int tc );

	int NumberOfArgs( GC::ptr<sexpr_t> args ) {
		auto pv = std::get_if<list_t>( &args->node );
		if( pv == nullptr ) return 1;

		int i = 0;
		while( pv != nullptr ) {
			++i;
			auto cdr = (*pv).second;
			pv = std::get_if<list_t>( &cdr->node );
		}
		return i;
	}

	GC::ptr<sexpr_t> evalPlus( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalMinus( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalMul( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalDiv( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalGt( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalLt( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalGe( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalLe( env_t &env, GC::ptr<sexpr_t>, int tc );

	GC::ptr<sexpr_t> evalAbsvector( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalAbsvectorP( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalAddressRead( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalAddressWrite( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalAnd( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalClose( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalCn( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalCond( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalConsP( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalCons( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalDefun( env_t &env, GC::ptr<sexpr_t>, int tc );      
	GC::ptr<sexpr_t> evalErrorToString( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalEqual( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalFreeze( env_t &env, GC::ptr<sexpr_t>, int tc );     
	GC::ptr<sexpr_t> evalGetTime( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalHd( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalIf( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalIntern( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalKLambda( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalLambda( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalLet( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalNToString( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalNumberP( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalOpen( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalOr( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalPos( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalReadByte( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalSet( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalSimpleError( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalStr( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalStringP( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalStringToN( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalTl( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalTlstr( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalTrapError( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalType( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalValue( env_t &env, GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalWriteByte( env_t &env, GC::ptr<sexpr_t>, int tc );

    std::unordered_map< std::string, std::pair< native_function, int > > primitives = {
        { "+",              { evalPlus,             2 } },
        { "-",              { evalMinus,			2 } },
        { "*",              { evalMul,			    2 } },
        { "/",              { evalDiv,			    2 } },
        { ">",              { evalGt,			    2 } },
        { "<",              { evalLt,			    2 } },
        { ">=",             { evalGe,			    2 } },
        { "<=",             { evalLe,			    2 } },
        { "absvector",      { evalAbsvector,		0 } },                
        { "absvector?",     { evalAbsvectorP,		1 } },
        { "<-address",      { evalAddressRead,	    2 } },
        { "address->",      { evalAddressWrite,	    3 } },
        { "and",            { evalAnd,			    2 } },
        { "close",          { evalClose,			2 } },
        { "cn",             { evalCn,			    2 } }, 
        { "cond",           { evalCond,			    -1 } },
        { "cons?",          { evalConsP,			1 } },         
        { "cons",           { evalCons,			    2 } },
        { "defun",          { evalDefun,			-1 } },
        { "error-to-string",{ evalErrorToString,	1 } },
        { "=",              { evalEqual,			2 } },
        { "eval-kl",        { evalKLambda,		    -1 } },
        { "freeze",         { evalFreeze,			2 } },        
        { "get-time",       { evalGetTime,		    1 } }, 
        { "hd",             { evalHd,			    1 } },
        { "if",             { evalIf,			    3 } },
        { "intern",         { evalIntern,			1 } },
        { "lambda",         { evalLambda,			-1 } },
        { "let",            { evalLet,			    2 } },        
        { "n->string",      { evalNToString,		1 } },
        { "number?",        { evalNumberP,		    1 } },
        { "open",           { evalOpen,			    2 } },
        { "or",             { evalOr,			    2 } },
        { "pos",            { evalPos,			    2 } },
        { "read-byte",      { evalReadByte,		    2 } },
        { "set",            { evalSet,			    2 } },
        { "simple-error",   { evalSimpleError,	    1 } },
        { "str",            { evalStr,			    1 } },
        { "string?",        { evalStringP,		    1 } },
        { "string->n",      { evalStringToN,		1 } }, 
        { "tl",             { evalTl,			    1 } },                                 
        { "tlstr",          { evalTlstr,			1 } },
        { "trap-error",     { evalTrapError,		0 } }, 
        { "type",           { evalType,			    1 } },
        { "value",          { evalValue,			1 } },
        { "write-byte",     { evalWriteByte,		1 } },               
    };

    GC::ptr<sexpr_t> apply( env_t &env, GC::ptr<sexpr_t> function, GC::ptr<sexpr_t>args, int tc );

    GC::ptr<sexpr_t> eval( env_t &env, GC::ptr<sexpr_t> sexpr, int tc ) {
        
        if( isAtom( sexpr ) ) return sexpr;

        if( std::holds_alternative<list_t>( sexpr->node ) ) {
            // Check if this is a primtive call
            if( std::holds_alternative<symbol_t>( std::get<list_t>( sexpr->node ).first->node ) ) {
                // function_symbol is the function name, e.g. (func a b c), function_symbol is "func"
                auto function_symbol = std::get<symbol_t>( std::get<list_t>( sexpr->node ).first->node );
                // is the function a primitve function
                auto primitive_iter = primitives.find( function_symbol.symbol );
                if( primitive_iter != primitives.end() ) {
                    // check the number of arguments 
                    auto native_function = primitive_iter->second.first;
                    auto nparams = primitive_iter->second.second;
                    if( nparams >= 0 ) {
                        auto nargs = NumberOfArgs( std::get<list_t>( sexpr->node ).second );
                        if( nparams != nargs ) throw error_t{ ERROR_INCORRECT_NUMBER_OF_ARGUMENTS, nargs };
                    }

                    return native_function( env, std::get<list_t>( sexpr->node ).second, tc );
                }
            }

            GC::ptr<sexpr_t> function = eval( env, std::get<list_t>( sexpr->node ).first, tc );

            apply( env, function, std::get<list_t>( sexpr->node ).second, tc );

        }

        return nullptr;
    }


    GC::ptr<sexpr_t> apply( env_t &env, GC::ptr<sexpr_t> function, GC::ptr<sexpr_t>args, int tc ) {


        return nullptr;
    }



}
