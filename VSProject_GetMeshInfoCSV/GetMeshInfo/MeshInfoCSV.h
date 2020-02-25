#pragma once
#include "vector"
#include "stdlib.h"
#include "algorithm"
using namespace std;

struct outData {
	float verticesPos_X;
	float verticesPos_Y;
	float verticesPos_Z;
	unsigned int index;
	unsigned int initialIndex;
};

//sort by smaller position
bool cmpByPosition(outData a, outData b)
{
	if (a.verticesPos_X != b.verticesPos_X)
		return a.verticesPos_X < b.verticesPos_X;
	else if (a.verticesPos_Y != b.verticesPos_Y)
		return a.verticesPos_Y < b.verticesPos_Y;
	else
		return a.verticesPos_Z < b.verticesPos_Z;
}

class MeshInfoCSV {

public:
	vector<outData> allInfo;
	vector<unsigned int> indices;

	MeshInfoCSV(vector<outData> allInfo)
	{
		this->allInfo = allInfo;
		//initial indices
		for (int i = 0; i < allInfo.size(); i++)
			this->indices.push_back(i);
		OptimizeData();
	}

	//remove duplicate vertex and modify indices
	void OptimizeData()
	{
		//sort by position's value for remove the duplicate index
		sort(allInfo.begin(), allInfo.end(), cmpByPosition);

		//compare with previous data
		//将重复的顶点去掉，更新面片的索引下标为不重复的顶点下标(内存优化)
		if (allInfo.size() > 1)
		{
			int newIndex = 0;
			allInfo[0].index = newIndex;
			indices[allInfo[0].initialIndex] = newIndex;
			int len = allInfo.size();
			for (int i = 0; i < len - 1; i++)
			{
				if (allInfo[newIndex].verticesPos_X == allInfo[newIndex + 1].verticesPos_X 
					&& allInfo[newIndex].verticesPos_Y == allInfo[newIndex + 1].verticesPos_Y
					&& allInfo[newIndex].verticesPos_Z == allInfo[newIndex + 1].verticesPos_Z)
				{//update repetitive vertex's indexa
					allInfo[newIndex + 1].index = newIndex;
					indices[allInfo[newIndex + 1].initialIndex] = newIndex;
					allInfo.erase(allInfo.begin() + newIndex + 1);
				}
				else
				{
					newIndex++;
					indices[allInfo[newIndex].initialIndex] = newIndex;
					allInfo[newIndex].index = newIndex;
				}
			}
		}
	}

	void ExportVertices()
	{	
		int i;
		FILE *fp;
		//please keep close excel when print to file
		fopen_s(&fp,"MeshData/OpenglVertices.csv", "w+");//model has one mesh
		//fopen_s(&fp,"MeshData/OpenglVertices.csv", "a+");//TODO:model has multiple mesh
		fprintf(fp, ",MeshID,Vertice_X,Vertice_Y,Vertice_Z\n");
		for (i = 0; i < allInfo.size(); i++)
		{
			fprintf(fp, "%d,1,%.2f,%.2f,%.2f\n", i+1,allInfo[i].verticesPos_X, allInfo[i].verticesPos_Y, allInfo[i].verticesPos_Z);
		}
		fprintf(fp, "%d,0,0,0,0\n",i+1);
		fclose(fp);
		cout << "Vertices successfully print to csv excel!" << endl;
	}
	
	void ExportTriangles()
	{
		int i;
		FILE *fp;
		fopen_s(&fp, "MeshData/OpenglTriangles.csv", "w+");//model has one mesh
		//fopen_s(&fp, "MeshData/OpenglTriangles.csv", "a+");//TODO:model has multiple mesh
		fprintf(fp, ",MeshID,Vertice1,Vertice2,Vertice3\n");
		for (i = 0; i < indices.size(); i+=3)
		{
			//Vertex order must be clockwise in ue4 draw
			fprintf(fp, "%d,1,%d,%d,%d\n", i/3 + 1, indices[i], indices[i+2], indices[i+1]);
		}
		fprintf(fp, "%d,0,0,0,0\n", i/3 + 1);
		fclose(fp);
		cout << "Triangles successfully print to csv excel!" << endl;
	}
};