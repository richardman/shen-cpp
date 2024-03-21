#include <iostream>
#include <time.h>

#include "./eval.h"
#include "./parse.h"
#include "./status_codes.h"
#include "./types.h"

namespace klambda {

    template <typename T>
    T *mustBeOfType( GC::ptr<sexpr_t> arg ) {
        auto pv = std::get_if<T>( &arg->node );
        if( pv == nullptr ) throw error_t{ ERROR_SEXPR_INCORRECT_TYPE, 0 };

        return pv;
    }

	GC::ptr<sexpr_t> evalAbsvector( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalAbsvectorP( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalAddressRead( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalAddressWrite( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }

	GC::ptr<sexpr_t> evalAnd( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto operand = eval( env, car( args ), 0 );
        auto pv = std::get_if<bool>( &operand->node );
        if( ( pv == nullptr ) || (*pv) == false ) return GC::make<sexpr_t>( false );

        operand = eval( env, car( cdr( args ) ), 0 );
        pv = std::get_if<bool>( &operand->node );
        return GC::make<sexpr_t>( ( pv != nullptr ) && (*pv) == true );        
    }

	GC::ptr<sexpr_t> evalClose( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }

	GC::ptr<sexpr_t> evalCn( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto pv_first = mustBeOfType<std::string>( eval( env, car( args ), 0 ) );
        auto pv_second = mustBeOfType<std::string>( eval( env, car( cdr( args ) ), 0 ) );

        std::string res = *pv_first + *pv_second;
        return GC::make<sexpr_t>( res ); 
    }

	GC::ptr<sexpr_t> evalCond( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }

	GC::ptr<sexpr_t> evalConsP( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto first = eval( env, car( args ), 0 );
        return GC::make<sexpr_t>( first != nullptr && std::holds_alternative<list_t>( first->node ) );
    }

	GC::ptr<sexpr_t> evalCons( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto first = eval( env, car( args ), 0 );
        auto second = eval( env, car( cdr( args ) ), 0 );
        auto pv_second = std::get_if<list_t>( &second->node );

        // make a dotted pair
        if( pv_second == nullptr ) {
            return GC::make<sexpr_t>( (list_t){ first, second } );
        }

        return GC::make<sexpr_t>( (list_t){ first, GC::make<sexpr_t>( (list_t){ second, nullptr } ) } );
    }


	GC::ptr<sexpr_t> evalDefun( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }      
	GC::ptr<sexpr_t> evalErrorToString( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }

	GC::ptr<sexpr_t> evalEqual( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalFreeze( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }     

	GC::ptr<sexpr_t> evalGetTime( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto first = eval( env, car( args ), 0 );
        auto pv = mustBeOfType<symbol_t>( first );

        static time_t last_called = 0;
        time_t t = time( NULL );
        
        int64_t res = t;
        if( pv->symbol == "run" || pv->symbol == "real" ) {
            res -= last_called;
        } else if( pv->symbol != "unix" ) {
            throw error_t{ ERROR_INCORRECT_ARGUMENT, 0};
        }

        last_called = t;        
        return GC::make<sexpr_t>( res ); 
    }

	GC::ptr<sexpr_t> evalHd( env_t &env, GC::ptr<sexpr_t>args, int tc ) { 
        auto first = eval( env, car( args ), 0 );
        auto pv = mustBeOfType<list_t>( first );

        return pv->first; 
    }

	GC::ptr<sexpr_t> evalIf( env_t &env, GC::ptr<sexpr_t> args, int tc ) {
        auto cond = eval( env, car( args ), 0 );
        auto if_true  = car( cdr( args ) );
        auto if_false = car( cdr( cdr( args ) ) );

        std::cout << "cond: " << SexprToString( cond, true ) << std::endl;
        auto pv = std::get_if<bool>( &cond->node );
        if( pv == nullptr ) throw error_t{ ERROR_SEXPR_NOT_A_BOOLEAN, 0 };

		return ( (*pv) == true ) ? eval( env, if_true, tc ) : eval( env, if_false, tc );
    } 

	GC::ptr<sexpr_t> evalIntern( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalKLambda( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalLambda( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalLet( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalNToString( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }

	GC::ptr<sexpr_t> evalNumberP( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto operand = eval( env, car( args ), 0 );
        return GC::make<sexpr_t>( std::holds_alternative<int64_t>( operand->node ) || std::holds_alternative<double>( operand->node ) );
    }

	GC::ptr<sexpr_t> evalOpen( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }

	GC::ptr<sexpr_t> evalOr( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto operand = eval( env, car( args ), 0 );
        auto pv = std::get_if<bool>( &operand->node );
        if( ( pv != nullptr ) && (*pv) == true ) return GC::make<sexpr_t>( true );

        operand = eval( env, car( cdr( args ) ), 0 );
        pv = std::get_if<bool>( &operand->node );
        return GC::make<sexpr_t>( ( pv != nullptr ) && (*pv) == true );         
    }
    
	GC::ptr<sexpr_t> evalPos( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto first = eval( env, car( args ), 0 );
        auto pv_first = mustBeOfType<std::string>( first );

        auto second = eval( env, car( cdr( args ) ), 0);
        auto pv_second = mustBeOfType<int64_t>( second );

        std::string str = *pv_first;
        int pos = *pv_second;

        if( pos < 0 || pos >= str.size() ) throw error_t{ ERROR_POS_OUT_OF_RANGE, pos };

        std::string res;
        res.push_back( str[pos] );
        return  GC::make<sexpr_t>( res ); 
    }

	GC::ptr<sexpr_t> evalReadByte( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	
    GC::ptr<sexpr_t> evalSet( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        return nullptr; 
    }

	GC::ptr<sexpr_t> evalSimpleError( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }

	GC::ptr<sexpr_t> evalStr( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto operand = eval( env, car( args ), 0 );

        // Cannot call gc::make<>() inline because it will call the symbol_t constructor even if the
        // argument is a string
        std::string str;
        if( std::holds_alternative<symbol_t>( operand->node ) ) 
            str = std::get<symbol_t>(operand->node).symbol;
        else if( std::holds_alternative<int64_t>( operand->node ) ) 
            str = std::to_string( std::get<int64_t>(operand->node) );
        else if( std::holds_alternative<double>( operand->node ) ) 
            str = std::to_string( std::get<double>(operand->node) );
        else if( std::holds_alternative<bool>( operand->node ) ) 
            str = ( std::get<bool>(operand->node) == true ? "true" : "false" );            
        else if( std::holds_alternative<std::string>( operand->node ) ) 
            str = std::get<std::string>( operand->node );  
        else
            throw error_t{ ERROR_ARGUMENT_IS_NOT_AN_ATOM, 0 };
        return GC::make<sexpr_t>( str ); 
        }


	GC::ptr<sexpr_t> evalStringP( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalStringToN( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }

	GC::ptr<sexpr_t> evalTl( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto operand = eval( env, car( args ), 0 );
        auto pv = mustBeOfType<list_t>( operand );

        return pv->second; 
    }        

	GC::ptr<sexpr_t> evalTlstr( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto operand = eval( env, car( args ), 0 );
        auto pv = std::get_if<std::string>( &operand->node );        

        std::string tlstr( &(*pv)[1] );
        return GC::make<sexpr_t>( tlstr ); 
    }

	GC::ptr<sexpr_t> evalTrapError( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalType( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalValue( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }
	GC::ptr<sexpr_t> evalWriteByte( env_t &env, GC::ptr<sexpr_t>, int tc ) { return nullptr; }



	GC::ptr<sexpr_t> evalPlus( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto left = eval( env, car( args ), 0 );
        auto right = eval( env, car( cdr( args ) ), 0 );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) + std::get<int64_t>( right->node ) );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) + std::get<double>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   + std::get<int64_t>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   + std::get<double>( right->node ) );    
        throw error_t{ ERROR_SEXPR_NOT_A_NUMBER, 0 };
        return nullptr;
    }

	GC::ptr<sexpr_t> evalMinus( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto left = eval( env, car( args ), 0 );
        auto right = eval( env, car( cdr( args ) ), 0 );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) - std::get<int64_t>( right->node ) );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) - std::get<double>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   - std::get<int64_t>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   - std::get<double>( right->node ) );    
        throw error_t{ ERROR_SEXPR_NOT_A_NUMBER, 0 };
        return nullptr;
    }

	GC::ptr<sexpr_t> evalMul( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto left = eval( env, car( args ), 0 );
        auto right = eval( env, car( cdr( args ) ), 0 );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) * std::get<int64_t>( right->node ) );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) * std::get<double>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )  * std::get<int64_t>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )  * std::get<double>( right->node ) );    
        throw error_t{ ERROR_SEXPR_NOT_A_NUMBER, 0 };
        return nullptr;
    }

	GC::ptr<sexpr_t> evalDiv( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto left = eval( env, car( args ), 0 );
        auto right = eval( env, car( cdr( args ) ), 0 );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( (double)( std::get<int64_t>( left->node ) ) / std::get<int64_t>( right->node ) );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node )  / std::get<double>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   / std::get<int64_t>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   / std::get<double>( right->node ) );    
        throw error_t{ ERROR_SEXPR_NOT_A_NUMBER, 0 };
        return nullptr;
    }

	GC::ptr<sexpr_t> evalGt( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto left = eval( env, car( args ), 0 );
        auto right = eval( env, car( cdr( args ) ), 0 );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) > std::get<int64_t>( right->node ) );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) > std::get<double>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   > std::get<int64_t>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   > std::get<double>( right->node ) );    
        throw error_t{ ERROR_SEXPR_NOT_A_NUMBER, 0 };
        return nullptr;
    }

	GC::ptr<sexpr_t> evalLt( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto left = eval( env, car( args ), 0 );
        auto right = eval( env, car( cdr( args ) ), 0 );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) < std::get<int64_t>( right->node ) );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) < std::get<double>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   < std::get<int64_t>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   < std::get<double>( right->node ) );    
        throw error_t{ ERROR_SEXPR_NOT_A_NUMBER, 0 };
        return nullptr;
    }

	GC::ptr<sexpr_t> evalGe( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto left = eval( env, car( args ), 0 );
        auto right = eval( env, car( cdr( args ) ), 0 );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) >= std::get<int64_t>( right->node ) );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) >= std::get<double>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   >= std::get<int64_t>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   >= std::get<double>( right->node ) );    
        throw error_t{ ERROR_SEXPR_NOT_A_NUMBER, 0 };
        return nullptr;
    }

	GC::ptr<sexpr_t> evalLe( env_t &env, GC::ptr<sexpr_t> args, int tc ) { 
        auto left = eval( env, car( args ), 0 );
        auto right = eval( env, car( cdr( args ) ), 0 );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) <= std::get<int64_t>( right->node ) );
        if( std::holds_alternative<int64_t>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<int64_t>( left->node ) <= std::get<double>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<int64_t>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   <= std::get<int64_t>( right->node ) );
        if( std::holds_alternative<double>( left->node ) && std::holds_alternative<double>( right->node ) )         
            return GC::make<sexpr_t>( std::get<double>( left->node )   <= std::get<double>( right->node ) );    
        throw error_t{ ERROR_SEXPR_NOT_A_NUMBER, 0 };
        return nullptr;
    }



}
