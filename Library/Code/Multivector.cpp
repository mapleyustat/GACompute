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

/*static*/ Multivector::Term::ProductType Multivector::Term::OtherProductType( ProductType productType )
{
	if( productType == OUTER_PRODUCT )
		return GEOMETRIC_PRODUCT;

	return OUTER_PRODUCT;
}

bool Multivector::Multiply( const Multivector& multivectorA, const Multivector& multivectorB, Term::ProductType productType )
{
	Term::ProductType otherProductType = Term::OtherProductType( productType );

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

			Term* termResult = new Term( productType );
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

Multivector::Term::Term( ProductType productType, Scalar* coeficient /*= 0*/ )
{
	this->productType = productType;
	this->coeficient = new Scalar();

	// STPTODO: We might provide an option here to take over the given scalar memory, since copying the scalar could be expensive.

	if( coeficient )
		this->coeficient->Assign( *coeficient );
}

Multivector::Term::~Term( void )
{
	delete coeficient;
}

Multivector::Term* Multivector::Term::Clone( void ) const
{
	Term* clonedTerm = new Term( productType );

	clonedTerm->coeficient->Assign( *coeficient );
	clonedTerm->productOfVectors.Copy( productOfVectors );

	return clonedTerm;
}

Multivector::SumOfTerms::Node* Multivector::FindTermOfProductType( Term::ProductType productType )
{
	SumOfTerms::Node* node = sumOfTerms.Head();

	while( node )
	{
		Term* term = node->data;

		if( term->productType == productType )
			break;

		node = node->Next();
	}

	return node;
}

bool Multivector::CollectTerms( Term::ProductType productType )
{
	Term::ProductType otherProductType = Term::OtherProductType( productType );

	for(;;)
	{
		SumOfTerms::Node* node = FindTermOfProductType( otherProductType );
		if( !node )
			break;

		Term* term = node->data;

		Multivector multivector;
		if( !term->PerformProductMorphism( multivector ) )
			return false;

		sumOfTerms.Remove( node, true );
		sumOfTerms.Absorb( &multivector.sumOfTerms );
	}

	//...

	return true;
}

// Note that in the interest of efficiency, we do not preserve this term in this process.
bool Multivector::Term::PerformProductMorphism( Multivector& multivector )
{
	ProductType targetProductType = OtherProductType( productType );

	if( productOfVectors.Count() == 1 )
	{
		ProductOfVectors::Node* node = productOfVectors.Head();
		productOfVectors.Remove( node, false );

		Term* term = new Term( targetProductType, coeficient );
		term->productOfVectors.InsertAfter( 0, node );
		multivector.sumOfTerms.InsertAfter()->data = term;

		return true;
	}
	else if( productOfVectors.Count() == 0 )
	{
		Term* term = new Term( targetProductType, coeficient );
		multivector.sumOfTerms.InsertAfter()->data = term;

		return true;
	}

	Scalar* scalar = 0;
	
	if( !coeficient->IsOne() )
	{
		scalar = coeficient;
		coeficient = new Scalar();
		coeficient->Assign( 1.0 );
	}

	ProductOfVectors::Node* node = productOfVectors.Head();
	Vector* vector = node->data;
	productOfVectors.Remove( node, false );
		
	bool success = false;

	do
	{
		Multivector subMultivector;
		if( !PerformProductMorphism( subMultivector ) )
			break;

		if( !multivector.InnerProductMultiply( *vector, subMultivector, targetProductType ) )
			break;

		if( targetProductType == GEOMETRIC_PRODUCT )
		{
			if( !multivector.OuterProductMultiply( *vector, subMultivector, OUTER_PRODUCT ) )
				break;
		}
		else
		{
			// STPTODO: Negate multivector here.

			if( !multivector.GeometricProductMultiply( *vector, subMultivector, GEOMETRIC_PRODUCT ) )
				break;
		}

		success = true;
	}
	while( false );

	delete node;

	if( scalar )
	{
		if( success )
		{
			// Scale multivector by scalar here...
		}

		delete scalar;
	}

	return success;
}

// Note that we accumulate into this multivector and fail if not all terms are of the given type.
bool Multivector::OuterProductMultiply( const Vector& vectorA, const Multivector& multivectorB, Term::ProductType homogeneousProductType )
{
	if( homogeneousProductType == Term::GEOMETRIC_PRODUCT )
		return false;		// Not yet supported.  (Could write it in terms of other functions.)

	return true;
}

// Note that we accumulate into this multivector and fail if not all terms are of the given type.
bool Multivector::InnerProductMultiply( const Vector& vectorA, const Multivector& multivectorB, Term::ProductType homogeneousProductType )
{
	return false;
}

// Note that we accumulate into this multivector and fail if not all terms are of the given type.
bool Multivector::GeometricProductMultiply( const Vector& vectorA, const Multivector& multivectorB, Term::ProductType homogeneousProductType )
{
	if( homogeneousProductType == Term::OUTER_PRODUCT )
		return false;		// Not yet supported.  (Could write it in terms of other functions.)

	return false;
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

bool Multivector::IsHomogeneousOfProductType( Term::ProductType productType ) const
{
	if( CountProductTypes( productType ) == sumOfTerms.Count() )
		return true;

	return false;
}

bool Multivector::IsHomogeneousOfGrade( int grade ) const
{
	for( const SumOfTerms::Node* node = sumOfTerms.Head(); node; node = node->Next() )
	{
		const Term* term = node->data;

		if( term->Grade() != grade )
			return false;
	}

	return true;
}

int Multivector::Term::Grade( void ) const
{
	if( productType != OUTER_PRODUCT )
		return -1;

	return productOfVectors.Count();
}

// Multivector.cpp