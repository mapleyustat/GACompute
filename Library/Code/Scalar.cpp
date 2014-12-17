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

bool Scalar::Assign( double number )
{
	sumOfTermsNumerator.RemoveAll();
	sumOfTermsDenominator.RemoveAll();

	Term* term = new Term();
	term->productOfFactors.InsertAfter()->data = new NumericalFactor( number );
	sumOfTermsNumerator.InsertAfter()->data = term;

	sumOfTermsDenominator.InsertAfter()->data = new Term();

	return true;
}

bool Scalar::Assign( const char* variable )
{
	sumOfTermsNumerator.RemoveAll();
	sumOfTermsDenominator.RemoveAll();

	Term* term = new Term();
	term->productOfFactors.InsertAfter()->data = new VariableFactor( variable );
	sumOfTermsNumerator.InsertAfter()->data = term;

	sumOfTermsDenominator.InsertAfter()->data = new Term();

	return true;
}

bool Scalar::AssignInnerProduct( const char* vectorA, const char* vectorB )
{
	sumOfTermsNumerator.RemoveAll();
	sumOfTermsDenominator.RemoveAll();

	Term* term = new Term();
	term->productOfFactors.InsertAfter()->data = new InnerProductFactor( vectorA, vectorB );
	sumOfTermsNumerator.InsertAfter()->data = term;

	sumOfTermsDenominator.InsertAfter()->data = new Term();

	return true;
}

bool Scalar::Assign( const Scalar& scalar )
{
	sumOfTermsNumerator.Copy( scalar.sumOfTermsNumerator );
	sumOfTermsDenominator.Copy( scalar.sumOfTermsDenominator );

	return true;
}

bool Scalar::AssignSum( const Scalar& scalarA, const Scalar& scalarB )
{
	Scalar* scalarResult = this;

	Scalar scalarStorage;
	if( this == &scalarA || this == &scalarB )
		scalarResult = &scalarStorage;

	scalarResult->sumOfTermsNumerator.RemoveAll();
	Multiply( scalarResult->sumOfTermsNumerator, scalarA.sumOfTermsNumerator, scalarB.sumOfTermsDenominator );
	Multiply( scalarResult->sumOfTermsNumerator, scalarB.sumOfTermsNumerator, scalarA.sumOfTermsDenominator );

	scalarResult->sumOfTermsDenominator.RemoveAll();
	Multiply( scalarResult->sumOfTermsDenominator, scalarA.sumOfTermsDenominator, scalarB.sumOfTermsDenominator );

	scalarResult->CollectTerms();

	if( scalarResult == &scalarStorage )
		return Assign( scalarStorage );

	return true;
}

bool Scalar::AssignProduct( const Scalar& scalarA, const Scalar& scalarB )
{
	Scalar* scalarResult = this;

	Scalar scalarStorage;
	if( this == &scalarA || this == &scalarB )
		scalarResult = &scalarStorage;

	scalarResult->sumOfTermsNumerator.RemoveAll();
	Multiply( scalarResult->sumOfTermsNumerator, scalarA.sumOfTermsNumerator, scalarB.sumOfTermsNumerator );

	scalarResult->sumOfTermsDenominator.RemoveAll();
	Multiply( scalarResult->sumOfTermsDenominator, scalarA.sumOfTermsDenominator, scalarB.sumOfTermsDenominator );

	scalarResult->CollectTerms();

	if( scalarResult == &scalarStorage )
		return Assign( scalarStorage );

	return true;
}

// Note that we accumulate into the given result sum.
/*static*/ void Scalar::Multiply( SumOfTerms& sumOfTermsResult, const SumOfTerms& sumOfTermsA, const SumOfTerms& sumOfTermsB )
{
	for( const SumOfTerms::Node* nodeA = sumOfTermsA.Head(); nodeA; nodeA = nodeA->Next() )
	{
		const Term* termA = nodeA->data;

		for( const SumOfTerms::Node* nodeB = sumOfTermsB.Head(); nodeB; nodeB = nodeB->Next() )
		{
			const Term* termB = nodeB->data;

			Term* termProduct = Multiply( termA, termB );
			sumOfTermsResult.InsertAfter()->data = termProduct;
		}
	}
}

/*static*/ Scalar::Term* Scalar::Multiply( const Term* termA, const Term* termB )
{
	Term* termProduct = new Term();

	for( const Term::ProductOfFactors::Node* node = termA->productOfFactors.Head(); node; node = node->Next() )
	{
		Factor* factor = node->data;
		termProduct->productOfFactors.InsertAfter()->data = factor->Clone();
	}

	for( const Term::ProductOfFactors::Node* node = termB->productOfFactors.Head(); node; node = node->Next() )
	{
		Factor* factor = node->data;
		termProduct->productOfFactors.InsertAfter()->data = factor->Clone();
	}

	return termProduct;
}

bool Scalar::AssignInverse( const Scalar& scalar )
{
	if( scalar.IsZero() )
		return false;

	if( this == &scalar )
	{
		SumOfTerms sumOfTermsTemporary;

		sumOfTermsTemporary.Absorb( &sumOfTermsNumerator );
		sumOfTermsNumerator.Absorb( &sumOfTermsDenominator );
		sumOfTermsDenominator.Absorb( &sumOfTermsTemporary );
	}
	else
	{
		sumOfTermsNumerator.Copy( scalar.sumOfTermsDenominator );
		sumOfTermsDenominator.Copy( scalar.sumOfTermsNumerator );
	}

	return true;
}

bool Scalar::IsZero( void ) const
{
	const_cast< Scalar* >( this )->CollectTerms();

	if( sumOfTermsNumerator.Count() == 0 )
		return true;

	return false;
}

