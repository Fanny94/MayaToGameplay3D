#pragma once
#include "Linker.h"

enum MessageType { MayaMesh, MayaCamera, MayaMaterial, MayaNodeDelete };

struct vertices
{
	float pos[3];
	float nor[3];
	float uv[2];
};

struct HeaderType
{
	char messageType[256];
	size_t vertexArray;
	size_t vertexCount;
};

struct CameraHeader
{
	char messageType[256];
	bool isPerspective;
	float projectionMatrix[16];
	float translation[3];
	float rotation[4];
	float nearPlane;
	float farPlane;
	float fieldOfView;

};

char* message;
CircBufferFixed *circPtr;
size_t offset = 0;
MCallbackIdArray idList;
MStatus res = MS::kSuccess;

MFloatPointArray pts;
std::vector<vertices> points;

MIntArray triangleCounts;
MIntArray triangleVertexIDs;
MVector vertexNormal;
MIntArray normalList, normalCount;
MFloatArray u, v;
MFloatVectorArray normals;

MIntArray triangleCountsOffsets;
MIntArray triangleIndices;

unsigned int i;

CameraHeader camHeader;
int currentView;
M3dView camView;
MDagPath camera;

MFloatMatrix projMatrix;
MObject parent;
double rotValues[4];

