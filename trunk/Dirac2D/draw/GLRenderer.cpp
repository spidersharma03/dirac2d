/*
 *  Renderer.cpp
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "GLRenderer.h"
#include "../geometry/CollisionShape.h"
#include "../geometry/RegularPolygon.h"

BEGIN_NAMESPACE_DIRAC2D


void GLRenderer::drawShape(CollisionShape* shape)
{
	switch ( shape->getShapeType() )
	{
		case EST_BOX:
			break;
		case EST_REGULARPOLY:
			drawPolygon( ((RegularPolygon*)shape)->getVertices() , ((RegularPolygon*)shape)->getNumVertices() );
			break;
		default:
			break;
	}
}

void GLRenderer::drawCircle( dfloat radius )
{
	
}

void GLRenderer::drawBox( dfloat width, dfloat height)
{
	dfloat w = width*0.5f; dfloat h = height*0.5f;
	glBegin(GL_QUADS);
	glVertex2f(w, h);
	glVertex2f(-w, h);
	glVertex2f(-w, -h);
	glVertex2f(w, -h);	
	glEnd();
}

void GLRenderer::drawPolygon( Vector2f* vertices, dint32 numVertices )
{
	glBegin(GL_POLYGON);
	for( dint32 i=0; i<numVertices; i++ )
		glVertex2f(vertices[i].x, vertices[i].y);
	glEnd();
}

void GLRenderer::drawLine( Vector2f& begin, Vector2f& end)
{
	glBegin(GL_LINES);
	glVertex2f(begin.x, begin.y);
	glVertex2f(end.x, end.y);
	glEnd();
}

void GLRenderer::drawPoint( Vector2f& point )
{
	glBegin(GL_POINTS);
	glVertex2f(point.x, point.y);
	glEnd();
}

void GLRenderer::drawAABB( AABB2f& aabb )
{
	glRectf(aabb.m_LowerBounds.x, aabb.m_LowerBounds.y, aabb.m_UpperBounds.x, aabb.m_UpperBounds.y);
}

void GLRenderer::setTransform( Matrix3f& xform)
{
	xform.getPointer(data);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glLoadMatrixf(data);
}

void GLRenderer::setColor( dchar r, dchar g, dchar b, dchar a )
{
	glColor4ub(r, g, b, a);
}

void GLRenderer::setColor( dchar r, dchar g, dchar b )
{
	glColor3ub(r, g, b);
}

void GLRenderer::setPointSize(dfloat size)
{
	glPointSize(size);
}

END_NAMESPACE_DIRAC2D