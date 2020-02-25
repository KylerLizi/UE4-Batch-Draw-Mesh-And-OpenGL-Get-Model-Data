#pragma once
#include "glad/glad.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "Shader.h"
#include "string"
#include "vector"
#include "MeshInfoCSV.h"
using namespace std;

#define EXPORT_MODEL_DATA

struct Vertex
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texCoords;
};

struct Texture
{
	unsigned int id;
	string type;
	string path;//store the texture's path for comparison with other textures
};

class Mesh
{
public:
	//grid data
	vector<Vertex> vertices;
	vector<unsigned int> indices;
	vector<Texture> textures;

	Mesh(vector<Vertex> vertices, vector<unsigned int> indices, vector<Texture> textures)
	{
		this->vertices = vertices;
		this->indices = indices;
		this->textures = textures;

#ifdef EXPORT_MODEL_DATA
		vector<outData> allInfo;
		outData info;
		for (int i = 0; i < vertices.size(); i++)
		{
			info.verticesPos_X = vertices[i].position.x;
			info.verticesPos_Y = vertices[i].position.y;
			info.verticesPos_Z = vertices[i].position.z;
			info.index = i;
			info.initialIndex = i;
			allInfo.push_back(info);
		}
		MeshInfoCSV exportTxt(allInfo);
		exportTxt.ExportVertices();
		exportTxt.ExportTriangles();
#endif
		setupMesh();
	}

	void Draw(Shader shader)
	{
		//textures in fragment shader naming standard
		/*uniform sampler2D texture_diffuse1;
		uniform sampler2D texture_diffuse2;
		uniform sampler2D texture_specular1;
		uniform sampler2D texture_specular2;*/

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;
		unsigned int normalNr = 1;
		unsigned int heightNr = 1;
		for (unsigned int i=0;i<textures.size();i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);//active texture unit before bind
			//get texture id(N in texture_diffuseN)
			string number;
			string name = textures[i].type;
			if (name == "texture_diffuse")
				number = to_string(diffuseNr++);
			else if (name == "texture_specular")
				number = to_string(specularNr++);
			else if (name == "texture_normal")
				number = to_string(normalNr++);
			else if (name == "texture_height")
				number = to_string(heightNr++);

			shader.setFloat(("material." + name + number).c_str(), i);
			glBindTexture(GL_TEXTURE_2D, textures[i].id);
		}
		glActiveTexture(GL_TEXTURE0);

		//draw grid
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
	}



private:
	//render data
	unsigned int VAO, VBO, EBO;

	void setupMesh()
	{
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		
		//vertex position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
		//vertex normal
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
		//vertex texCoords
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoords));
		
		glBindVertexArray(0);
}
};