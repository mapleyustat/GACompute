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
	if( this == &multivector )
		return true;

	sumOfTerms.Copy( multivector.sumOfTerms );
	return true;
}

bool Multivector::AssignScalar( const Scalar& scalar )
{
	//...
	return true;
}

bool Multivector::AssignScalar( const char* scalarName )
{
	//...
	return true;
}

bool Multivector::AssignScalar( double number )
{
	//...
	return true;
}

bool Multivector::AssignVector( const char* vectorName )
{
	//...
	return true;
}

bool Multivector::AssignSum( const Multivector& multivectorA, const Multivector& multivectorB )
{
	if( this == &multivectorA && this == &multivectorB )
	{
		Scalar scalarTwo;
		if( !scalarTwo.Assign( 2.0 ) )
			return false;

		if( !AssignScalarProduct( scalarTwo, *this ) )
			return false;

		return true;
	}
	
	bool accumulateA = true;
	bool accumulateB = true;

	if( this == &multivectorA )
		accumulateA = false;
	else if( this == &multivectorB )
		accumulateB = false;
	else
		sumOfTerms.RemoveAll();

	if( accumulateA )
	{
		for( const SumOfTerms::Node* node = multivectorA.sumOfTerms.Head(); node; node = node->Next() )
		{
			const Term* term = node->data;
			sumOfTerms.InsertAfter()->data = term->Clone();
		}
	}

	if( accumulateB )
	{
		for( const SumOfTerms::Node* node = multivectorB.sumOfTerms.Head(); node; node = node->Next() )
		{
			const Term* term = node->data;
			sumOfTerms.InsertAfter()->data = term->Clone();
		}
	}

	if( !CollectTerms( Term::OUTER_PRODUCT ) )
		return false;

	return true;
}

bool Multivector::AssignScalarProduct( const Scalar& scalarA, const Multivector& multivectorB )
{
	if( this != &multivectorB )
	{
		if( !Assign( multivectorB ) )
			return false;
	}

	for( SumOfTerms::Node* node = sumOfTerms.Head(); node; node = node->Next() )
	{
		Term* term = node->data;
		if( !term->coeficient->AssignProduct( scalarA, *term->coeficient ) )
			return false;
	}

	return true;
}

bool Multivector::AssignInnerProduct( const Multivector& multivectorA, const Multivector& multivectorB )
{
	if( multivectorA.CountProductTypes( Term::GEOMETRIC_PRODUCT ) > 0 )
		if( !const_cast< Multivector* >( &multivectorA )->CollectTerms( Term::OUTER_PRODUCT ) )
			return false;

	if( multivectorB.CountProductTypes( Term::GEOMETRIC_PRODUCT ) > 0 )
		if( !const_cast< Multivector* >( &multivectorB )->CollectTerms( Term::OUTER_PRODUCT ) )
			return false;

	Multivector* multivectorResult = this;

	Multivector multivectorStorage;
	if( this == &multivectorA || this == &multivectorB )
		multivectorResult = &multivectorStorage;

	for( const SumOfTerms::Node* nodeA = multivectorA.sumOfTerms.Head(); nodeA; nodeA = nodeA->Next() )
	{
		const Term* termA = nodeA->data;

		for( const SumOfTerms::Node* nodeB = multivectorB.sumOfTerms.Head(); nodeB; nodeB = nodeB->Next() )
		{
			const Term* termB = nodeB->data;

			Multivector innerProductMultivector;
			if( !termA->InnerProductMultiply( termB, innerProductMultivector ) )
				return false;

			Scalar scalar;
			if( !scalar.AssignProduct( *termA->coeficient, *termB->coeficient ) )
				return false;

			if( !innerProductMultivector.AssignScalarProduct( scalar, innerProductMultivector ) )
				return false;

			multivectorResult->sumOfTerms.Absorb( &innerProductMultivector.sumOfTerms );
		}
	}

	if( multivectorResult == &multivectorStorage )
		return Assign( multivectorStorage );

	return true;
}

bool Multivector::Term::InnerProductMultiply( const Term* term, Multivector& multivector ) const
{
	if( productType != OUTER_PRODUCT || term->productType != OUTER_PRODUCT )
		return false;

	//...

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
	if( this != &multivector )
		if( !Assign( multivector ) )
			return false;

	return Reverse();
}

bool Multivector::AssignInverse( const Multivector& multivector )
{
	// STPTODO: This is not a trivial problem.  It amounts to seting up and solving a linear system of equations.
	//          With symbolic variables in the mix, this makes it even less trivial.
	return false;
}

