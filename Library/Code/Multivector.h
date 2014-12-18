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
	bool AssignScalarProduct( const Scalar& scalarB, const Multivector& multivectorA );
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

		int SortCompareWith( const Vector* vector ) const;

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

		bool CombineWith( const Term* term, bool sortVectors = true );
		bool SortProduct( void );

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

	bool Multiply( const Multivector& multivectorA, const Multivector& multivectorB, Term::ProductType productType );

	bool OuterProductMultiply( const Vector& vectorA, const Multivector& multivectorB );
	bool InnerProductMultiply( const Vector& vectorA, const Multivector& multivectorB );
	bool GeometricProductMultiply( const Vector& vectorA, const Multivector& multivectorB );

	int CountProductTypes( Term::ProductType productType ) const;
};

// Multivector.h