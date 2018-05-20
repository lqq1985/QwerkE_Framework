#include "../../Systems/ResourceManager/ResourceManager.h"
#include "../../Systems/Graphics/Gfx_Classes/MaterialData.h"
#include "../../Systems/Graphics/ShaderProgram/ShaderFactory.h"
#include "../../../QwerkE_Common/Utilities/StringHelpers.h"
#include "../../../QwerkE_Common/Utilities/PrintFunctions.h"
#include "../../../QwerkE_Common/Utilities/FileIO/FileLoader/FileLoader.h"
#include "../../Math_Includes.h"
#include "../../Libraries/glew/GL/glew.h"
#include "../../QwerkE_Framework/QwerkE_Directory_Defines.h"

// TODO: Look at resource creation again. Should Resource Manager create assets or just store them?
// TODO: Load all files in folder. This avoids hard coded assets names and allows easy adding/removal of assets even at runtime.
// Objects may need to switch to assets ids. ids would act as unique identifiers in the asset list and would prevent crashing.
void ResourceManager::Init()
{
	m_NullMesh = InstantiateMesh(null_mesh);
	m_NullShader = InstantiateShader(null_shader);
	m_NullTexture = InstantiateTexture(null_texture); // TODO: Create a Texture class
	m_NullMaterial = InstantiateMaterial(null_material);
	m_NullModel = InstantiateModel(null_model);
	m_NullFont = InstantiateFont(null_font); // TODO: Create a valid null font
	// m_NullSound = InstantiateSound(null_sound); // TODO: Create a sound object class
}

// Instantiation Functions
// TODO: Handle errors and deleting assets before returning nullptr
Mesh* ResourceManager::InstantiateMesh(const char* meshName)
{
	MeshFactory t_MeshFactory;
	Mesh* mesh = nullptr;
    // TODO: Dereference *s?
	if (meshName == "Box") // Asset name
	{
		mesh = new Mesh();
		t_MeshFactory.GenerateBox(mesh, vec2(1, 1));
	}
	else if (meshName == "Circle")
	{
		mesh = MeshFactory::CreateCircle(1.0f, 20, vec2(1,1));
	}
	else if (meshName == "Cube")
	{
		mesh = new Mesh();
		t_MeshFactory.GenerateCube(mesh, vec3(1, 1, 1)); // TODO: Has UV issues
	}
	else if (meshName == "CreBox")
	{
		mesh = t_MeshFactory.CreateBox(vec2(1, 1));
	}
	else if (meshName == "CreCube")
	{
		mesh = t_MeshFactory.CreateCube(vec3(1, 1, 1), vec2(1, 1), true);
	}
	else if (meshName == "Plane")
	{
		mesh = t_MeshFactory.CreatePlane(vec2(10, 10), vec2(2, 2), vec2(1, 1));
	}
	else if (meshName == "TutorialCube")
	{
		mesh = t_MeshFactory.TutorialCube(vec3(1,1,1));
	}
	// update to use new model loading capabilities
    /*
	else if (meshName == "Teapot.obj")
    {
        mesh = t_MeshFactory.ImportOBJMesh(MeshPath("Teapot.obj"), vec3(0.5f,0.5f,0.5f), vec2(1,1), false);
    }
	*/
	else if (meshName == null_mesh)
	{
		mesh = t_MeshFactory.ImportOBJMesh(ModelFolderPath("null_model.obj"), vec3(0.5f, 0.5f, 0.5f), vec2(1, 1), false);
	}
	else
	{
        ConsolePrint("\nInstantiateMesh(): Mesh not found!\n");
		return m_NullMesh;
	}
	m_Meshes[meshName] = mesh; // Add to active list
	mesh->SetName(meshName);
	return mesh;
}

