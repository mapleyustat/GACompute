// Scalar.h

#pragma once

class GACOMPUTE_API GACompute::Scalar
{
public:

	Scalar( void );
	~Scalar( void );

	bool Assign( double number );
	bool Assign( const char* variable );
	bool Assign( const Scalar& scalar );
	bool AssignSum( const Scalar& scalarA, const Scalar& scalarB );
	bool AssignProduct( const Scalar& scalarA, const Scalar& scalarB );
	bool AssignInnerProduct( const char* vectorA, const char* vectorB );
	bool AssignInverse( const Scalar& scalar );

	bool Negate( void );
	bool Invert( void );
	bool Scale( double number );

	bool Print( char* buffer, int bufferSize, PrintStyle style ) const;

	bool IsZero( void ) const;
	bool IsOne( void ) const;

	bool CollectTerms( void );

private:

	class Factor
	{
	public:

		Factor( void );
		virtual ~Factor( void );

		enum Type { NUMERICAL, VARIABLE, INNER_PRODUCT };

		virtual Type ReturnType( void ) const = 0;
		virtual Factor* Clone( void ) const = 0;
		virtual bool CombineWith( const Factor* factor ) = 0;
		virtual bool IsOne( void ) const;
		virtual bool IsZero( void ) const = 0;
		virtual bool Print( char* buffer, int bufferSize, PrintStyle style ) const = 0;

		int SortCompareWith( const Factor* factor ) const;

		int exponent;
	};

	class NumericalFactor : public Factor
	{
	public:

		NumericalFactor( double number );
		virtual ~NumericalFactor( void );

		virtual Type ReturnType( void ) const override;
		virtual Factor* Clone( void ) const override;
		virtual bool CombineWith( const Factor* factor ) override;
		virtual bool IsOne( void ) const override;
		virtual bool IsZero( void ) const override;
		virtual bool Print( char* buffer, int bufferSize, PrintStyle style ) const override;

		double number;
	};

	class VariableFactor : public Factor
	{
	public:

		VariableFactor( const char* name );
		virtual ~VariableFactor( void );

		virtual Type ReturnType( void ) const override;
		virtual Factor* Clone( void ) const override;
		virtual bool CombineWith( const Factor* factor ) override;
		virtual bool IsZero( void ) const override;
		virtual bool Print( char* buffer, int bufferSize, PrintStyle style ) const override;

		char* name;
	};

	class InnerProductFactor : public Factor
	{
	public:

		InnerProductFactor( const char* vectorA, const char* vectorB );
		virtual ~InnerProductFactor( void );

		virtual Type ReturnType( void ) const override;
		virtual Factor* Clone( void ) const override;
		virtual bool CombineWith( const Factor* factor ) override;
		virtual bool IsZero( void ) const override;
		virtual bool Print( char* buffer, int bufferSize, PrintStyle style ) const override;

		char* vectorA;
		char* vectorB;
	};

	class Term
	{
	public:

		Term( void );
		~Term( void );

		Term* Clone( void ) const;

		bool CombineWith( const Term* term, bool combineFactors = true, bool sortFactors = true );
		bool IsZero( void ) const;
		bool IsOne( void ) const;
		bool Print( char* buffer, int bufferSize, PrintStyle style ) const;

		// An empty list is one.
		typedef List< Factor* > ProductOfFactors;
		ProductOfFactors productOfFactors;

		ProductOfFactors::Node* FindTermOfFactorType( Factor::Type factorType );

		void CombineFactors( void );
	};

	// An empty list is zero.
	typedef List< Term* > SumOfTerms;
	SumOfTerms sumOfTermsNumerator;
	SumOfTerms sumOfTermsDenominator;

	bool CollectTerms( SumOfTerms& sumOfTerms );

	static void Multiply( SumOfTerms& sumOfTermsResult, const SumOfTerms& sumOfTermsA, const SumOfTerms& sumOfTermsB );
	static Term* Multiply( const Term* termA, const Term* termB );

	bool Print( char* buffer, int bufferSize, const SumOfTerms& sumOfTerms, PrintStyle style ) const;
};

// Scalar.h