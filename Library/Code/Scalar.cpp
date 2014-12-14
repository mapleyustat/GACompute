// Scalar.cpp

#include "GACompute.h"

using namespace GACompute;

Scalar::Scalar( void )
{
}

Scalar::~Scalar( void )
{
}

void Scalar::Assign( double number )
{
}

void Scalar::Assign( const char* variable )
{
}

void Scalar::Assign( const Scalar& scalar )
{
}

void Scalar::AssignSum( const Scalar& scalarA, const Scalar& scalarB )
{
}

void Scalar::AssignProduct( const Scalar& scalarA, const Scalar& scalarB )
{
}

void Scalar::AssignInnerProduct( const char* vectorA, const char* vectorB )
{
}

bool Scalar::AssignInverse( const Scalar& scalar )
{
	return false;
}

void Scalar::CollectTerms( void )
{
}

Scalar::Factor::Factor( void )
{
	exponent = 0;
}

/*virtual*/ Scalar::Factor::~Factor( void )
{
}

Scalar::NumericalFactor::NumericalFactor( void )
{
	number = 0.0;
}

/*virtual*/ Scalar::NumericalFactor::~NumericalFactor( void )
{
}

Scalar::VariableFactor::VariableFactor( const char* name )
{
	int length = strlen( name ) + 1;
	this->name = new char[ length ];
	strcpy_s( this->name, length, name );
}

/*virtual*/ Scalar::VariableFactor::~VariableFactor( void )
{
	delete[] name;
}

Scalar::InnerProductFactor::InnerProductFactor( const char* vectorA, const char* vectorB )
{
	int length = strlen( vectorA ) + 1;
	this->vectorA = new char[ length ];
	strcpy_s( this->vectorA, length, vectorA );

	length = strlen( vectorB ) + 1;
	this->vectorB = new char[ length ];
	strcpy_s( this->vectorB, length, vectorB );
}

/*virtual*/ Scalar::InnerProductFactor::~InnerProductFactor( void )
{
	delete[] vectorA;
	delete[] vectorB;
}

Scalar::Term::Term( void )
{
}

Scalar::Term::~Term( void )
{
}

void Scalar::Term::CollectFactors( void )
{
}

// Scalar.cpp