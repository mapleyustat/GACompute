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

		enum ProductType { GEOMETRIC_PRODUCT, OUTER_PRODUCT };

		Term( ProductType productType, Scalar* coeficient = 0 );
		~Term( void );

		Term* Clone( void ) const;

		int Grade( void ) const;

		Scalar* coeficient;

		// An empty list is one.
		typedef List< Vector* > ProductOfVectors;
		ProductOfVectors productOfVectors;

		ProductType productType;

		static ProductType OtherProductType( ProductType productType );

		// Note that in the interest of efficiency, this method is distructive to this term.
		bool PerformProductMorphism( Multivector& multivector );
	};

	// An empty list is zero.
	typedef List< Term* > SumOfTerms;
	SumOfTerms sumOfTerms;

	bool CollectTerms( Term::ProductType productType );

	bool IsHomogeneousOfProductType( Term::ProductType productType ) const;
	bool IsHomogeneousOfGrade( int grade ) const;

	SumOfTerms::Node* FindTermOfProductType( Term::ProductType productType );

	// This performs the desired product between the two given multivectors.
	bool Multiply( const Multivector& multivectorA, const Multivector& multivectorB, Term::ProductType productType );

	// These perform the named product, expecting the given multivector to be homogeneous of the given product type.
	// The resulting multivector will also be homogeneous of the given grade.
	bool OuterProductMultiply( const Vector& vectorA, const Multivector& multivectorB, Term::ProductType homogeneousProductType );
	bool InnerProductMultiply( const Vector& vectorA, const Multivector& multivectorB, Term::ProductType homogeneousProductType );
	bool GeometricProductMultiply( const Vector& vectorA, const Multivector& multivectorB, Term::ProductType homogeneousProductType );

	int CountProductTypes( Term::ProductType productType ) const;
};

// Multivector.h