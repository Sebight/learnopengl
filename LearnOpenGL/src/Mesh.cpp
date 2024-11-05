#include "common.h"
#include "Texture.h"
#include "Vertex.h"
#include "Shader.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures)
{
	Vertices = vertices;
	Indices = indices;
	Textures = textures;

	Setup();
}

void Mesh::Draw(Shader& shader)
{
	uint diffuseN = 1;
	uint specularN = 1;

	for (uint i = 0; i < Textures.size(); i++) {
		glActiveTexture(GL_TEXTURE0 + i);

		std::string number;
		std::string name = Textures[i].Type;

		if (name == "texture_diffuse") {
			number = std::to_string(diffuseN++);
		}
		else if (name == "texture_specular") {
			number = std::to_string(specularN++);
		}

		shader.SetFloat(("material." + name + number).c_str(), static_cast<float>(i));
		glBindTexture(GL_TEXTURE_2D, Textures[i].Id);
	}
	glActiveTexture(GL_TEXTURE0);

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(Indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Mesh::Setup()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(Vertex), &Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(uint), &Indices[0], GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

	glBindVertexArray(0);
}