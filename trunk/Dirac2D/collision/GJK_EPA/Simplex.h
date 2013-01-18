//
//  Simplex.h
//  GJK2D
//
//  Created by Prashant on 21/07/11.
//

#include "../../Settings.h"
#include "../../definitions.h"
#include "../../maths/MathUtil.h"

#include <stdio.h>

#ifndef _SIMPLEX_H_
#define _SIMPLEX_H_

BEGIN_NAMESPACE_DIRAC2D

struct ClosestPoints
{
	Vector2f m_Point1;
	Vector2f m_Point2;
};

template< class T >
class Simplex
{
public:
	Simplex()
	{
		m_SimplexType = -1;
	}
	
	inline void addPoint1( Vector2<T>& p )
	{
		m_SupportPoint1[m_SimplexType] = p;
	}
	
	inline void addPoint2( Vector2<T>& p )
	{
		m_SupportPoint2[m_SimplexType] = p;
	}
	
	inline void addPoint( Vector2<T>& p )
	{
		m_SimplexType++;
		m_simplexPoints[m_SimplexType] = p;
	}
	
	inline void removePoint()
	{
		m_SimplexType--;
	}
	
	// check whether the simplex contains the point
	inline dbool contains( const Vector2<T>& point )
	{
		for( int i = 0; i<m_SimplexType+1; i++ )
		{
			if( m_simplexPoints[i] == point )
				return true;
		}
		return false;
	}
	
	Vector2<T> getPointOfMinNorm(const Vector2<T>& point)
	{
		Vector2<T> outPoint;
		if( m_SimplexType == 0 )
		{
			outPoint = m_simplexPoints[0];
		}
		if( m_SimplexType == 1 )
		{
			Vector2<T> d = m_simplexPoints[1] - m_simplexPoints[0];
			dfloat u = (point - m_simplexPoints[0]).dot(d)/d.lengthSquared();
			if( u < 0.0f )u = 0.0f;
			if( u > 1.0f )u = 1.0f;
			outPoint = m_simplexPoints[0] + u * d;
		}
		
		return outPoint;
	}
	
	void getClosestPoints(ClosestPoints& closestPoints)
	{
		if( m_SimplexType == 0 )
		{
			closestPoints.m_Point1 = m_SupportPoint1[0];
			closestPoints.m_Point2 = m_SupportPoint2[0];
		}
		if( m_SimplexType == 1 )
		{
			Vector2<T> d = m_simplexPoints[1] - m_simplexPoints[0];
			dfloat u = -m_simplexPoints[0].dot(d)/d.lengthSquared();
			if( u < 0.0f )u = 0.0f;
			if( u > 1.0f )u = 1.0f;
			closestPoints.m_Point1 = m_SupportPoint1[0] + u * ( m_SupportPoint1[1] - m_SupportPoint1[0] );
			closestPoints.m_Point2 = m_SupportPoint2[0] + u * ( m_SupportPoint2[1] - m_SupportPoint2[0] );
		}
		if( m_SimplexType == 2 )
		{
		}
	}
	
	dbool process( Vector2<T>& d , Vector2<T>& p);
	
	void render();
	
	friend class GJKAlgorithm;
protected:
	// Support Points. In case when polygon intersection is required these points are Minkowski difference of support points.
	Vector2<T> m_simplexPoints[3];
	// In case when polygon intersection is required these points are support points of each polygon. can be used to get closest points on the polygons.
	Vector2<T> m_SupportPoint1[3];
	Vector2<T> m_SupportPoint2[3];

	int m_SimplexType;
	
};


