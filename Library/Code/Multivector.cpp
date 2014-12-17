// Multivector.cpp

#include "GACompute.h"

using namespace GACompute;

Multivector::Multivector( void )
{
}

Multivector::~Multivector( void )
{
}

bool Multivector::Assign( const Multivector& multivector )
{
	sumOfTerms.Copy( multivector.sumOfTerms );

	return true;
}

bool Multivector::AssignScalar( const Scalar& scalar )
{
	return true;
}

bool Multivector::AssignVector( const char* name )
{
	return true;
}

bool Multivector::AssignSum( const Multivector& multivectorA, const Multivector& multivectorB )
{
	return true;
}

bool Multivector::AssignScalarProduct( const Multivector& multivector, const Scalar& scalar )
{
	return true;
}

bool Multivector::AssignInnerProduct( const Multivector& multivectorA, const Multivector& multivectorB )
{
	return true;
}

bool Multivector::AssignOuterProduct( const Multivector& multivectorA, const Multivector& multivectorB )
{
	return Multiply( multivectorA, multivectorB, Term::OUTER_PRODUCT );
}

bool Multivector::AssignGeometricProduct( const Multivector& multivectorA, const Multivector& multivectorB )
{
	return Multiply( multivectorA, multivectorB, Term::GEOMETRIC_PRODUCT );
}

bool Multivector::Multiply( const Multivector& multivectorA, const Multivector& multivectorB, Term::ProductType productType )
{
	Term::ProductType otherProductType;
	if( productType == Term::OUTER_PRODUCT )
		otherProductType = Term::GEOMETRIC_PRODUCT;
	else if( productType == Term::GEOMETRIC_PRODUCT )
		otherProductType = Term::OUTER_PRODUCT;
	else
		return false;

	if( multivectorA.CountProductTypes( otherProductType ) > 0 )
		if( !const_cast< Multivector* >( &multivectorA )->CollectTerms( productType ) )
			return false;

	if( multivectorB.CountProductTypes( otherProductType ) > 0 )
		if( !const_cast< Multivector* >( &multivectorB )->CollectTerms( productType ) )
			return false;

	Multivector* multivectorResult = this;

	Multivector multivectorStorage;
	if( this == &multivectorA || this == &multivectorB )
		multivectorResult = &multivectorStorage;

	multivectorResult->sumOfTerms.RemoveAll();

	for( const SumOfTerms::Node* nodeA = multivectorA.sumOfTerms.Head(); nodeA; nodeA = nodeA->Next() )
	{
		const Term* termA = nodeA->data;

		for( const SumOfTerms::Node* nodeB = multivectorB.sumOfTerms.Head(); nodeB; nodeB = nodeB->Next() )
		{
			const Term* termB = nodeB->data;

			Term* termResult = new Term();
			termResult->productType = productType;
			multivectorResult->sumOfTerms.InsertAfter()->data = termResult;

			if( !termResult->coeficient->AssignProduct( *termA->coeficient, *termB->coeficient ) )
				return false;

			for( const Term::ProductOfVectors::Node* node = termA->productOfVectors.Head(); node; node = node->Next() )
				termResult->productOfVectors.InsertAfter()->data = node->data->Clone();

			for( const Term::ProductOfVectors::Node* node = termB->productOfVectors.Head(); node; node = node->Next() )
				termResult->productOfVectors.InsertAfter()->data = node->data->Clone();
		}
	}

	if( !multivectorResult->CollectTerms( productType ) )
		return false;

	if( multivectorResult == &multivectorStorage )
		return Assign( multivectorStorage );

	return true;
}

bool Multivector::AssignReverse( const Multivector& multivector )
{
	return true;
}

bool Multivector::AssignInverse( const Multivector& multivector )
{
	return false;
}

bool Multivector::CalculateMagnitude( Scalar& scalar ) const
{
	return true;
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

Multivector::Vector* Multivector::Vector::Clone( void ) const
{
	return new Vector( name );
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

Multivector::Term* Multivector::Term::Clone( void ) const
{
	Term* clonedTerm = new Term();

	clonedTerm->coeficient->Assign( *coeficient );
	clonedTerm->productType = productType;
	clonedTerm->productOfVectors.Copy( productOfVectors );

	return clonedTerm;
}

bool Multivector::CollectTerms( Term::ProductType productType )
{
	return true;
}

int Multivector::CountProductTypes( Term::ProductType productType ) const
{
	int count = 0;

	for( const SumOfTerms::Node* node = sumOfTerms.Head(); node; node = node->Next() )
	{
		const Term* term = node->data;

		if( term->productType == productType )
			count++;
	}

	return count;
}

// Multivector.cpp