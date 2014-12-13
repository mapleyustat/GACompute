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

		char* name;
	};

	class Term
	{
	public:

		Term( void );
		~Term( void );

		Scalar* coeficient;

		List< Vector*, DeletingDataFactory< Vector* > > productOfVectors;

		enum ProductType { GEOMETRIC_PRODUCT, OUTER_PRODUCT };
		ProductType productType;
	};

	List< Term*, DeletingDataFactory< Term* > > sumOfTerms;

	void CollectTerms( void );

	void TransformTerms( Term::ProductType productType );

	void AssignTerm( const Term& term, Term::ProductType productType );
};

// Multivector.h