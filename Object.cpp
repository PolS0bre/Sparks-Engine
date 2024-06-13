#include "Object.h"
#include <iostream>
#include <fstream>


ObjectModel::ObjectModel()
{
}

ObjectModel::~ObjectModel()
{
}

void ObjectModel::Load(const char* fileName)
{
	string path = "Assets/OBJ/";
	path += fileName;
	vector<Position> vertices;
	vector<Normal> normals;
	vector<TexCoords> tCoords;

	ifstream file(path);
	if (file) {
		char currentMtlName[50];
		string line;
		while (getline(file, line)) {
			if (StartWith(line, "mtllib ")) {
				char mtlFileName[50];
				(void)sscanf_s(line.c_str(), "mtllib %s", mtlFileName, sizeof(mtlFileName));
				LoadMat(mtlFileName);
			}
			if (StartWith(line, "v ")) {
				Position pos;
				sscanf_s(line.c_str(), "v %f %f %f", &pos.x, &pos.y, &pos.z);
				vertices.push_back(pos);
			}
			if (StartWith(line, "vt ")) {
				TexCoords t;
				sscanf_s(line.c_str(), "vt %f %f", &t.s, &t.t);
				tCoords.push_back(t);
			}
			if (StartWith(line, "vn ")) {
				Normal n;
				sscanf_s(line.c_str(), "vn %f %f %f", &n.x, &n.y, &n.z);
				normals.push_back(n);
			}
			if (StartWith(line, "usemtl ")) {
				(void)sscanf_s(line.c_str(), "usemtl %s", currentMtlName , sizeof(currentMtlName));
			}
			if (StartWith(line, "f ")) {
				int v1, v2, v3;
				int n1, n2, n3;
				int t1, t2, t3;
					
				if (fileName != "Amiga-Days_MarioStar.obj") {
					(void)sscanf_s(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &t1, &n1, &v2, &t2, &n2, &v3, &t3, &n3);
				}
				else {
					(void)sscanf_s(line.c_str(), "f %d//%d %d//%d %d//%d", &v1, &n1, &v2, &n2, &v3, &n3);
				}

				AddVertexData(v1, n1, currentMtlName, vertices, normals);
				AddVertexData(v2, n2, currentMtlName, vertices, normals);
				AddVertexData(v3, n3, currentMtlName, vertices, normals);
			}
		}
	}
	else {
		cout << "OBJ file loading failed" << endl;
	}
	//ShowData();
}

vector<float> ObjectModel::GetVertexData()
{
	return mVertexData;
}

int ObjectModel::GetVertexCount()
{
	return mVertexData.size() / 9;
}

void ObjectModel::LoadMat(const char* fileName)
{
	string path = "Assets/OBJ/";
	path += fileName;
	ifstream file(path);
	if (file) {
		string line;
		while (getline(file, line))
		{
			char mtlName[25];
			if (StartWith(line, "newmtl ")) {
				(void)sscanf_s(line.c_str(), "newmtl %s", mtlName, sizeof(mtlName));
				mMaterialMap[mtlName] = Color();
			}
			if (StartWith(line, "Kd ")) {
				Color& color = mMaterialMap[mtlName];
				sscanf_s(line.c_str(), "Kd %f %f %f", &color.r, &color.g, &color.b);
			}
			/*if (StartWith(line, "map_Kd ")) {

			}
			*/

		}
	}
	else {
		cout << "Material file loading failed" << endl;
	}
}

bool ObjectModel::StartWith(string& line, const char* text)
{
	size_t textLen = strlen(text);
	if (line.size() < textLen) {
		return false;
	}
	for (size_t i = 0; i < textLen; i++) {
		if (line[i] == text[i])continue;
		else return false;
	}
	return true;
}

void ObjectModel::AddVertexData(int vIDx, int nIDx, const char* mtl, vector<Position>& vertices, vector<Normal>& normals)
{
	Color c = mMaterialMap[mtl];
	Position p = vertices[vIDx - 1];
	Normal n = normals[nIDx - 1];

	mVertexData.push_back(p.x);
	mVertexData.push_back(p.y);
	mVertexData.push_back(p.z);

	mVertexData.push_back(c.r);
	mVertexData.push_back(c.g);
	mVertexData.push_back(c.b);

	mVertexData.push_back(n.x);
	mVertexData.push_back(n.y);
	mVertexData.push_back(n.z);
}

void ObjectModel::ShowData()
{
	cout << "Vertex Data:" << endl;
	for (int i = 0; i < mVertexData.size(); i++) {
		cout << mVertexData[i] << endl;
	}
	cout << "Finished Vertex Data";
}

void ObjectModel::ClearData()
{
	mVertexData.clear();
	mMaterialMap.clear();
}

