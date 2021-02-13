#include "MeshList.h"
#include "MeshBuilder.h"
#include "LoadTGA.h"
MeshList::MeshList()
{
	meshlist[MESH_AXIS] = MeshBuilder::GenerateAxes("Axis", 1000, 1000, 1000);

	meshlist[MESH_QUAD] = MeshBuilder::GenerateQuad("FloorQuad", Color(1,1,1));
}

MeshList::~MeshList()
{
}

Mesh* MeshList::GetMesh(MESH_TYPE meshtype)
{
	return meshlist[meshtype];
}

