// BilinearForm.h

#pragma once

class GACOMPUTE_API GACompute::BilinearForm
{
public:

	BilinearForm( void );
	virtual ~BilinearForm( void );

	virtual Scalar* InnerProduct( const char* vectorA, const char* vectorB ) const = 0;
};

// BilinearForm.h