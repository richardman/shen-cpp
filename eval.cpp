
#include "./eval.h"

namespace klambda {

    typedef GC::ptr<sexpr_t> (*native_function)( GC::ptr<sexpr_t>, int tc );

	GC::ptr<sexpr_t> evalArithmetic( GC::ptr<sexpr_t>, int tc ) {
        return nullptr;
    }
    
	GC::ptr<sexpr_t> evalAbsvector( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalAbsvectorP( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalAddressRead( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalAddressWrite( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalAnd( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalClose( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalCn( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalCond( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalConsP( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalCons( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalDefun( GC::ptr<sexpr_t>, int tc );      
	GC::ptr<sexpr_t> evalErrorToString( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalEqual( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalFreeze( GC::ptr<sexpr_t>, int tc );     
	GC::ptr<sexpr_t> evalGetTime( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalHd( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalIf( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalIntern( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalKLambda( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalLambda( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalLet( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalNToString( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalNumberP( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalOpen( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalOr( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalPos( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalReadByte( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalSet( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalSimpleError( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalStr( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalStringP( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalStringToN( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalTl( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalTlstr( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalTrapError( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalType( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalValue( GC::ptr<sexpr_t>, int tc );
	GC::ptr<sexpr_t> evalWriteByte( GC::ptr<sexpr_t>, int tc );

    std::unordered_map< std::string, native_function > primitives = {
/*
        { "+",              evalArithmetic },
        { "-",              evalArithmetic },
        { "*",              evalArithmetic },
        { "/",              evalArithmetic },
        { ">",              evalArithmetic },
        { "<",              evalArithmetic },
        { ">=",             evalArithmetic },
        { "<=",             evalArithmetic },
        { "absvector",      evalAbsvector },                
        { "absvector?",     evalAbsvectorP },
        { "<-address",      evalAddressRead },
        { "address->",      evalAddressWrite },
        { "and",            evalAnd },
        { "close",          evalClose },
        { "cn",             evalCn }, 
        { "cond",           evalCond },
        { "cons?",          evalConsP },         
        { "cons",           evalCons },
        { "defun",          evalDefun },
        { "error-to-string",evalErrorToString },
        { "=",              evalEqual },
        { "eval-kl",        evalKLambda },
        { "freeze",         evalFreeze },        
        { "get-time",       evalGetTime }, 
        { "hd",             evalHd },
        { "if",             evalIf },
        { "intern",         evalIntern },
        { "lambda",         evalLambda },
        { "let",            evalLet },        
        { "n->string",      evalNToString },
        { "number?",        evalNumberP },
        { "open",           evalOpen },
        { "or",             evalOr },
        { "pos",            evalPos },
        { "read-byte",      evalReadByte },
        { "set",            evalSet },
        { "simple-error",   evalSimpleError },
        { "str",            evalStr },
        { "string?",        evalStringP },
        { "string->n",      evalStringToN }, 
        { "tl",             evalTl },                                 
        { "tlstr",          evalTlstr },
        { "trap-error",     evalTrapError }, 
        { "type",           evalType },
        { "value",          evalValue },
        { "write-byte",     evalWriteByte },        
*/        
    };

    GC::ptr<sexpr_t> apply( GC::ptr<sexpr_t> function, GC::ptr<sexpr_t>args, int tc );

    GC::ptr<sexpr_t> eval( GC::ptr<sexpr_t> sexpr, int tc ) {
        
        if( isAtom( sexpr ) ) return sexpr;

        if( std::holds_alternative<list_t>( sexpr->node ) ) {
            // Check if this is a primtive call
            if( std::holds_alternative<symbol_t>( std::get<list_t>( sexpr->node ).first->node ) ) {
                auto function_symbol = std::get<symbol_t>( std::get<list_t>( sexpr->node ).first->node );
                auto primitive_iter = primitives.find( function_symbol.symbol );
                if( primitive_iter != primitives.end() ) {
                    return primitive_iter->second( std::get<list_t>( sexpr->node ).second, tc );
                }
            }

            GC::ptr<sexpr_t> function = eval( std::get<list_t>( sexpr->node ).first, tc );

            apply( function, std::get<list_t>( sexpr->node ).second, tc );

        }

        return nullptr;
    }


    GC::ptr<sexpr_t> apply( GC::ptr<sexpr_t> function, GC::ptr<sexpr_t>args, int tc ) {


        return nullptr;
    }



}