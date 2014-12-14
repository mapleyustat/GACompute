// Multivector.h

#pragma once

class GACompute::Multivector
{
public:

	Multivector( void );
	~Multivector( void );

	void Assign( const Multivector& multivector );
	void AssignScalar( const Scalar& scalar );
	void AssignVector( const char* name );
	void AssignSum( const Multivector& multivectorA, const Multivector& multivectorB );
	void AssignScalarProduct( const Multivector& multivector, const Scalar& scalar );
	void AssignInnerProduct( const Multivector& multivectorA, const Multivector& multivectorB );
	void AssignOuterProduct( const Multivector& multivectorA, const Multivector& multivectorB );
	void AssignGeometricProduct( const Multivector& multivectorA, const Multivector& multivectorB );
	void AssignReverse( const Multivector& multivector );
	bool AssignInverse( const Multivector& multivector );

	void CalculateMagnitude( Scalar& scalar ) const;

private:

	class Vector
	{
	public:

		Vector( const char* name );
		~Vector( void );

		Vector* Clone( void ) const;

		char* name;
	};

	class Term
	{
	public:

		Term( void );
		~Term( void );

		Term* Clone( void ) const;

		Scalar* coeficient;

		// An empty list is one.
		typedef List< Vector* > ProductOfVectors;
		ProductOfVectors productOfVectors;

		enum ProductType { GEOMETRIC_PRODUCT, OUTER_PRODUCT };
		ProductType productType;
	};

	// An empty list is zero.
	typedef List< Term* > SumOfTerms;
	SumOfTerms sumOfTerms;

	void CollectTerms( void );

	void TransformTerms( Term::ProductType productType );

	void AssignTerm( const Term& term, Term::ProductType productType );
};

// Multivector.h