//
//  GJKAlgorithm.h
//  GJK2D
//
//  Created by Prashant on 21/07/11.
//


#include "Simplex.h"

#ifndef _GJK_ALGORITHM_H_
#define _GJK_ALGORITHM_H_

BEGIN_NAMESPACE_DIRAC2D

class CollisionShape;
class GJKAlgorithm
{
public:

	static GJKAlgorithm* getInstance()
	{
		if( !m_Instance )
		{
			m_Instance = new GJKAlgorithm();
		}
		return m_Instance;
	}
	
	dbool pointInsideShape( CollisionShape& polygon, Vector2f& p );
	
	dbool intersectShapes( CollisionShape* polygon1, Matrix3f m1, CollisionShape* polygon2, Matrix3f m2 );
	
	dbool intersectShapes( CollisionShape& polygon1, Matrix3f& m1, CollisionShape& polygon2, Matrix3f& m2, Simplexf& simplex );

	dbool intersectShapesStepWise( CollisionShape& polygon1, Matrix3f& m1, CollisionShape& polygon2, Matrix3f& m2 );

	dbool pointInsideShapeStepWise( CollisionShape& polygon, Vector2f& p );
	
	void initGJKStepWise(CollisionShape& polygon, Vector2f& p);

	Simplexf simplex;

	static int numIterations;

private:
	
	static GJKAlgorithm *m_Instance;
	
	Vector2f direction;
	
	GJKAlgorithm()
	{
	}
};

#endif

END_NAMESPACE_DIRAC2D