#ifndef _iDebugRenderer_HG_
#define _iDebugRenderer_HG_

// This is the interface that EVERYTHING can use.
// Adds the ability to add debug items to draw

#include <glm/vec3.hpp>

class iDebugRenderer
{
public:
	virtual ~iDebugRenderer() {};

	struct sDebugTri
	{
		sDebugTri();
		sDebugTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 colour, bool bPersist=false);
		sDebugTri(glm::vec3 v[3], glm::vec3 colour, bool bPersist=false);
		glm::vec3 v[3];		glm::vec3 colour;
		bool bPersist;	
		bool bIgnorDepthBuffer;
	};

	struct sDebugLine
	{
		sDebugLine();
		sDebugLine(glm::vec3 start, glm::vec3 end, glm::vec3 colour, bool bPersist=false);
		sDebugLine(glm::vec3 points[2], glm::vec3 colour, bool bPersist=false);
		glm::vec3 points[2];		glm::vec3 colour;
		bool bPersist;
		bool bIgnorDepthBuffer;
	};

	struct sDebugPoint
	{
		sDebugPoint();
		sDebugPoint(glm::vec3 xyz, glm::vec3 colour, bool bPersist=false);
		sDebugPoint(glm::vec3 xyz, glm::vec3 colour, float pointSize, bool bPersist=false);
		glm::vec3 xyz;		glm::vec3 colour;	float pointSize;
		bool bPersist;
		bool bIgnorDepthBuffer;
	};

	virtual void addTriangle(glm::vec3 v1XYZ, glm::vec3 v2XYZ, glm::vec3 v3XYZ, glm::vec3 colour, bool bPersist = false) = 0;
	virtual void addTriangle(sDebugTri &tri) = 0;
	virtual void addLine(glm::vec3 startXYZ, glm::vec3 endXYZ, glm::vec3 colour, bool bPersist = false) = 0;
	virtual void addLine(sDebugLine &line) = 0;
	virtual void addPoint(glm::vec3 xyz, glm::vec3 colour, bool bPersist = false) = 0;
	virtual void addPoint(sDebugPoint &point) = 0;
	virtual void addPoint(glm::vec3 xyz, glm::vec3 colour, float pointSize, bool bPersist = false) = 0;
};

typedef iDebugRenderer::sDebugTri	drTri;
typedef iDebugRenderer::sDebugLine	drLine;
typedef iDebugRenderer::sDebugPoint drPoint;


#endif
