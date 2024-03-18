#include "./types.h"

namespace klambda {

	GC::ptr<sexpr_t> evalArithmetic( GC::ptr<sexpr_t>, int tc );
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

	GC::ptr<sexpr_t> evalIf( GC::ptr<sexpr_t> args, int tc ) ; /*{
        auto nargs = NumberOfArgs( args );
        // if( nargs != 3 ) throw error_t{ ERROR_INCORRECT_NUMBER_OF_ARGUMENTS, nargs };

        auto cond = eval( car( args ) );
        auto if_true  = car( cdr( args ) );
        auto if_false = cdr( cdr( args ) );

        auto pv = std::get_if<bool>( &cond->node );

    } */

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






}