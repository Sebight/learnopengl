#pragma once
class Mesh
{
public:
	std::vector<Vertex> Vertices;
	std::vector<unsigned int> Indices;
	std::vector<Texture> Textures;

	Mesh(std::vector<Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Texture>& textures);

	void Draw(Shader& shader);
private:
	unsigned int m_VAO;
	unsigned int m_VBO;
	unsigned int m_EBO;

	void Setup();
};

