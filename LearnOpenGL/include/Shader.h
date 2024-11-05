#pragma once
class Shader
{
public:
	unsigned int Id;

	Shader(const char* vPath, const char* fPath);

	void Use();

	void SetBool(const char* name, bool value) const;
	void SetInt(const char* name, int value) const;
	void SetFloat(const char* name, float value) const;

	void SetMat4f(const char* name, const glm::mat4& value);
	void SetVec3(const char* name, const float x, const float y, const float z);
};
