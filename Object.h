#pragma once
#include <vector>
#include <string>
#include <map>
using namespace std;
class ObjectModel
{
public:
	ObjectModel();
	~ObjectModel();

	void Load(const char* fileName);
	vector<float> GetVertexData();
	int GetVertexCount();
	void ClearData();

private:
	struct Position { float x, y, z; };
	struct Color { float r, g, b; };
	struct TexCoords { float s, t; };
	struct Normal { float x, y, z; };

	void LoadMat(const char* fileName);
	bool StartWith(string& line, const char* text);
	void AddVertexData(int vIDx, int nIDx, const char* mtl, vector<Position>& vertices, vector<Normal>& normals);
	map<string, Color> mMaterialMap;
	vector<float> mVertexData;
	void ShowData();
};

