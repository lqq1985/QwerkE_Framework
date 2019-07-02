#include "../../Graphics/Graphics_Header.h"
#include "../../Graphics/Shader/ShaderProgram.h"
#include "../../Graphics/Shader/ShaderComponent.h"
#include "../../QwerkE_Common/Utilities/StringHelpers.h"
#include "../../QwerkE_Common/Utilities/PrintFunctions.h"
#include "../../QwerkE_Common/Utilities/FileIO/FileUtilities.h"

ShaderProgram* ShaderFactory::CreateShader(eShaderTypes type)
{
	ShaderProgram* shader = new ShaderProgram();
	shader->GetVertShader()->SetStringData(CreateVertexShader(type));
	shader->GetFragShader()->SetStringData(CreateFragmentShader(type));
	shader->GetGeoShader()->SetStringData(CreateGeometryShader(type));

	if (LinkCreatedShaderProgram(shader))
	{
		shader->FindAttributesAndUniforms();
		return shader;
	}
	else
	{
		delete shader;
		return nullptr;
	}
}

ShaderProgram* ShaderFactory::CreateShader(const char* vertFileDir, const char* fragFileDir, const char* geoFileDir)
{
	ShaderProgram* shader = new ShaderProgram();
	if(vertFileDir) CreateShaderComponent(GL_VERTEX_SHADER, vertFileDir);
	if (fragFileDir) CreateShaderComponent(GL_VERTEX_SHADER, fragFileDir);
	if (geoFileDir) CreateShaderComponent(GL_VERTEX_SHADER, geoFileDir);

	if (LinkCreatedShaderProgram(shader))
	{
		shader->FindAttributesAndUniforms();
		return shader;
	}
	else
	{
		delete shader;
		return nullptr;
	}
}
// Returns new shader handle is successful, else 0
ShaderComponent* ShaderFactory::CreateShaderComponent(GLenum shaderType, const char* shaderPath)
{
	const char* shaderString = LoadCompleteFile(shaderPath, 0);
	ShaderComponent* result = GenerateShaderFromData(shaderType, shaderString);
	if (result)
	{
		result->SetType(DeepCopyString(GetFileExtension(shaderPath).c_str()));
		result->SetName(GetFileNameWithExt(shaderPath));
	}
	return result;
}

ShaderComponent* ShaderFactory::GenerateShaderFromData(GLenum shaderType, const char* shaderData)
{
	GLuint shaderHandle = glCreateShader(shaderType); // fails if context is not current for GLFW

	glShaderSource(shaderHandle, 1, &shaderData, NULL);
	glCompileShader(shaderHandle);

	GLint success = 0;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		// Error compiling shader
		GLchar infoLog[512];
		glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);

		const char* shaderTypeString = nullptr;
		if (shaderType == GL_VERTEX_SHADER)
			shaderTypeString = "GL_VERTEX_SHADER";
		else if (shaderType == GL_FRAGMENT_SHADER)
			shaderTypeString = "GL_FRAGMENT_SHADER";
		else if (shaderType == GL_GEOMETRY_SHADER)
			shaderTypeString = "GL_GEOMETRY_SHADER";

		char* next_token = 0;
		char* ShaderName = strtok_s((char*)shaderData, "\n", &next_token);
		// TODO: error reads as garbage characters.
		OutputPrint("\n%s: ShaderFactory: CreateShader(Glenum, const char*) %s compile error-> ", ShaderName, shaderTypeString);
		OutputPrint(infoLog); // OpenGL message

							  // cleanup
		glDeleteShader(shaderHandle);
		shaderHandle = 0;
		return nullptr;
	}
	else
	{
		ShaderComponent* comp = new ShaderComponent();
		comp->SetStringData(shaderData);
		comp->SetHandle(shaderHandle);
		return comp;
	}
}

GLuint ShaderFactory::CreateShaderProgram(GLuint vert, GLuint frag, GLuint geo)
{
	if (vert == 0 && frag == 0 && geo == 0) // no valid shaders
	{
        QwerkE::LogWarning(__FILE__, __LINE__, "");
		return 0;
	}
	return LinkShaders(vert, frag, geo);
}