ShaderProgram* ResourceManager::InstantiateShader(const char* shaderName)
{
	// Read directory for file?

	// TODO: Create a .qshader file that just lists what shaders to use
	// and then build a new program using those shaders.
	// This might be how dynamic shaders work

	ShaderProgram* shader = new ShaderProgram();
	// 2D
	if (shaderName == "Basic2DTex") // Asset name
	{
		shader->Init(ShaderFolderPath("Basic2DTex.vert"), ShaderFolderPath("Basic2DTex.frag"), NULL); // Asset directories
	}
	else if (shaderName == "2DMenu")
	{
		shader->Init(ShaderFolderPath("2DMenu.vert"), ShaderFolderPath("2DMenu.frag"), NULL);
	}
	else if (shaderName == "Basic2D")
	{
		// shader->Init(eShader_Basic2D);
		shader->Init(ShaderFolderPath("Basic2D.vert"), ShaderFolderPath("Basic2D.frag"), NULL);
	}
	else if (shaderName == "2DMenuText")
	{
		shader->Init(ShaderFolderPath("2DMenuText.vert"), ShaderFolderPath("2DMenuText.frag"), NULL);
	}
	else if (shaderName == "Basic2DTransform")
	{
		shader->Init(ShaderFolderPath("Basic2DTransform.vert"), ShaderFolderPath("Basic2DTransform.frag"), NULL);
	}
	else if (shaderName == "Sprite2D")
	{
		shader->Init(ShaderFolderPath("Sprite2D.vert"), ShaderFolderPath("Sprite2D.frag"), NULL);
	}
	// 3D
	else if (shaderName == "Basic3D")
	{
		ShaderFactory temp;
		shader = temp.CreateShader(eShader_Basic3D);
	}
	else if (shaderName == "LitMaterial")
	{
		shader->Init(ShaderFolderPath("LitMaterial.vert"), ShaderFolderPath("LitMaterial.frag"), NULL);
	}
	else if (shaderName == "vec3Material")
	{
		shader->Init(ShaderFolderPath("vec3Material.vert"), ShaderFolderPath("vec3Material.frag"), NULL);
	}
	else if (shaderName == "BasicLighting")
	{
		shader->Init(ShaderFolderPath("vec3Material.vert"), ShaderFolderPath("vec3Material.frag"), NULL);
	}
	else if (shaderName == "Box2D_Debug")
	{
		// InstantiateShader("Data/Shaders/Box2DDebug.vert", "Data/Shaders/Box2DDebug.frag", NULL, "Box2D_Debug");
	}
	else if (shaderName == "text")
	{
		shader->Init(ShaderFolderPath("text.vert"), ShaderFolderPath("text.frag"), NULL);
	}
    else if (shaderName == "TestShader")
    {
        shader->Init(ShaderFolderPath("TestShader.vert"), ShaderFolderPath("TestShader.frag"), NULL);
    }
	else if (shaderName == null_shader)
	{
		shader->Init(ShaderFolderPath("null_shader.vert"), ShaderFolderPath("null_shader.frag"), NULL);
	}
	else
	{
        ConsolePrint("\nInstantiateShader(): Shader not found!\n");
		return m_NullShader;
	}

	m_Shaders[shaderName] = shader;
	return shader;
}

GLuint ResourceManager::InstantiateTexture(const char* textureName)
{
	GLuint textureHandle = -1;
	textureHandle = Load2DTexture(TextureFolderPath(textureName));

	if (textureHandle != 0)
		m_Textures[textureName] = textureHandle;
	else
	{
		ConsolePrint("\nInstantiateTexture(): Texture not found!\n");
		textureHandle = m_NullTexture;
	}

	return textureHandle;
}

