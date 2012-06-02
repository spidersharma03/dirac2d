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
#include "../geometry/Circle.h"
#include "../geometry/Capsule.h"
#include "../geometry/Edge.h"
#include "../geometry/EdgeChain.h"

BEGIN_NAMESPACE_DIRAC2D


void GLRenderer::drawShape(CollisionShape* shape)
{
	switch ( shape->getShapeType() )
	{
		case EST_BOX:
			break;
		case EST_EDGE:
			drawLine( ((Edge*)shape)->m_Vertex1, ((Edge*)shape)->m_Vertex2);
			break;
		case EST_EDGE_CHAIN:
		{
			EdgeChain* edgeChain = ((EdgeChain*)shape);
			for( dint32 e=0; e<edgeChain->getNumEdges(); e++ )
			{
				Edge* edge = edgeChain->getEdge(e);
				drawLine( edge->m_Vertex1, edge->m_Vertex2);
			}
			break;
		}
		case EST_CIRCLE:
			drawCircle( shape->m_Centroid.x, shape->m_Centroid.y, ((Circle*)shape)->getRadius());
			break;
		case EST_CAPSULE:
			drawCapsule( shape->m_Centroid.x, shape->m_Centroid.y, ((Capsule*)shape)->getRadius() , ((Capsule*)shape)->getHeight() );
			break;
		case EST_REGULARPOLY:
			drawPolygon( ((RegularPolygon*)shape)->getVertices() , ((RegularPolygon*)shape)->getNumVertices() );
			break;
		default:
			break;
	}
}

void GLRenderer::drawCircle( dfloat cx, dfloat cy, dfloat radius )
{
	glTranslatef(cx, cy, 0.0f);
	glBegin(GL_LINE_LOOP);
	dfloat angle = 0.0f;
	dfloat dAngle = 2.0f*PI/20;
	for( dint32 v=0; v<20; v++ )
	{
		dfloat x = radius * cos(angle);
		dfloat y = radius * sin(angle);
		glVertex2f(x, y);
		angle += dAngle;
	}
	glEnd();
	
	glBegin(GL_LINES);
	glVertex2f(0.0f, 0.0f);
	glVertex2f(radius, 0.0f);
	glEnd();
}

void GLRenderer::drawCapsule( dfloat cx, dfloat cy, dfloat radius, dfloat height)
{
	// Draw Capsule Box
	glTranslatef(cx, cy, 0.0f);
	glBegin(GL_LINES);
		glVertex2f(-height*0.5f, radius);
		glVertex2f( height*0.5f, radius);
		glVertex2f(-height*0.5f, -radius);
		glVertex2f( height*0.5f, -radius);
	glEnd();
	// Draw Capsule Caps.
	glBegin(GL_LINE_LOOP);
	dfloat angle = -PI_2;
	dfloat dAngle = PI/10;
	for( dint32 v=0; v<11; v++ )
	{
		dfloat x = radius * cos(angle) + height*0.5f;
		dfloat y = radius * sin(angle);
		glVertex2f(x, y);
		angle += dAngle;
	}
	glEnd();
	
	glBegin(GL_LINE_LOOP);
	angle = -3*PI_2;
	dAngle = PI/10;
	for( dint32 v=0; v<11; v++ )
	{
		dfloat x = radius * cos(angle) - height*0.5f;
		dfloat y = radius * sin(angle);
		glVertex2f(x, y);
		angle += dAngle;
	}
	glEnd();
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