bool ShaderFactory::LinkCreatedShaderProgram(ShaderProgram* shader)
{
    if (shader)
    {
        if (shader->GetVertShader() || shader->GetFragShader() || shader->GetGeoShader())
        {
            // TODO: Add in geometry shader support
            // GLuint result = LinkShaders(shader->GetVertShader()->GetHandle(), shader->GetFragShader()->GetHandle(), shader->GetGeoShader()->GetHandle());
            GLuint result = LinkShaders(shader->GetVertShader(), shader->GetFragShader(), shader->GetGeoShader());
            if (result != 0)
                shader->SetProgram(result);
            else
                QwerkE::LogError(__FILE__, __LINE__, "Shader link failed for shader program: %s", shader->GetName());

            return result != 0;
        }
        else
        {
            QwerkE::LogError(__FILE__, __LINE__, "Shader link failed because of null ShaderComponent*s for shader: %s", shader->GetName());
        }
    }
    else
    {
        QwerkE::LogError(__FILE__, __LINE__, "Shader link failed because of null ShaderProgram*");
        return false;
    }
}

GLuint ShaderFactory::CreateVertexShader(const char* vertPath)
{
	GLuint shaderHandle = glCreateShader(GL_VERTEX_SHADER); // fails if context is not current for GLFW

	const char* data = LoadCompleteFile(vertPath, 0);
	if (data)
	{
		glShaderSource(shaderHandle, 1, &data, NULL);
		glCompileShader(shaderHandle);

		GLint success = 0;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			// Error compiling shader
			GLchar infoLog[512];
			glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);

			char* next_token = 0;
			char* ShaderName = strtok_s((char*)data, "\n", &next_token);
            // TODO: error reads as garbage characters.
            // QwerkE::LogError(__FILE__, __LINE__, "TODO");
			OutputPrint("\n%s: ShaderFactory: CreateShader(Glenum, const char*) %s compile error-> ", ShaderName, "GL_VERTEX_SHADER");
			OutputPrint(infoLog); // OpenGL message

								  // cleanup
			glDeleteShader(shaderHandle);
			delete[] data;
			return 0;
		}
		else
		{
			delete[] data;
			return shaderHandle;
		}
	}
	else
	{
        QwerkE::LogWarning(__FILE__, __LINE__, "Failed to create vertex shader for file: %s", vertPath);
		return 0;
	}
}

GLuint ShaderFactory::CreateFragmentShader(const char* fragPath)
{
	GLuint shaderHandle = glCreateShader(GL_FRAGMENT_SHADER); // fails if context is not current for GLFW

	const char* data = LoadCompleteFile(fragPath, 0);
	if (data)
	{
		glShaderSource(shaderHandle, 1, &data, NULL);
		glCompileShader(shaderHandle);

		GLint success = 0;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			// Error compiling shader
			GLchar infoLog[512];
			glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);

			char* next_token = 0;
			char* ShaderName = strtok_s((char*)data, "\n", &next_token);
			// TODO: error reads as garbage characters.
			OutputPrint("\n%s: ShaderFactory: CreateShader(Glenum, const char*) %s compile error-> ", ShaderName, "GL_FRAGMENT_SHADER");
			OutputPrint(infoLog); // OpenGL message

								  // cleanup
			glDeleteShader(shaderHandle);
			delete[] data;
			return 0;
		}
		else
		{
			delete[] data;
			return shaderHandle;
		}
	}
	else
	{
        QwerkE::LogWarning(__FILE__, __LINE__, "Failed to create fragment shader for file: %s", fragPath);
		return 0;
	}
}

GLuint ShaderFactory::CreateGeometryShader(const char* geoPath)
{
	GLuint shaderHandle = glCreateShader(GL_GEOMETRY_SHADER); // fails if context is not current for GLFW

	const char* data = LoadCompleteFile(geoPath, 0);
	if (data)
	{
		glShaderSource(shaderHandle, 1, &data, NULL);
		glCompileShader(shaderHandle);

		GLint success = 0;
		glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			// Error compiling shader
			GLchar infoLog[512];
			glGetShaderInfoLog(shaderHandle, 512, NULL, infoLog);

			char* next_token = 0;
			char* ShaderName = strtok_s((char*)data, "\n", &next_token);
			// TODO: error reads as garbage characters.
            // TODO: QwerkE::LogError(...)
			OutputPrint("\n%s: ShaderFactory: CreateShader(Glenum, const char*) %s compile error-> ", ShaderName, "GL_GEOMETRY_SHADER");
			OutputPrint(infoLog); // OpenGL message

								  // cleanup
			glDeleteShader(shaderHandle);
			delete[] data;
			return 0;
		}
		else
		{
			delete[] data;
			return shaderHandle;
		}
	}
	else
	{
        QwerkE::LogWarning(__FILE__, __LINE__, "Failed to create geometry shader for file: %s", geoPath);
		return 0;
	}
}

