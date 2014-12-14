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

		int exponent;
	};

	class NumericalFactor : public Factor
	{
	public:

		NumericalFactor( void );
		virtual ~NumericalFactor( void );

		double number;
	};

	class VariableFactor : public Factor
	{
	public:

		VariableFactor( const char* name );
		virtual ~VariableFactor( void );

		char* name;
	};

	class InnerProductFactor : public Factor
	{
	public:

		InnerProductFactor( const char* vectorA, const char* vectorB );
		virtual ~InnerProductFactor( void );

		char* vectorA;
		char* vectorB;
	};

	class Term
	{
	public:

		Term( void );
		~Term( void );

		typedef List< Factor* > ProductOfFactors;
		ProductOfFactors productOfFactors;

		void CollectFactors( void );
	};

	typedef List< Term* > SumOfTerms;
	SumOfTerms sumOfTermsNumerator;
	SumOfTerms sumOfTermsDenominator;

	void CollectTerms( void );
};

// Scalar.h