// GACompute.h

#pragma once

#if 0
namespace GACompute
{
	// put all classes at same scope and break into seperate files.
	class Multivector
	{
	public:

		class Vector;

		class Scalar
		{
			class Term			// ScalarTerm
			{
				class Factor	// ScalarFactor
				{
				};

				class Variable : public Factor	// ScalarVariable
				{
					std::string name;
				};

				class InnerProduct : public Factor	// ScalarInnerProduct
				{
					Vector* vectorA;
					Vector* vectorB;
				};

				double coeficient;

				FactorList product;
			};

			TermList numeratorSum;
			TermList denominatorSum;
		};

		class Vector
		{
			std::string name;

			// if null is returned, then we keep the product as a scalar factor
			virtual Scalar* InnerProduct( Vector* vector ) const; // default imple uses lookup on registered data?
		};

		class Term		// MultivectorTerm
		{
			Scalar coeficient;

			VectorList product;

			ProductType productType; // geometric or outer
		};

		TermList sum;
	};
}
#endif

// GACompute.h