const char* ShaderFactory::CreateVertexShader(eShaderTypes shaderType)
{
	std::string vertexData;

	SetVertName(shaderType, vertexData);
	SetRenderEngineVersion(vertexData);

	switch (shaderType)
	{
	case eShader_Basic2D:
		vertexData = Basic2DShaderVert(vertexData);
		break;
	case eShader_Basic3D:
		vertexData = Basic3DShaderVert(vertexData);
		break;
	case eShader_LitMaterial:
		vertexData = LitMaterialVert(vertexData);
		break;
    default:
        QwerkE::LogWarning(__FILE__, __LINE__, "ShaderType %i unsupported", (int)shaderType);
        break;
	}

	const char* returnString = DeepCopyString(vertexData.c_str());

    if (returnString == nullptr)
    {
        QwerkE::LogError(__FILE__, __LINE__, "Failed to create vertex shader for type: %i", (int)shaderType);
    }
	return returnString;
}

const char* ShaderFactory::CreateFragmentShader(eShaderTypes shaderType)
{
	std::string fragData;

	SetFragName(shaderType, fragData);
	SetRenderEngineVersion(fragData);

	switch (shaderType)
	{
	case eShader_Basic2D:
		fragData = Basic2DShaderFrag(fragData);
		break;
	case eShader_Basic3D:
		fragData = Basic3DShaderFrag(fragData);
		break;
	case eShader_LitMaterial:
		fragData = LitMaterialFrag(fragData);
        break;
    default:
        QwerkE::LogWarning(__FILE__, __LINE__, "ShaderType %i unsupported", (int)shaderType);
        break;
	}

    const char* returnString = DeepCopyString(fragData.c_str());

    if (returnString == nullptr)
    {
        QwerkE::LogError(__FILE__, __LINE__, "Failed to create fragment shader");
    }
	return returnString;
}

const char* ShaderFactory::CreateGeometryShader(eShaderTypes shaderType)
{
    QwerkE::LogWarning(__FILE__, __LINE__, "CreateGeometryShader() is an incomplete function and was called");
	return 0; // TODO: Build geometry shader support
}
// File name comments
void ShaderFactory::SetShaderFileName(eShaderTypes shaderType, std::string &shaderString, std::string extension)
{
    switch (shaderType) // Add the shader name to a comment for easier debugging
    {
    case eShader_Basic3D:
        shaderString.append("// Basic3D." + extension);
        break;
    case eShader_LitMaterial:
        shaderString.append("// LitMaterial." + extension);
        break;
    default:
        QwerkE::LogWarning(__FILE__, __LINE__, "ShaderType %i unsupported", (int)shaderType);
        break;
    }
}

void ShaderFactory::SetVertName(eShaderTypes shaderType, std::string &shaderString)
{
    SetShaderFileName(shaderType, shaderString, vertex_shader_ext);
}

void ShaderFactory::SetFragName(eShaderTypes shaderType, std::string &shaderString)
{
    SetShaderFileName(shaderType, shaderString, fragment_shader_ext);
}

void ShaderFactory::SetGeoName(eShaderTypes shaderType, std::string &shaderString)
{
    SetShaderFileName(shaderType, shaderString, geometry_shader_ext);
}
// Engine version comment
void ShaderFactory::SetRenderEngineVersion(std::string &shaderString)
{
	// TODO: Get and store the openGL version for reference
    // TODO: Remove OpenGL dependency
	int _OpenGLVersion = 330; // TODO: This should be in some graphics manager or utility function
	std::string _OpenGLprofile = "core"; // TODO: This should be in some graphics manager or utility function
	shaderString.append("\n#version " + std::to_string(_OpenGLVersion) + " " + _OpenGLprofile);
}
// Main function generation
void ShaderFactory::OpenMain(std::string &shaderString)
{
	shaderString.append("\nvoid main()\n{");
}

