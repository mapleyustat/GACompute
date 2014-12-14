// Multivector.cpp

#include "GACompute.h"

using namespace GACompute;

Multivector::Multivector( void )
{
}

Multivector::~Multivector( void )
{
}

void Multivector::Assign( const Multivector& multivector )
{
}

void Multivector::AssignScalar( const Scalar& scalar )
{
}

void Multivector::AssignVector( const char* name )
{
}

void Multivector::AssignSum( const Multivector& multivectorA, const Multivector& multivectorB )
{
}

void Multivector::AssignScalarProduct( const Multivector& multivector, const Scalar& scalar )
{
}

void Multivector::AssignInnerProduct( const Multivector& multivectorA, const Multivector& multivectorB )
{
}

void Multivector::AssignOuterProduct( const Multivector& multivectorA, const Multivector& multivectorB )
{
}

void Multivector::AssignGeometricProduct( const Multivector& multivectorA, const Multivector& multivectorB )
{
}

void Multivector::AssignReverse( const Multivector& multivector )
{
}

bool Multivector::AssignInverse( const Multivector& multivector )
{
	return false;
}

void Multivector::CalculateMagnitude( Scalar& scalar ) const
{
}

Multivector::Vector::Vector( const char* name )
{
	int length = strlen( name ) + 1;
	this->name = new char[ length ];
	strcpy_s( this->name, length, name );
}

Multivector::Vector::~Vector( void )
{
	delete[] name;
}

Multivector::Term::Term( void )
{
	coeficient = new Scalar();

	productType = OUTER_PRODUCT;
}

Multivector::Term::~Term( void )
{
	delete coeficient;
}

void Multivector::CollectTerms( void )
{
}

void Multivector::TransformTerms( Term::ProductType productType )
{
}

void Multivector::AssignTerm( const Term& term, Term::ProductType productType )
{
}

// Multivector.cpp