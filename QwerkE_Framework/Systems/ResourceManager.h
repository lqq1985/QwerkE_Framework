#ifndef _ResourceManager_H_
#define _ResourceManager_H_

#include "../../QwerkE_Common/Libraries/glew/GL/glew.h"
#include "../../QwerkE_Common/Libraries/freetype2/freetype/freetype.h" // TODO: create a font class?

#include <string>
#include <map>

class Mesh;
class ShaderProgram;
class Model;
struct MaterialData;

// For instances where 1 asset may be shared between objects in
// possibly many scenes, the ResourceManager() may want to count
// the number of instances of an asset and only off load any
// memory when all objects are finished with that asset.

struct AssetInfo
{
	AssetInfo(std::string name, std::string directory) { m_ID = name, m_Directory = directory; }
	std::string m_ID = "Uninitialized";
	std::string m_Directory = "Uninitialized";
	short m_References = 0; // When not 0, it is in RAM
};

class ResourceManager
{
public:
    ResourceManager();
    virtual ~ResourceManager();
	void Init();

	// getters
	Mesh* GetMesh(const char* name); // specify .ext
	ShaderProgram* GetShader(const char* name); // specify .ext
	GLuint GetTexture(const char* name); // specify .ext
	MaterialData* GetMaterial(const char* name); // specify .ext
	Model* GetModel(const char* name); // specify .ext
	FT_Face GetFont(const char* name); // specify .ext

	// TODO: Handle loading additional resources
	// CubeMap* GetCubeMap(const char* name); // specify .ext
	// FBO* GetFBO(const char* name); // specify .ext
	// Sound* GetSound(const char* name); // specify .ext

	// Load directory
	// void LoadDirectory(const char* folder);

	// Cleanup
	void DeleteAllResources();

private:
	// resource storage
	std::map<std::string, Mesh*> m_Meshes; // TODO: Rename
	std::map<std::string, ShaderProgram*> m_Shaders;
	std::map<std::string, GLuint> m_Textures;
	std::map<std::string, MaterialData*> m_Materials;
    std::map<std::string, Model*> m_Models;
	std::map<std::string, FT_Face> m_Fonts;

	// Utilities
	bool isUnique(Mesh* mesh);
	bool isUnique(ShaderProgram* shader);
	bool isUnique(GLuint texturehandle);
	bool isUnique(MaterialData* material);
	bool isUnique(Model* model);
	bool isUnique(FT_Face model);

	// Allocations
	Mesh* InstantiateMesh(const char* name);
	ShaderProgram* InstantiateShader(const char* name);
	GLuint InstantiateTexture(const char* name);
	MaterialData* InstantiateMaterial(const char* name);
	Model* InstantiateModel(const char* name);
	FT_Face InstantiateFont(const char* fontName);

	// Deletions
	// TODO: void* return? Actually write functions.
	// Remember that smart pointers may be necessary for references.
	bool DeleteMesh(const char* name) {};
	bool DeleteShader(const char* name) {};
	bool DeleteTexture(const char* name) {};
	bool DeleteMaterial(const char* name) {};
	bool DeleteModel(const char* name) {};
	bool DeleteFont(const char* name) {};

	// Directories
	const char* m_TextureDir = "Resources/Textures/";
};

#endif //!_ResourceManager_H_
