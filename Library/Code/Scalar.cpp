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
	exponent = 1;
}

/*virtual*/ Scalar::Factor::~Factor( void )
{
}

/*virtual*/ bool Scalar::Factor::IsOne( void ) const
{
	if( exponent == 0 )
		return true;

	return false;
}

/*static*/ int Scalar::Factor::SortCompare( const Factor* factorA, const Factor* factorB )
{
	int typeA = factorA->ReturnType();
	int typeB = factorB->ReturnType();

	if( typeA < typeB )
		return -1;

	if( typeA > typeB )
		return 1;

	return 0;
}

Scalar::NumericalFactor::NumericalFactor( double number )
{
	this->number = number;
}

/*virtual*/ Scalar::NumericalFactor::~NumericalFactor( void )
{
}

/*virtual*/ Scalar::Factor::Type Scalar::NumericalFactor::ReturnType( void ) const
{
	return NUMERICAL;
}

/*virtual*/ Scalar::Factor* Scalar::NumericalFactor::Clone( void ) const
{
	return new NumericalFactor( number );
}

/*virtual*/ bool Scalar::NumericalFactor::CombineWith( const Factor* factor )
{
	if( factor->ReturnType() != NUMERICAL )
		return false;

	const NumericalFactor* numericalFactor = ( const NumericalFactor* )factor;
	number *= numericalFactor->number;
	return true;
}

/*virtual*/ bool Scalar::NumericalFactor::IsOne( void ) const
{
	if( number == 1.0 )
		return true;
	
	return false;
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

/*virtual*/ Scalar::Factor::Type Scalar::VariableFactor::ReturnType( void ) const
{
	return VARIABLE;
}

/*virtual*/ Scalar::Factor* Scalar::VariableFactor::Clone( void ) const
{
	return new VariableFactor( name );
}

/*virtual*/ bool Scalar::VariableFactor::CombineWith( const Factor* factor )
{
	if( factor->ReturnType() != VARIABLE )
		return false;

	const VariableFactor* variableFactor = ( const VariableFactor* )factor;

	if( strcmp( name, variableFactor->name ) != 0 )
		return false;

	exponent += factor->exponent;
	return true;
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

/*virtual*/ Scalar::Factor::Type Scalar::InnerProductFactor::ReturnType( void ) const
{
	return INNER_PRODUCT;
}

/*virtual*/ Scalar::Factor* Scalar::InnerProductFactor::Clone( void ) const
{
	return new InnerProductFactor( vectorA, vectorB );
}

/*virtual*/ bool Scalar::InnerProductFactor::CombineWith( const Factor* factor )
{
	if( factor->ReturnType() != INNER_PRODUCT )
		return false;

	const InnerProductFactor* innerProductFactor = ( const InnerProductFactor* )factor;

	if( strcmp( vectorA, innerProductFactor->vectorA ) != 0 )
		return false;

	if( strcmp( vectorB, innerProductFactor->vectorB ) != 0 )
		return false;

	exponent += factor->exponent;
	return true;
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

void Scalar::Term::CombineFactors( void )
{
	ProductOfFactors::Node* nodeA = productOfFactors.Head();
	while( nodeA )
	{
		Factor* factorA = nodeA->data;

		ProductOfFactors::Node* nodeB = nodeA->Next();
		while( nodeB )
		{
			Factor* factorB = nodeB->data;

			ProductOfFactors::Node* nextNodeB = nodeB->Next();

			if( factorA->CombineWith( factorB ) )
				productOfFactors.Remove( nodeB );

			nodeB = nextNodeB;
		}

		nodeA = nodeA->Next();
	}

	nodeA = productOfFactors.Head();
	while( nodeA )
	{
		Factor* factorA = nodeA->data;

		ProductOfFactors::Node* nextNodeA = nodeA->Next();

		if( factorA->IsOne() )
			productOfFactors.Remove( nodeA );

		nodeA = nextNodeA;
	}
}

// Scalar.cpp