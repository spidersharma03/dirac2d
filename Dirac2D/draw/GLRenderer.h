/*
 *  GLRenderer.h
 *  Dirac2D
 *
 *  Created by Prashant on 03/05/12.
 *
 */

#include "Renderer.h"

#ifndef _GL_RENDERER_H_
#define _GL_RENDERER_H_

#ifdef WIN32
  #include <GL/glut.h>
#else
  #include <OpenGL/OpenGL.h>
#endif

BEGIN_NAMESPACE_DIRAC2D

class GLRenderer : public Renderer
{
public:
	GLRenderer(PhysicalWorld* world):Renderer(world)
	{
	}
	
	virtual void setPointSize(dfloat size);

	virtual void setColor( dchar r, dchar g, dchar b, dchar a );
	
	virtual void setColor( dchar r, dchar g, dchar b );
	
	virtual void drawShape(CollisionShape*);
	
	virtual void drawCircle( dfloat radius );
	
	virtual void drawBox( dfloat width, dfloat height);
	
	virtual void drawPolygon( Vector2f* vertices, dint32 numVertices );
	
	virtual void drawLine( Vector2f& begin, Vector2f& end);
	
	virtual void drawPoint( Vector2f& point );
	
	virtual void setTransform( Matrix3f& xform);

protected:
	dfloat data[16];
};

END_NAMESPACE_DIRAC2D

#endif