// Scalar.cpp

#include "GACompute.h"

using namespace GACompute;

Scalar::Scalar( void )
{
	sumOfTermsDenominator.InsertAfter()->data = new Term();
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
	sumOfTermsNumerator.Copy( scalar.sumOfTermsNumerator );
	sumOfTermsDenominator.Copy( scalar.sumOfTermsDenominator );
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

Scalar::NumericalFactor::NumericalFactor( double number )
{
	this->number = number;
}

/*virtual*/ Scalar::NumericalFactor::~NumericalFactor( void )
{
}

/*virtual*/ Scalar::Factor* Scalar::NumericalFactor::Clone( void ) const
{
	return new NumericalFactor( number );
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

/*virtual*/ Scalar::Factor* Scalar::VariableFactor::Clone( void ) const
{
	return new VariableFactor( name );
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

/*virtual*/ Scalar::Factor* Scalar::InnerProductFactor::Clone( void ) const
{
	return new InnerProductFactor( vectorA, vectorB );
}

Scalar::Term::Term( void )
{
}

Scalar::Term::~Term( void )
{
}

Scalar::Term* Scalar::Term::Clone( void ) const
{
	Term* clonedTerm = new Term();

	clonedTerm->productOfFactors.Copy( productOfFactors );

	return clonedTerm;
}

void Scalar::Term::CollectFactors( void )
{
}

// Scalar.cpp