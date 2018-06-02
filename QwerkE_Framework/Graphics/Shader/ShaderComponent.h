#ifndef _ShaderComponent_H_
#define _ShaderComponent_H_

typedef unsigned int GLuint;

#include <string>

class ShaderComponent
{
public:
	ShaderComponent();
	ShaderComponent(const char* shaderPath);
	~ShaderComponent();

	std::string GetName() const { return m_Name; }
	std::string GetType() const { return m_Type; }
	GLuint GetHandle() const { return m_ShaderHandle; }
	const char* GetStringData() const { return m_ShaderString; }

	void SetName(std::string name) { m_Name = name; }
	void SetType(std::string name) { m_Type = name; }
	void SetHandle(GLuint handle) { m_ShaderHandle = handle; }
	void SetStringData(const char* shaderData) { m_ShaderString = shaderData; }

private:
	std::string m_Name = "Uninitialized";
	std::string m_Type = "Uninitialized";

	const char* m_ShaderString = nullptr;
	GLuint m_ShaderHandle = 0;
};

#endif // !_ShaderComponent_H_
