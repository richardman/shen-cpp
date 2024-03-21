
#pragma once

#include "./types.h"

namespace klambda {

    GC::ptr<sexpr_t> eval( env_t &env, GC::ptr<sexpr_t> sexpr, int tc );


}