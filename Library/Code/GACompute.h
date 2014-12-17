// GACompute.h

#pragma once

#if defined EXPORT_GACOMPUTE
#	define GACOMPUTE_API		__declspec( dllexport )
#elif defined IMPORT_GACOMPUTE
#	define GACOMPUTE_API		__declspec( dllimport )
#else
#	define GACOMPUTE_API
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "List.h"

namespace GACompute
{
	class Scalar;
	class Multivector;
	class BilinearForm;

	enum PrintStyle { PRINT_NORMAL, PRINT_LATEX };
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