bool Scalar::Term::CombineWith( const Term* term, bool combineFactors /*= true*/, bool sortFactors /*= true*/ )
{
	if( combineFactors )
	{
		CombineFactors();

		// This is ugly, but by just combining factors, we know
		// that we have not changed the the given term in terms
		// of algebraic equality.
		const_cast< Term* >( term )->CombineFactors();
	}

	if( sortFactors )
	{
		productOfFactors.Sort( ProductOfFactors::SORT_ASCENDING );

		// This is ugly, but we can't make the list order mutable
		// without also making the list data mutable.
		const_cast< Term* >( term )->productOfFactors.Sort( ProductOfFactors::SORT_ASCENDING );
	}

	ProductOfFactors* productOfFactorsA = &productOfFactors;
	ProductOfFactors* productOfFactorsB = &const_cast< Term* >( term )->productOfFactors;

	ProductOfFactors::Node* nodeA = productOfFactorsA->Head();
	ProductOfFactors::Node* nodeB = productOfFactorsB->Head();

	if( !nodeA || nodeA->data->ReturnType() != Factor::NUMERICAL )
	{
		nodeA = productOfFactorsA->InsertBefore();
		nodeA->data = new NumericalFactor( 1.0 );
	}

	if( !nodeB || nodeB->data->ReturnType() != Factor::NUMERICAL )
	{
		nodeB = productOfFactorsB->InsertBefore();
		nodeB->data = new NumericalFactor( 1.0 );
	}

	if( productOfFactorsA->Count() != productOfFactorsB->Count() )
		return false;

	while( nodeA && nodeB )
	{
		const Factor* factorA = nodeA->data;
		const Factor* factorB = nodeB->data;

		if( factorA->ReturnType() != factorB->ReturnType() )
			return false;

		if( factorA->exponent != factorB->exponent )
			return false;

		switch( nodeA->data->ReturnType() )
		{
			case Factor::VARIABLE:
			{
				const VariableFactor* variableFactorA = ( const VariableFactor* )factorA;
				const VariableFactor* variableFactorB = ( const VariableFactor* )factorB;

				if( 0 != strcmp( variableFactorA->name, variableFactorB->name ) )
					return false;

				break;
			}
			case Factor::INNER_PRODUCT:
			{
				const InnerProductFactor* innerProductFactorA = ( const InnerProductFactor* )factorA;
				const InnerProductFactor* innerProductFactorB = ( const InnerProductFactor* )factorB;

				// This works, because we sort vectors taken in the (commutative) inner product.

				if( 0 != strcmp( innerProductFactorA->vectorA, innerProductFactorB->vectorA ) )
					return false;

				if( 0 != strcmp( innerProductFactorA->vectorB, innerProductFactorB->vectorB ) )
					return false;

				break;
			}
		}
	}

	nodeA = productOfFactorsA->Head();
	nodeB = productOfFactorsB->Head();

	NumericalFactor* numericalFactorA = ( NumericalFactor* )nodeA->data;
	NumericalFactor* numericalFactorB = ( NumericalFactor* )nodeB->data;

	numericalFactorA->number += numericalFactorB->number;

	return true;
}

void Scalar::CollectTerms( void )
{
	CollectTerms( sumOfTermsNumerator );
	CollectTerms( sumOfTermsDenominator );
}

void Scalar::CollectTerms( SumOfTerms& sumOfTerms )
{
	SumOfTerms::Node* node = sumOfTerms.Head();
	while( node )
	{
		Term* term = node->data;

		term->CombineFactors();
		term->productOfFactors.Sort( Term::ProductOfFactors::SORT_ASCENDING );

		node = node->Next();
	}

	SumOfTerms::Node* nodeA = sumOfTerms.Head();
	while( nodeA )
	{
		Term* termA = nodeA->data;

		SumOfTerms::Node* nodeB = nodeA->Next();
		while( nodeB )
		{
			Term* termB = nodeB->data;

			SumOfTerms::Node* nextNodeB = nodeB->Next();

			if( termA->CombineWith( termB, false, false ) )
				sumOfTerms.Remove( nodeB );

			nodeB = nextNodeB;
		}
	}

	nodeA = sumOfTerms.Head();
	while( nodeA )
	{
		Term* termA = nodeA->data;

		SumOfTerms::Node* nextNodeA = nodeA->Next();

		if( termA->IsZero() )
			sumOfTerms.Remove( nodeA );

		nodeA = nextNodeA;
	}
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

int Scalar::Factor::SortCompareWith( const Factor* factor ) const
{
	int typeA = ReturnType();
	int typeB = factor->ReturnType();

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

/*virtual*/ bool Scalar::NumericalFactor::IsZero( void ) const
{
	if( number == 0.0 )
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

/*virtual*/ bool Scalar::VariableFactor::IsZero( void ) const
{
	return false;
}

Scalar::InnerProductFactor::InnerProductFactor( const char* vectorA, const char* vectorB )
{
	const char* firstVector = vectorA;
	const char* secondVector = vectorB;

	if( strcmp( vectorA, vectorB ) > 0 )
	{
		firstVector = vectorB;
		secondVector = vectorA;
	}

	int length = strlen( firstVector ) + 1;
	this->vectorA = new char[ length ];
	strcpy_s( this->vectorA, length, firstVector );

	length = strlen( secondVector ) + 1;
	this->vectorB = new char[ length ];
	strcpy_s( this->vectorB, length, secondVector );
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

/*virtual*/ bool Scalar::InnerProductFactor::IsZero( void ) const
{
	return false;
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

bool Scalar::Term::IsZero( void ) const
{
	const ProductOfFactors::Node* node = productOfFactors.Head();
	while( node )
	{
		Factor* factor = node->data;

		if( factor->IsZero() )
			return true;

		node = node->Next();
	}

	return false;
}

// Scalar.cpp