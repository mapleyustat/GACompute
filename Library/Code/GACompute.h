// GACompute.h

#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

namespace GACompute
{
	class Scalar;
	class Multivector;
	class BilinearForm;
}

#include "Scalar.h"
#include "Multivector.h"
#include "BilinearForm.h"

extern GACompute::BilinearForm* theBilinearForm;

// STPTODO: An expression expander simply evaluates an expression tree.
//          A factorer, on the other hand, does the reverse, taking an expression
//          and turning it into an expression tree.  Support for expansions and
//          factorizations would be useful and interesting.

// GACompute.h