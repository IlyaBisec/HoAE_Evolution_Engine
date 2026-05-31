#include <windows.h>
#include "comms\comms.h"
#include "Maya\include\MStatus.h"
#include "Maya\include\MLibrary.h"
#include "Maya\include\MFnMesh.h"
#include "Maya\include\MPointArray.h"
#include "Maya\include\MIntArray.h"
#include "Maya\include\MObject.h"
#include "Maya\include\MStatus.h"
#include "Maya\include\MString.h"
#include "Maya\include\MLibrary.h"
#include "Maya\include\MGlobal.h"
#include "Maya\include\MSelectionList.h"
#include "Maya\include\MItSelectionList.h"
#include "Maya\include\MDagPath.h"
#include "Maya\include\MItMeshVertex.h"

#include "Convert.cpp"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	remove("C.obj");

	// CurDir:
	char CurDir[MAX_PATH];
	ZeroMemory(CurDir, sizeof(CurDir));
	GetCurrentDirectory(MAX_PATH, CurDir);

	// Init Maya:
	MStatus ms = MLibrary::initialize("Booleans");
	if(!ms) {
		MessageBox(NULL, "MLibrary::initialize() FAILED!", "--- ERROR ---", 0);
		return -1;
	}

	// Restoring CurDir:
	SetCurrentDirectory(CurDir);

	cRawMesh RawMeshA, RawMeshB;
	cStr Log;
	if(!RawMeshA.LoadObj("A.obj", &Log)) {
		MessageBox(NULL, Log, "LoadObj() FAILED!", 0);
		char CurDir[MAX_PATH];
		GetCurrentDirectory(4096, CurDir);
		MessageBox(NULL, CurDir, "--- CurDir ---", MB_OK);
		return 0;
	}
	RawMeshA.Type(comms::RMTTTris);
	if(!RawMeshB.LoadObj("B.obj", &Log)) {
		MessageBox(NULL, Log, "LoadObj() FAILED!", 0);
		return 0;
	}
	RawMeshB.Type(comms::RMTTTris);

	// Maya:
	// A:
	MObject newTransformA;
	MFnMesh fnPolyA;
	assert(Convert(RawMeshA, newTransformA, fnPolyA));
	assert(fnPolyA.numVertices() == RawMeshA.m_Verts.Count());
	assert(fnPolyA.numPolygons() == RawMeshA.m_Tris.Count() / 3);
	// CleanUp:
	assert(MGlobal::selectByName(fnPolyA.name(), MGlobal::kReplaceList) == MS::kSuccess);
	MString CleanUpCommand("polyCleanupArgList 3 { \"0\",\"1\",\"1\",\"0\",\"0\",\"0\",\"0\",\"0\",\"0\",\"1e-005\",\"0\",\"1e-005\",\"0\",\"1e-005\",\"0\",\"2\",\"0\" };");
	//assert(MS::kSuccess == MGlobal::executeCommand(CleanUpCommand));
	MGlobal::executeCommand(CleanUpCommand);
	// B:
	MObject newTransformB;
	MFnMesh fnPolyB;
	assert(Convert(RawMeshB, newTransformB, fnPolyB));
	assert(fnPolyB.numVertices() == RawMeshB.m_Verts.Count());
	assert(fnPolyB.numPolygons() == RawMeshB.m_Tris.Count() / 3);
	// Selecting operands:
	assert(MGlobal::selectByName(fnPolyA.name(), MGlobal::kReplaceList) == MS::kSuccess);
	assert(MGlobal::selectByName(fnPolyB.name(), MGlobal::kAddToList) == MS::kSuccess);
	// executing polyBoolOp:
	MString BoolCommand("polyBoolOp -op 1 ");
	BoolCommand += fnPolyB.name();
	BoolCommand += " ";
	BoolCommand += fnPolyA.name();
	assert(MS::kSuccess == MGlobal::executeCommand(BoolCommand));
	// Result:
	MString ResultName("polySurface3");
	// polyTriangulate:
	MString TriCommand("polyTriangulate ");
	TriCommand += ResultName;
	assert(MS::kSuccess == MGlobal::executeCommand(TriCommand));
	// Selecting result (object mode):
	assert(MS::kSuccess == MGlobal::selectByName(ResultName, MGlobal::kReplaceList));

	// Extracting result:
	MSelectionList slist;
	assert(MGlobal::getActiveSelectionList(slist) == MS::kSuccess);
	MItSelectionList iter(slist);
	assert(!iter.isDone());

	MDagPath objectPath;
	MObject component;
	MObject mesh;
	MStatus ReturnStatus;
	cRawMesh RawMeshC;
	for(; !iter.isDone(); iter.next()) {
		ReturnStatus = iter.getDagPath(objectPath, component);
		assert(MS::kSuccess == ReturnStatus);
		if(objectPath.hasFn(MFn::kMesh)) {
			assert(MS::kSuccess == objectPath.extendToShape());
			mesh = objectPath.node(&ReturnStatus);
			assert(MS::kSuccess == ReturnStatus);
			MFnMesh C(mesh, &ReturnStatus);
			assert(MS::kSuccess == ReturnStatus);
			if(Convert(C, &RawMeshC)) {
				RawMeshC.SaveObj("C.obj");
			}
		}
	}
	return 0;
}
