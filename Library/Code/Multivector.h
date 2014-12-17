// Multivector.h

#pragma once

class GACOMPUTE_API GACompute::Multivector
{
public:

	Multivector( void );
	~Multivector( void );

	bool Assign( const Multivector& multivector );
	bool AssignScalar( const Scalar& scalar );
	bool AssignVector( const char* name );
	bool AssignSum( const Multivector& multivectorA, const Multivector& multivectorB );
	bool AssignScalarProduct( const Multivector& multivector, const Scalar& scalar );
	bool AssignInnerProduct( const Multivector& multivectorA, const Multivector& multivectorB );
	bool AssignOuterProduct( const Multivector& multivectorA, const Multivector& multivectorB );
	bool AssignGeometricProduct( const Multivector& multivectorA, const Multivector& multivectorB );
	bool AssignReverse( const Multivector& multivector );
	bool AssignInverse( const Multivector& multivector );

	bool CalculateMagnitude( Scalar& scalar ) const;

	//void Print( char* buffer, int bufferSize, PrintStyle style ) const;

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

	bool CollectTerms( Term::ProductType productType );

	bool Multiply( const Multivector& multivectorA, const Multivector& multivectorB, Term::ProductType productType );

	int CountProductTypes( Term::ProductType productType ) const;
};

// Multivector.h