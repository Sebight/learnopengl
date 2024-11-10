#pragma once

class Model
{
public:
	Model() = default;
	Model(std::string& path);
	Model(const char* path);

	void Draw(Shader& shader);
private:
	std::vector<Mesh> m_meshes;
	std::string m_directory;
	std::vector<Texture> m_loadedTextures;

	void LoadModel(std::string& path);
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	uint TextureFromFile(const char* path, const std::string& dir, bool gamma = false );

	std::vector<Texture> LoadMaterialTextures(aiMaterial* material, aiTextureType type, std::string typeName);
};