void ShaderFactory::CloseMain(std::string &shaderString)
{
	shaderString.append("\n}");
}
/* Getters + Setters */
// Getters
// Setters
void ShaderFactory::SetPrefixes(const char* attr, const char* unif, const char* trans)
{
    if (!attr || !unif || !trans)
        QwerkE::LogWarning(__FILE__, __LINE__, "Null value was passed");

	m_AttributePrefix = attr ? DeepCopyString(attr) : m_AttributePrefix;
	m_UniformPrefix = unif ? DeepCopyString(unif) : m_UniformPrefix;
	m_TransferPrefix = trans ? DeepCopyString(trans) : m_TransferPrefix;
}
// private functions
// Returns new program handle if successful, else 0
GLuint ShaderFactory::LinkShaders(const ShaderComponent* vert, const ShaderComponent* frag, const ShaderComponent* geo)
{
    if (!vert && !frag && !geo)
    {
        QwerkE::LogError(__FILE__, __LINE__, "Error linking shaders. No valid shader component");
        return 0;
    }

    GLuint vertHandle = vert == nullptr ? 0 : vert->GetHandle();
    GLuint fragHandle = frag == nullptr ? 0 : frag->GetHandle();
    GLuint geoHandle = geo == nullptr ? 0 : geo->GetHandle();

    if (vertHandle || fragHandle || geoHandle)
        return LinkShaders(vertHandle, fragHandle, geoHandle);
    else
    {
        QwerkE::LogWarning(__FILE__, __LINE__, "Error linking shaders. No valid shader component handles");
        return 0;
    }
}