MaterialData* ResourceManager::InstantiateMaterial(const char* matName)
{
	MaterialData* material = nullptr;
	// TODO: Set map names
	if (matName == "container.mat") // Asset name
	{
		material = new MaterialData(GetTexture("container.png"), GetTexture("container.png"), GetTexture("container_specular.png")); // Asset names
	}
	else if (matName == "nanosuit_arms.mat") // TODO: Re-use textures
	{
		GLuint dif, spec, norm;

		dif = Load2DTexture(MaterialFolderPath("nanosuit/arm_dif.png"));
		spec = Load2DTexture(MaterialFolderPath("nanosuit/arm_showroom_spec.png"));
		// norm = Load2DTexture(MaterialDir("nanosuit/arm_showroom_ddn.png")); // currently unused

		material = new MaterialData(dif, dif, spec);
	}
	else if (matName == "nanosuit_legs.mat") // TODO: Re-use textures
	{
		GLuint dif, spec, norm;

		dif = Load2DTexture(MaterialFolderPath("nanosuit/leg_dif.png"));
		spec = Load2DTexture(MaterialFolderPath("nanosuit/leg_showroom_spec.png"));
		// norm = Load2DTexture(MaterialDir("nanosuit/leg_showroom_ddn.png")); // currently unused

		material = new MaterialData(dif, dif, spec);
	}
	else if (matName == "nanosuit_body.mat") // TODO: Re-use textures
	{
		GLuint dif, spec, norm;

		dif = Load2DTexture(MaterialFolderPath("nanosuit/body_dif.png"));
		spec = Load2DTexture(MaterialFolderPath("nanosuit/body_showroom_spec.png"));
		// norm = Load2DTexture(MaterialDir("nanosuit/body_showroom_ddn.png")); // currently unused

		material = new MaterialData(dif, dif, spec);
	}
	else if (matName == "nanosuit_glass.mat") // TODO: Re-use textures
	{
		GLuint dif, spec, norm;

		dif = Load2DTexture(MaterialFolderPath("nanosuit/glass_dif.png"));
		// spec = Load2DTexture(MaterialDir("nanosuit/?????.png"));
		// norm = Load2DTexture(MaterialDir("nanosuit/glass_ddn.png")); // currently unused

		material = new MaterialData(dif, dif, dif);
	}
	else if (matName == "nanosuit_helmet.mat") // TODO: Re-use textures
	{
		GLuint dif, spec, norm;

		dif = Load2DTexture(MaterialFolderPath("nanosuit/helmet_diff.png"));
		spec = Load2DTexture(MaterialFolderPath("nanosuit/helmet_showroom_spec.png"));
		// norm = Load2DTexture(MaterialDir("nanosuit/helmet_showroom_ddn.png")); // currently unused

		material = new MaterialData(dif, dif, spec);
	}
	else if (matName == "nanosuit_hand.mat") // TODO: Re-use textures
	{
		GLuint dif, spec, norm;

		dif = Load2DTexture(MaterialFolderPath("nanosuit/hand_dif.png"));
		spec = Load2DTexture(MaterialFolderPath("nanosuit/hand_showroom_spec.png"));
		// norm = Load2DTexture(MaterialDir("nanosuit/hand_showroom_ddn.png")); // currently unused

		material = new MaterialData(dif, dif, spec);
	}
	else if (matName == "UV_Map.mat")
	{
		material = new MaterialData(GetTexture("UV_Map.png"), GetTexture("UV_Map.png"), GetTexture("UV_Map.png"));
	}
    else if (matName == null_material)
    {
        material = new MaterialData(GetTexture("null_texture.png"), GetTexture("null_texture.png"), GetTexture("null_texture.png"));
    }
	else if (matName == "Test.qmat") // could just check for extension, then handle that
	{
		// TODO: Try to load a .qmat file
	}
	else
	{
        ConsolePrint("\nInstantiateMaterial(): Material not found!\n");
		return m_NullMaterial; // do not add another material
	}

	m_Materials[matName] = material;
	material->name = matName;
	return material;
}

Model* ResourceManager::InstantiateModel(const char* modelName)
{
	// MeshFactory meshFact; // out dated
	// if (modelName == "LightBulb") // Asset name
	// model = meshFact.ImportOBJModel("Resources/Models/Light_Bulb.obj"); // Asset directory

	Model* model = QwerkE::FileLoader::LoadModelFile(ModelFolderPath(modelName));

	// null check
	if (model != nullptr)
	{
		m_Models[modelName] = model;
		model->SetName(modelName);
		return model;
	}
	else
	{
		ConsolePrint("\nInstantiateModel(): Model not found!\n");
		return m_NullModel;
	}
}

FT_Face ResourceManager::InstantiateFont(const char* fontName)
{
	FT_Face font;
	FT_Library ft; // TODO: No need to reload ft library

	if (FT_Init_FreeType(&ft))
		ConsolePrint("ERROR::FREETYPE: Could not init FreeType Library");

	if (FT_New_Face(ft, FontFolderPath(fontName), 0, &font))
	{
		ConsolePrint("ERROR::FREETYPE: Failed to load font");
		return NULL;
	}
	m_Fonts[fontName] = font;
	FT_Done_FreeType(ft);
	assert(font != NULL);
	return font;
}

ALuint ResourceManager::InstantiateSound(const char* soundName, DWORD& bufferSize, unsigned short& channels)
{
	// TODO: QwerkE::FileLoader::LoadSoundFile(SoundFolderPath(soundName), bufferSize, channels, frequency);
	return 0;
}