// Scalar.h

#pragma once

class GACompute::Scalar
{
public:

	Scalar( void );
	~Scalar( void );

	void Assign( double number );
	void Assign( const char* variable );
	void Assign( const Scalar& scalar );
	void AssignSum( const Scalar& scalarA, const Scalar& scalarB );
	void AssignProduct( const Scalar& scalarA, const Scalar& scalarB );
	void AssignInnerProduct( const char* vectorA, const char* vectorB );
	bool AssignInverse( const Scalar& scalar );

private:

	class Factor
	{
	public:

		Factor( void );
		virtual ~Factor( void );

		virtual Factor* Clone( void ) const = 0;

		int exponent;
	};

	class NumericalFactor : public Factor
	{
	public:

		NumericalFactor( double number );
		virtual ~NumericalFactor( void );

		virtual Factor* Clone( void ) const override;

		double number;
	};

	class VariableFactor : public Factor
	{
	public:

		VariableFactor( const char* name );
		virtual ~VariableFactor( void );

		virtual Factor* Clone( void ) const override;

		char* name;
	};

	class InnerProductFactor : public Factor
	{
	public:

		InnerProductFactor( const char* vectorA, const char* vectorB );
		virtual ~InnerProductFactor( void );

		virtual Factor* Clone( void ) const override;

		char* vectorA;
		char* vectorB;
	};

	class Term
	{
	public:

		Term( void );
		~Term( void );

		Term* Clone( void ) const;

		// An empty list is one.
		typedef List< Factor* > ProductOfFactors;
		ProductOfFactors productOfFactors;

		void CollectFactors( void );
	};

	// An empty list is zero.
	typedef List< Term* > SumOfTerms;
	SumOfTerms sumOfTermsNumerator;
	SumOfTerms sumOfTermsDenominator;

	void CollectTerms( void );
};

// Scalar.h