bool Multivector::Negate( void )
{
	return Scale( -1.0 );
}

bool Multivector::Scale( double number )
{
	for( SumOfTerms::Node* node = sumOfTerms.Head(); node; node = node->Next() )
	{
		Term* term = node->data;
		if( !term->coeficient->Scale( number ) )
			return false;
	}

	return true;
}

bool Multivector::Reverse( void )
{
	if( !CollectTerms( Term::GEOMETRIC_PRODUCT ) )
		return false;

	for( SumOfTerms::Node* node = sumOfTerms.Head(); node; node = node->Next() )
	{
		Term* term = node->data;
		if( !term->Reverse() )
			return false;
	}

	return true;
}

bool Multivector::CalculateMagnitude( Scalar& scalar ) const
{
	//...
	return false;
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

int Multivector::Vector::SortCompareWith( const Vector* vector ) const
{
	return strcmp( name, vector->name );
}

Multivector::Term::Term( ProductType productType, Scalar* coeficient /*= 0*/ )
{
	this->productType = productType;
	this->coeficient = new Scalar();

	// STPTODO: We might provide an option here to take over the given scalar memory,
	//          since copying the scalar could be expensive.

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

bool Multivector::Term::Reverse( void )
{
	if( productType != GEOMETRIC_PRODUCT )
		return false;

	ProductOfVectors reverseProduct;

	while( productOfVectors.Count() > 0 )
	{
		ProductOfVectors::Node* node = productOfVectors.Head();
		productOfVectors.Remove( node, false );
		reverseProduct.InsertBefore( 0, node );
	}

	productOfVectors.Absorb( &reverseProduct );

	return true;
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

	if( productType == Term::OUTER_PRODUCT )
	{
		// STPTODO: Presort all term products, accounting for the sign change.  This way we can use CombineWidth( ..., false ) to save time.
	}

	// STPTODO: Combine terms here.

	// STPTODO: Cull zero terms here.

	return true;
}

bool Multivector::Term::CombineWith( const Term* term, bool sortVectors /*= true*/ )
{
	if( productType != term->productType )
		return false;

	if( productOfVectors.Count() != term->productOfVectors.Count() )
		return false;

	if( sortVectors )
	{
		SortProduct();

		const_cast< Term* >( term )->SortProduct();
	}

	ProductOfVectors* productOfVectorsA = &productOfVectors;
	ProductOfVectors* productOfVectorsB = &const_cast< Term* >( term )->productOfVectors;

	ProductOfVectors::Node* nodeA = productOfVectorsA->Head();
	ProductOfVectors::Node* nodeB = productOfVectorsB->Head();

	while( nodeA && nodeB )
	{
		Vector* vectorA = nodeA->data;
		Vector* vectorB = nodeB->data;

		if( 0 != strcmp( vectorA->name, vectorB->name ) )
			return false;

		nodeA = nodeA->Next();
		nodeB = nodeB->Next();
	}

	if( !coeficient->AssignSum( *coeficient, *term->coeficient ) )
		return false;

	return true;
}

bool Multivector::Term::SortProduct( void )
{
	if( productType == GEOMETRIC_PRODUCT )
		return false;

	int adjacentSwapCount = productOfVectors.Sort( ProductOfVectors::SORT_ASCENDING );
	if( adjacentSwapCount % 2 )
		return coeficient->Negate();

	return true;
}

// Note that in the interest of efficiency, we do not preserve this term in this process.
// Also note that we accumulate into the given multivector.
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

	ProductOfVectors::Node* node = productOfVectors.Head();
	Vector* vector = node->data;
	productOfVectors.Remove( node, false );
		
	bool success = false;

	do
	{
		if( targetProductType == GEOMETRIC_PRODUCT )
		{
			Multivector termMultivector;
			termMultivector.sumOfTerms.InsertAfter()->data = Clone();
			
			Multivector innerProductMultivector;
			if( !innerProductMultivector.InnerProductMultiply( *vector, termMultivector ) )
				break;

			innerProductMultivector.Negate();

			while( innerProductMultivector.sumOfTerms.Count() > 0 )
			{
				SumOfTerms::Node* node = innerProductMultivector.sumOfTerms.Head();
				Term* term = node->data;

				if( !term->PerformProductMorphism( multivector ) )
					break;

				innerProductMultivector.sumOfTerms.Remove( node, true );
			}

			if( innerProductMultivector.sumOfTerms.Count() > 0 )
				break;
		}

		Multivector subMultivector;
		if( !PerformProductMorphism( subMultivector ) )
			break;

		if( targetProductType == OUTER_PRODUCT )
		{
			if( !multivector.InnerProductMultiply( *vector, subMultivector ) )
				break;

			if( !multivector.OuterProductMultiply( *vector, subMultivector ) )
				break;
		}
		else if( targetProductType == GEOMETRIC_PRODUCT )
		{
			if( !multivector.GeometricProductMultiply( *vector, subMultivector ) )
				break;
		}

		success = true;
	}
	while( false );

	delete node;

	return success;
}

const Multivector::Term::ProductOfVectors::Node* Multivector::Term::FindVectorWithName( const char* name ) const
{
	const ProductOfVectors::Node* node = productOfVectors.Head();
	while( node )
	{
		const Vector* vector = node->data;
		if( 0 == strcmp( name, vector->name ) )
			break;

		node = node->Next();
	}

	return node;
}

// Note that we accumulate into this multivector and fail unless every term of the given multivector is an outer product.
// Every accumulated term will be an outer product.
bool Multivector::OuterProductMultiply( const Vector& vectorA, const Multivector& multivectorB )
{
	for( const SumOfTerms::Node* node = multivectorB.sumOfTerms.Head(); node; node = node->Next() )
	{
		const Term* term = node->data;
		if( term->productType != Term::OUTER_PRODUCT )
			return false;

		if( term->FindVectorWithName( vectorA.name ) )
			continue;

		Term* newTerm = term->Clone();
		newTerm->productOfVectors.InsertBefore()->data = vectorA.Clone();
	}

	return true;
}

// Note that we accumulate into this multivector and fail unless every term of the given multivector is an outer product.
// Every accumulated term will be an outer product.
bool Multivector::InnerProductMultiply( const Vector& vectorA, const Multivector& multivectorB )
{
	for( const SumOfTerms::Node* node = multivectorB.sumOfTerms.Head(); node; node = node->Next() )
	{
		const Term* term = node->data;
		if( term->productType != Term::OUTER_PRODUCT )
			return false;

		if( !term->InnerProductMultiply( vectorA, *this, false ) )
			return false;
	}

	return false;
}

bool Multivector::Term::InnerProductMultiply( const Vector& vector, Multivector& multivector, bool vectorRight ) const
{
	if( productType != OUTER_PRODUCT )
		return false;

	if( productOfVectors.Count() == 0 )
	{
		Term* newTerm = new Term( OUTER_PRODUCT );
		newTerm->coeficient->Assign( *coeficient );
		newTerm->productOfVectors.InsertAfter()->data = vector.Clone();
		multivector.sumOfTerms.InsertAfter()->data = newTerm;
		return true;
	}

	bool negateNewTerms = false;
	if( vectorRight && productOfVectors.Count() % 2 == 0 )
		negateNewTerms = true;

	ProductOfVectors::Node* node = const_cast< ProductOfVectors* >( &productOfVectors )->Head();

	do
	{
		ProductOfVectors::Node* nextNode = node->Next();

		const_cast< ProductOfVectors* >( &productOfVectors )->Remove( node );

		Term* newTerm = Clone();
		multivector.sumOfTerms.InsertAfter()->data = newTerm;

		Scalar* innerProductScalar = theBilinearForm->InnerProduct( vector.name, node->data->name );
		if( !newTerm->coeficient->AssignProduct( *newTerm->coeficient, *innerProductScalar ) )
			return false;

		if( negateNewTerms )
			newTerm->coeficient->Negate();

		if( nextNode )
			const_cast< ProductOfVectors* >( &productOfVectors )->InsertBefore( nextNode, node );
		else
			const_cast< ProductOfVectors* >( &productOfVectors )->InsertAfter( 0, node );

		node = nextNode;
	}
	while( node );

	return true;
}

// Note that we accumulate into this multivector and fail unless every term of the given multivector is a geometric product.
// Every accumulated term will be a geometric product.
bool Multivector::GeometricProductMultiply( const Vector& vectorA, const Multivector& multivectorB )
{
	for( const SumOfTerms::Node* node = multivectorB.sumOfTerms.Head(); node; node = node->Next() )
	{
		const Term* term = node->data;
		if( term->productType != Term::GEOMETRIC_PRODUCT )
			return false;

		Term* newTerm = term->Clone();
		newTerm->productOfVectors.InsertBefore()->data = vectorA.Clone();
	}

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