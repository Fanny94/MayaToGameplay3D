#include <Windows.h>
#include "Linker.h"
#include "CircBuffer.h"
#include "PluginDeclarations.h"

//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref__abc_export_2_maya_mesh_writer_8cpp_example_html
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_mesh_op_cmd_2mesh_op_fty_action_8cpp_example_html
//http://help.autodesk.com/view/MAYAUL/2016/ENU/?guid=__cpp_ref_obj_export_2obj_export_8cpp_example_html


void GetMeshes(MFnMesh &mesh)
{

	MGlobal::displayInfo("current mesh: " + mesh.name());

	mesh.getPoints(pts, MSpace::kObject);
	mesh.getUVs(u, v, 0);

	mesh.getTriangleOffsets(triangleCountsOffsets, triangleIndices);
	mesh.getTriangles(triangleCounts, triangleVertexIDs);
	mesh.getNormals(normals, MSpace::kObject);

	points.resize(triangleVertexIDs.length());
	
	mesh.getNormalIds(normalCount, normalList);
	

	for (i = 0; i < triangleVertexIDs.length(); i++) {

		points.at(i).pos[0] = pts[triangleVertexIDs[i]].x;
		points.at(i).pos[1] = pts[triangleVertexIDs[i]].y;
		points.at(i).pos[2] = pts[triangleVertexIDs[i]].z;

		points.at(i).nor[0] = normals[triangleVertexIDs[i]].x;
		points.at(i).nor[1] = normals[triangleVertexIDs[i]].y;
		points.at(i).nor[2] = normals[triangleVertexIDs[i]].z;

		points.at(i).uv[0] = u[triangleVertexIDs[triangleIndices[i]]];
		points.at(i).uv[1] = v[triangleVertexIDs[triangleIndices[i]]];

		MString index= "";
		index += i;

		MString vx = "";
		MString vy = "";
		MString vz = "";

		vx += points.at(i).pos[0];
		vy += points.at(i).pos[1];
		vz += points.at(i).pos[2];

		MString nx = "";
		MString ny = "";
		MString nz = "";

		nx += points.at(i).nor[0];
		ny += points.at(i).nor[1];
		nz += points.at(i).nor[2];

		MString u = "";
		MString v = "";
		u += points.at(i).uv[0];
		v += points.at(i).uv[1];

		MGlobal::displayInfo("VERTEX " + index + ": " + vx + " " + vy + " " + vz + "\n" +
			"NORMAL " + index + ": " + nx + " " + ny + " " + nz + "\n" +
			"UV " + index + ": " + u + " " + v);

	}



	char* message = new char[5000];
	HeaderType header{"mesh", points.size() * sizeof(vertices), points.size()};

	memcpy(message, &header, sizeof(HeaderType));
	memcpy(message + sizeof(HeaderType), points.data(), sizeof(vertices) * points.size());

	CircBufferFixed *circPtr = new CircBufferFixed(L"buff", true, 1 << 20, 256);
	circPtr->push(message, sizeof(HeaderType) + (sizeof(vertices) * points.size()   ));

}

void GetCamera(MFnCamera &camera)
{	
	MGlobal::displayInfo("current camera: " + camera.name());
}

void MNodeFunction(MObject &node, void* clientData)
{
	if (node.hasFn(MFn::kMesh)) {
		MFnMesh mesh(node);
		if (mesh.canBeWritten()) {
			GetMeshes(mesh);
			}
		}
}

//void timeElapsedFunction(float elapsedTime, float lastTime, void *clientData)
//{
//	MStatus res = MS::kSuccess;
//
//	
//}

EXPORT MStatus initializePlugin(MObject obj)
{

	MFnPlugin myPlugin(obj, "Maya Plugin", "1.0", "Any", &res);
	if (MFAIL(res)) {
		CHECK_MSTATUS(res);
	}

	MItDag itMesh(MItDag::kBreadthFirst, MFn::kMesh);
	while (itMesh.isDone() == false)
	{
		MFnMesh mesh(itMesh.currentItem());
		GetMeshes(mesh);

		itMesh.next();
	}

	//MItDag itCamera(MItDag::kBreadthFirst, MFn::kCamera);
	//while (itCamera.isDone() == false)
	//{
	//	MFnCamera camera(itCamera.currentItem());

	//	GetCamera(camera);

	//	itCamera.next();
	//}

	//MCallbackId timeId = MTimerMessage::addTimerCallback(5, timeElapsedFunction, NULL, &res);
	//
	//if (res == MS::kSuccess) {
	//	idList.append(timeId);
	//	MGlobal::displayInfo("time callback Succeeded");
	//}
	//else {
	//	MGlobal::displayInfo("time callback Failed");
	//}

	MCallbackId nodeId = MDGMessage::addNodeAddedCallback(MNodeFunction, kDefaultNodeType, NULL, &res);

	if (res == MS::kSuccess) {
		idList.append(nodeId);
		MGlobal::displayInfo("node callback Succeeded");
	}
	else {
		MGlobal::displayInfo("node callback Failed");
	}

	MGlobal::displayInfo("Maya plugin loaded!");

	return res;
}

EXPORT MStatus uninitializePlugin(MObject obj)
{
	MFnPlugin plugin(obj);

	MGlobal::displayInfo("Maya plugin unloaded!");

	MMessage::removeCallbacks(idList);

	return MS::kSuccess;
}