// Returns new program handle if successful, else 0
GLuint ShaderFactory::LinkShaders(GLuint vert, GLuint frag, GLuint geo)
{
    if (!vert && !frag && !geo)
    {
        QwerkE::LogError(__FILE__, __LINE__, "Error linking shaders. All components %i", 0); // (int)gc_NullHandleValue);
        return 0; // gc_NullHandleValue;
    }

    GLuint result = 0; // gc_NullHandleValue;
	result = glCreateProgram(); // create new program

	// TODO: Handle error when attaching linked shaders. May want to de-link existing shaders before attaching?
	if (vert) glAttachShader(result, vert); // link vertex shader
	if (frag) glAttachShader(result, frag); // link fragment shader
	if (geo) glAttachShader(result, geo); // link geometry shader

	glLinkProgram(result);
	CheckGraphicsErrors(__FILE__, __LINE__);
	// GL_INVALID_VALUE is generated if program is not a value generated by OpenGL.
	// GL_INVALID_OPERATION is generated if program is not a program object.
	// GL_INVALID_OPERATION is generated if program is the currently active program object and transform feedback mode is active.

	GLint success;
	glGetProgramiv(result, GL_LINK_STATUS, &success);
	if (!success)
	{
		GLchar infoLog[512];
		glGetProgramInfoLog(result, 512, NULL, infoLog);
        QwerkE::LogError(__FILE__, __LINE__, "Error linking shaders: %s, %s, %s. Message: %s", vert, frag, geo, infoLog);
        return 0; // gc_NullHandleValue;
	}

	CheckGraphicsErrors(__FILE__, __LINE__);
	return result;
}
/* Utility */
// Structs
void ShaderFactory::AddMaterialStruct(std::string &string)
{
	// TODO: Update material struct
	const char* material_struct =
		"\nstruct Material {"
		"\nvec3 ambient;"
		"\nvec3 diffuse;"
		"\nvec3 specular;"
		"\nfloat shininess;"
		"\n};\n";
	string.append(material_struct);
}
// TODO: Would a light struct help?
void ShaderFactory::AddLightStruct(std::string &string)
{
	const char* light_struct =
		"\nstruct Light {"
		"\nvec3 colour;"
		"\n};\n";
	string.append(light_struct);
}
// attributes
void ShaderFactory::AddAttributeInt(const char* name, std::string &string)
{
	const char* line = "\nin int ";
	string.append(line);
	string.append(m_AttributePrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddAttributeFloat(const char* name, std::string &string)
{
	const char* line = "\nin float ";
	string.append(line);
	string.append(m_AttributePrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddAttributeVec2(const char* name, std::string &string)
{
	const char* line = "\nin vec2 ";
	string.append(line);
	string.append(m_AttributePrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddAttributeVec3(const char* name, std::string &string)
{
	const char* line = "\nin vec3 ";
	string.append(line);
	string.append(m_AttributePrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddAttributeVec4(const char* name, std::string &string)
{
	const char* line = "\nin vec4 ";
	string.append(line);
	string.append(m_AttributePrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddAttributeMat2(const char* name, std::string &string)
{
	const char* line = "\nin mat2 ";
	string.append(line);
	string.append(m_AttributePrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddAttributeMat3(const char* name, std::string &string)
{
	const char* line = "\nin mat3 ";
	string.append(line);
	string.append(m_AttributePrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddAttributeMat4(const char* name, std::string &string)
{
	const char* line = "\nin mat4 ";
	string.append(line);
	string.append(m_AttributePrefix);
	string.append(name);
	string.append(";");
}
// uniforms
void ShaderFactory::AddUniformMaterialStruct(const char* name, std::string &string)
{
	const char* line = "\nuniform Material ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddUniformInt(const char* name, std::string &string)
{
	const char* line = "\nuniform int ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddUniformFloat(const char* name, std::string &string)
{
	const char* line = "\nuniform float ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddUniformVec2(const char* name, std::string &string)
{
	const char* line = "\nuniform vec2 ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddUniformVec3(const char* name, std::string &string)
{
	const char* line = "\nuniform vec3 ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddUniformVec4(const char* name, std::string &string)
{
	const char* line = "\nuniform vec4 ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddUniformMat2(const char* name, std::string &string)
{
	const char* line = "\nuniform mat2 ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddUniformMat3(const char* name, std::string &string)
{
	const char* line = "\nuniform mat3 ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddUniformMat4(const char* name, std::string &string)
{
	const char* line = "\nuniform mat4 ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddUniformSampler2D(const char* name, std::string string)
{
	const char* line = "\nuniform sampler2D ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddUniformTexture(int* count, std::string string)
{
	const char* line = "\nuniform sampler2D ";
	string.append(line);
	string.append(m_UniformPrefix);
	string.append("Texture");
	string.append(std::to_string(*count));
	*count++; // increment
	string.append(";");
}
// input
void ShaderFactory::AddInputInt(const char* name, std::string &string)
{
	const char* line = "\nin int ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddInputFloat(const char* name, std::string &string)
{
	const char* line = "\nin float ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddInputVec2(const char* name, std::string &string)
{
	const char* line = "\nin vec2 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddInputVec3(const char* name, std::string &string)
{
	const char* line = "\nin vec3 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddInputVec4(const char* name, std::string &string)
{
	const char* line = "\nin vec4 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddInputMat2(const char* name, std::string &string)
{
	const char* line = "\nin mat2 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddInputMat3(const char* name, std::string &string)
{
	const char* line = "\nin mat3 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddInputMat4(const char* name, std::string &string)
{
	const char* line = "\nin mat4 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}
// output
void ShaderFactory::AddOutputInt(const char* name, std::string &string)
{
	const char* line = "\nout int ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddOutputFloat(const char* name, std::string &string)
{
	const char* line = "\nout float ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddOutputVec2(const char* name, std::string &string)
{
	const char* line = "\nout vec2 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddOutputVec3(const char* name, std::string &string)
{
	const char* line = "\nout vec3 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddOutputVec4(const char* name, std::string &string)
{
	const char* line = "\nout vec4 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddOutputMat2(const char* name, std::string &string)
{
	const char* line = "\nout mat2 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddOutputMat3(const char* name, std::string &string)
{
	const char* line = "\nout mat3 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}

void ShaderFactory::AddOutputMat4(const char* name, std::string &string)
{
	const char* line = "\nout mat4 ";
	string.append(line);
	string.append(m_TransferPrefix);
	string.append(name);
	string.append(";");
}