template< class T > dbool Simplex<T>::process( Vector2<T>& d , Vector2<T>& p)
{
	if( m_SimplexType == 0 )
		return false;
	/*   Simplex type 1 (Line Segment)
	 
	 |    2             |
	 | p1            p2 |
1	 |------------------| 4
	 |                  |
	 |    3             |
	 
	 we do not check the region 1 at all as we already know that the Point p dosen't lie in there. 
	 this is because we added point p2 as the last point in our simplex which is in the direction of 
	 the test Point p.
	 */
	if( m_SimplexType == 1 )
	{
		// Check if the point lies in reigion 4
		Vector2<T> p2p1 = m_simplexPoints[1] - m_simplexPoints[0];
		Vector2<T> p1p  = m_simplexPoints[1] - p;
		dfloat sign = p2p1.dot(p1p);
		// p lies in region 4. so the Simplex will become of type 0. set the new search direction towards the test point.
		if( sign < 0.0f )
		{
			m_simplexPoints[0] = m_simplexPoints[1];
			m_SupportPoint1[0] = m_SupportPoint1[1];
			m_SupportPoint2[0] = m_SupportPoint2[1];
			
			d = p - m_simplexPoints[1];
			m_SimplexType = 0;
			return false;
		}
		// p lies in reigion 2 or 3. Set the direction as normal to p1p2 vector( make sure the direction is towards the test point. Simplex remains the same.
		else 
		{
			Vector2<T> normal( -p2p1.y, p2p1.x );
			dfloat signNormal = p1p.dot(normal);
			// On the Edge. return true
			//if( fabs(signNormal) < EPSILON )
			//	return true;
			
			normal *= -signNormal;
			d = normal;
			return false;
		}
		
	}
	/*   Simplex type 2 (Triangle)
	 
	 2	 
	 
	 
	 B
	 |\     4
	 | \
	 |  \
1	 |   \
	 |    \
	 |  7  \
	 |------\A
	 C         5
	 6
	 
	 3	 
	 We do not need to test region 1, 2,3 as A is the latest point added to our simplex, which is in the direction of the test Point p.
	 */
	
	else if ( m_SimplexType == 2 )
	{
		Vector2<T> AB = m_simplexPoints[2] - m_simplexPoints[1];
		Vector2<T> AC = m_simplexPoints[2] - m_simplexPoints[0];
		Vector2<T> AP = m_simplexPoints[2] - p;
		AP *= -1.0f;
		//perpendicular to AB
		Vector2<T> AB_Normal(-AB.y, AB.x);
		//perpendicular to AC
		Vector2<T> AC_Normal(-AC.y, AC.x);
		
		dfloat sign = AB_Normal.dot(AC);
		AB_Normal *= sign;
		AC_Normal *= AC_Normal.dot(AB);
		
		// Region 4 OR 5
		dfloat signAB_Normal = AB_Normal.dot(AP);
		
		//if( fabs(signAB_Normal) < EPSILON )
		//	return true;
		
		float signAC_Normal = AC_Normal.dot(AP);
		
		//if( fabs(signAC_Normal) < EPSILON )
		//	return true;
		
		if( signAB_Normal >= 0.0f )
		{
			// region 4. Set the new direction. Simplex is changed and contains A and B point.
			dfloat signABAP = AB.dot(AP);
			//if( fabs(signABAP) < EPSILON )
			//	return true;
			
			if(  signABAP < 0.0f )
			{
				d = AB_Normal;
				m_simplexPoints[0] = m_simplexPoints[1];
				m_simplexPoints[1] = m_simplexPoints[2];
				
				m_SupportPoint1[0] = m_SupportPoint1[1];
				m_SupportPoint1[1] = m_SupportPoint1[2];

				m_SupportPoint2[0] = m_SupportPoint2[1];
				m_SupportPoint2[1] = m_SupportPoint2[2];

				m_SimplexType = 1;
				return false;
			}
			// region 5. Simplex becomes 0 simplex with point A. Set the new direction, which is p - A.
			else 
			{
				m_simplexPoints[0] = m_simplexPoints[2]; 
				
				m_SupportPoint1[0] = m_SupportPoint1[2];
				m_SupportPoint2[0] = m_SupportPoint2[2];

				d = p - m_simplexPoints[0];
				m_SimplexType = 0;
				return false;
			}
			
		}
		// Region 6 OR 5
		else if ( signAC_Normal >= 0.0f )
		{
			// region 6. Set the new direction. Simplex is changed and contains A and C point.
			if( AC.dot(AP) < 0.0f )
			{
				d = AC_Normal;
				m_SimplexType = 1;
				m_simplexPoints[1] = m_simplexPoints[2];
				
				m_SupportPoint1[1] = m_SupportPoint1[2];
				m_SupportPoint2[1] = m_SupportPoint2[2];

				return false;
			}
			// region 5. Simplex becomes 0 simplex with point A. Set the new direction which is p - A.
			else 
			{
				m_simplexPoints[0] = m_simplexPoints[2]; 
				
				m_SupportPoint1[0] = m_SupportPoint1[2];
				m_SupportPoint2[0] = m_SupportPoint2[2];

				d = p - m_simplexPoints[0];
				m_SimplexType = 0;
				return false;
			}
		}
		// Inside Triangle
		else
		{
			return true;
		}
		
	}
	else 
	{
		return false;
	}
	
}

typedef Simplex<dfloat> Simplexf;

END_NAMESPACE_DIRAC2D

#endif
