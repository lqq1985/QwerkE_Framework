#include "GraphicsHelpers.h"
#include "../../Entities/Components/RenderComponent.h"
#include "../../QwerkE_Common/Utilities/FileIO/FileUtilities.h"
#include "../../QwerkE_Common/Utilities/StringHelpers.h"
#include "../../QwerkE_Common/Libraries/cJSON_Interface/cJSONInterface.h"
#include "../../Systems/ResourceManager/ResourceManager.h"
#include "../../Systems/ServiceLocator.h"
#include "../Material.h"
#include "../MaterialComponent.h"
#include "../Shader/ShaderProgram.h"
#include "../Shader/ShaderComponent.h"
#include "../Renderable.h"
#include "../Texture.h"
#include "../../Headers/QwerkE_Enums.h"

#include <map>
#include <string>

void CheckAllGraphicsErrors()
{
	// TODO: Handle all sorts of graphics system errors?
}

#ifdef OpenGL
#include "OpenGL/OpenGLHelpers.h"
void CheckGraphicsErrors()
{
	GLCheckforErrors();
}

void CheckGraphicsErrors(char* file, int line)
{
	GLCheckforErrors(file, line);
}

void CheckGraphicsErrors(const char* file, int line)
{
	GLCheckforErrors(file, line);
}

// TODO: Return a valid handle and try to avoid a stack frame if not necessary
GLuint Load2DTexture(const char* filename, bool flipVertically)
{
	return GLLoad2DTexture(filename, flipVertically);
}

GLuint LoadCubeMapTexture(const char* filename)
{
	return GLLoadCubeMapTexture(filename);
}

GLuint CopyFBOToTexture(FrameBufferObject& fbo, int w, int h, int x, int y)
{
	// TODO: abstract from library specific implementation
	// Note: You can specify what region of the framebuffer to take which
	// means you can easily capture a specified square from the scene.

	// bind framebuffer
	fbo.Bind();
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, w, h, 0);

	// create return texture
	GLuint result;
	glGenTextures(1, &result);
	glBindTexture(GL_TEXTURE_2D, result);

	// copy data
	glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, x, y, w, h, 1);
	CheckGraphicsErrors(__FILE__, __LINE__);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); // , , GL_CLAMP
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// return handle to newly created texture
	fbo.UnBind();
	glBindTexture(GL_TEXTURE_2D, 0); // unbind
	return result;
}

#elif defined(Vulcan)
#include "OpenGL/VulcanHelpers.h"

#elif defined(Direct3D)
#include "OpenGL/Direct3DHelpers.h"
#else
#pragma error "Define graphics library!"

#endif // OpenGL

void SaveObjectSchematic(RenderComponent* rComp) // save to file
{
	const char* filePath = ObjectSchematicFolderPath(StringAppend(rComp->GetSchematicName().c_str(), object_schematic_ext));

	// if file does not exist, create one, otherwise overwrite data
	if (!FileExists(filePath))
	{
		CreateEmptycJSONFile(filePath);
	}
	else
	{
		EmptycJSONFile(filePath);
	}

	cJSON* root = OpencJSONStream(filePath);

	if (root)
	{
		AddItemToRoot(root, CreateString("Name", rComp->GetSchematicName().c_str()));

		cJSON* renderables = CreateArray("Renderables");

		// TODO: Better way to get access to renderable values?
		std::vector<Renderable>* renderablesList = (std::vector<Renderable>*)rComp->LookAtRenderableList();

		for (size_t i = 0; i < renderablesList->size(); i++)
		{
			// TODO: Set the renderable names
			cJSON* renderable = CreateArray(StringAppend("R", std::to_string(i).c_str()));

			AddItemToArray(renderable, CreateString("Shader", renderablesList->at(i).GetShaderSchematic()->GetName().c_str()));
			AddItemToArray(renderable, CreateString("Material", renderablesList->at(i).GetMaterialSchematic()->GetMaterialName().c_str()));
			AddItemToArray(renderable, CreateString("MeshFile", renderablesList->at(i).GetMesh()->GetFileName().c_str()));
			AddItemToArray(renderable, CreateString("MeshName", renderablesList->at(i).GetMesh()->GetName().c_str()));

			AddItemToArray(renderables, renderable);
		}

		AddItemToRoot(root, renderables);
	}

	PrintRootObjectToFile(filePath, root);
	ClosecJSONStream(root);
}

RenderComponent* LoadRenderComponentFromSchematic(const char* schematicPath) // load from file
{
	RenderComponent* rComp = new RenderComponent();
	LoadRenderComponentFromSchematic(schematicPath, rComp);
	return rComp;
}
void LoadRenderComponentFromSchematic(const char* schematicPath, RenderComponent* renderComponent) // load from file
{
	cJSON* root = OpencJSONStream(schematicPath);

	ResourceManager* resMan = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
	if (root)
	{
		renderComponent->SetSchematicName(GetFileNameWithExt(schematicPath));

		cJSON* name = GetItemFromRootByKey(root, "Name");
		// rComp.name = name->valuestring;
		cJSON* renderables = GetItemFromRootByKey(root, "Renderables");
		int size = GetArraySize(renderables);

		renderComponent->AppendEmptyRenderables(size);

		for (int i = 0; i < size; i++) // per renderable
		{
			cJSON* currRenderable = GetItemFromArrayByIndex(renderables, i);

			renderComponent->SetNameAtIndex(i, currRenderable->string);
			renderComponent->SetShaderAtIndex(i, resMan->GetShaderProgram(GetItemFromArrayByKey(currRenderable, "Shader")->valuestring));
			renderComponent->SetMaterialAtIndex(i, resMan->GetMaterial(GetItemFromArrayByKey(currRenderable, "Material")->valuestring));
			renderComponent->SetMeshAtIndex(i, resMan->GetMeshFromFile(GetItemFromArrayByKey(currRenderable, "MeshFile")->valuestring, GetItemFromArrayByKey(currRenderable, "MeshName")->valuestring));

            std::vector<Renderable>* renderables = renderComponent->GetRenderableList();
            for (int i = 0; i < renderables->size(); i++)
            {
                renderables->at(i).GetMesh()->SetupShaderAttributes(renderables->at(i).GetShaderSchematic());
            }
		}
	}
	if (renderComponent->SeeRenderableList()->size() < 1)
	{
		renderComponent->AppendEmptyRenderables(1);
		renderComponent->SetNameAtIndex(0, "null_renderable");
		renderComponent->SetShaderAtIndex(0, resMan->GetShaderProgram(null_shader));
		renderComponent->SetMaterialAtIndex(0, resMan->GetMaterial(null_material));
		renderComponent->SetMeshAtIndex(0, resMan->GetMesh(null_mesh));
	}

	ClosecJSONStream(root);
}

void SaveMaterialSchematic(Material* mat)
{
	const char* filePath = TextureFolderPath(StringAppend(mat->GetMaterialName().c_str(), material_schematic_ext));

	// if file does not exist, create one,otherwise overwrite data
	if (!FileExists(filePath))
	{
		CreateEmptycJSONFile(filePath);
	}
	else
	{
		EmptycJSONFile(filePath);
	}

	// save data
	// Note: I am not saving texture handles because they are not reliable.
	// TODO: Should I use struct member names like "s_Name"?
	cJSON* root = OpencJSONStream(filePath);

	AddItemToRoot(root, CreateString("Name", "MaterialSchematic1.msch"));

	//cJSON* OtherData = CreateArray("OtherData");
	//AddItemToArray(OtherData, CreateNumber("Shine", mat->s_Shine));
	// TODO: LightData AddItemToArray(OtherData, CreateNumber("LightData", mat->s_Shine)); {r,g,b,a}
	// AddItemToRoot(root, OtherData);

	cJSON* Names = CreateArray("MaterialComponents");

	const std::map<eMaterialMaps, Texture*>* matComponents = mat->SeeMaterials();

	for (auto p : *matComponents)
	{
		Texture* texture = p.second;
		AddItemToArray(Names, CreateString(std::to_string((int)p.first).c_str(), texture->s_Name.c_str()));
	}

	// AddItemToArray(Names, CreateString("AmbientName", mat->s_AmbientName.c_str()));
	// AddItemToArray(Names, CreateString("DiffuseName", mat->s_DiffuseName.c_str()));
	// AddItemToArray(Names, CreateString("SpecularName", mat->s_SpecularName.c_str()));
	// AddItemToArray(Names, CreateString("EmissiveName", mat->s_EmissiveName.c_str()));
	// AddItemToArray(Names, CreateString("HeightName", mat->s_HeightName.c_str()));
	// AddItemToArray(Names, CreateString("NormalName", mat->s_NormalsName.c_str()));
	// AddItemToArray(Names, CreateString("ShininessName", mat->s_ShininessName.c_str()));
	// AddItemToArray(Names, CreateString("OpacityName", mat->s_OpacityName.c_str()));
	// AddItemToArray(Names, CreateString("DisplacementName", mat->s_DisplacementName.c_str()));
	// AddItemToArray(Names, CreateString("LightMapName", mat->s_LightMapName.c_str()));
	// AddItemToArray(Names, CreateString("ReflectionName", mat->s_ReflectionName.c_str()));

	AddItemToRoot(root, Names);

	PrintRootObjectToFile(filePath, root);
	ClosecJSONStream(root);
}

Material* LoadMaterialSchematic(const char* schematicPath)
{
	Material* mat = new Material();
	LoadMaterialSchematic(schematicPath, mat);
	return mat;
}

void LoadMaterialSchematic(const char* schematicPath, Material* material)
{
	cJSON* root = OpencJSONStream(schematicPath);
	if (root)
	{
		ResourceManager* resMan = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);

		// load mat data from file
		material->SetMaterialName(GetItemFromRootByKey(root, "Name")->valuestring);

		// load "other" data
		//cJSON* otherData = GetItemFromRootByKey(root, "OtherData");
		//mat->s_Shine = GetItemFromArrayByKey(otherData, "Shine")->valuedouble;
		// TODO: mat->s_LightValue = GetItemFromArrayByKey(otherData, "LightData")->valuedouble;

		// set texture names
		cJSON* textureNames = GetItemFromRootByKey(root, "TextureNames");

		for (int i = 0; i < GetArraySize(textureNames); i++)
		{
			cJSON* value = GetItemFromArrayByIndex(textureNames, i);
			if (value)
			{
				// ask for texture from resource manager, making sure the texture gets created
				// then add it to the material in its proper map "slot"
				material->AddTexture(resMan->GetTexture(value->valuestring), (eMaterialMaps)(value->string[0] - 48));
			}
		}
		// mat->s_AmbientName = GetItemFromArrayByKey(textureNames, "AmbientName")->valuestring;
		// mat->s_DiffuseName = GetItemFromArrayByKey(textureNames, "DiffuseName")->valuestring;
		// mat->s_SpecularName = GetItemFromArrayByKey(textureNames, "SpecularName")->valuestring;
		// mat->s_EmissiveName = GetItemFromArrayByKey(textureNames, "EmissiveName")->valuestring;
		// mat->s_HeightName = GetItemFromArrayByKey(textureNames, "HeightName")->valuestring;
		// mat->s_NormalsName = GetItemFromArrayByKey(textureNames, "NormalName")->valuestring;
		// mat->s_ShininessName = GetItemFromArrayByKey(textureNames, "ShininessName")->valuestring;
		// mat->s_OpacityName = GetItemFromArrayByKey(textureNames, "OpacityName")->valuestring;
		// mat->s_DisplacementName = GetItemFromArrayByKey(textureNames, "DisplacementName")->valuestring;
		// mat->s_LightMapName = GetItemFromArrayByKey(textureNames, "LightMapName")->valuestring;
		// mat->s_ReflectionName = GetItemFromArrayByKey(textureNames, "ReflectionName")->valuestring;
	}

	ClosecJSONStream(root);

	// load texture handles from ResourceManager
	ResourceManager* resMan = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);

	// TODO: Stop trying to load "Empty" files
	// material->s_AmbientHandle = resMan->GetTexture(mat->s_AmbientName.c_str());
	// material->s_DiffuseHandle = resMan->GetTexture(mat->s_DiffuseName.c_str());
	// material->s_SpecularHandle = resMan->GetTexture(mat->s_SpecularName.c_str());
	// material->s_EmissiveHandle = resMan->GetTexture(mat->s_EmissiveName.c_str());
	// material->s_HeightHandle = resMan->GetTexture(mat->s_HeightName.c_str());
	// material->s_NormalsHandle = resMan->GetTexture(mat->s_NormalsName.c_str());
	// material->s_ShininessHandle = resMan->GetTexture(mat->s_ShininessName.c_str());
	// material->s_OpacityHandle = resMan->GetTexture(mat->s_OpacityName.c_str());
	// material->s_DisplacementHandle = resMan->GetTexture(mat->s_DisplacementName.c_str());
	// material->s_LightMapHandle = resMan->GetTexture(mat->s_LightMapName.c_str());
	// material->s_ReflectionHandle = resMan->GetTexture(mat->s_ReflectionName.c_str());
}

void SaveShaderSchematic(ShaderProgram* shader)
{
	const char* filePath = StringAppend(AssetsDir, "Shaders/LitMaterial", shader_schematic_ext);

	// if file does not exist, create one,otherwise overwrite data
	if (!FileExists(filePath))
	{
		CreateEmptycJSONFile(filePath);
	}
	else
	{
		EmptycJSONFile(filePath);
	}

	cJSON* root = OpencJSONStream(filePath);

	if (root)
	{
		AddItemToRoot(root, CreateString("Name", shader->GetName().c_str()));

		AddItemToRoot(root, CreateString("vert", shader->GetVertShader()->GetName().c_str()));
		AddItemToRoot(root, CreateString("frag", shader->GetFragShader()->GetName().c_str()));
		AddItemToRoot(root, CreateString("geo", shader->GetGeoShader()->GetName().c_str()));

		PrintRootObjectToFile(filePath, root);
	}
	ClosecJSONStream(root);
}

ShaderProgram* LoadShaderSchematic(const char* schematicPath)
{
	ShaderProgram* shader = new ShaderProgram();
	ResourceManager* resMan = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);

	cJSON* root = OpencJSONStream(schematicPath);

	if (root)
	{
		shader->SetName(GetItemFromRootByKey(root, "Name")->valuestring);

		shader->SetVertName(GetItemFromRootByKey(root, "vert")->valuestring);
		shader->SetFragName(GetItemFromRootByKey(root, "frag")->valuestring);
		shader->SetGeoName(GetItemFromRootByKey(root, "geo")->valuestring);
	}

	shader->SetVertShader(resMan->GetShaderComponent(shader->GetVertName().c_str()));
	shader->SetFragShader(resMan->GetShaderComponent(shader->GetFragName().c_str()));
	shader->SetGeoShader(resMan->GetShaderComponent(shader->GetGeoName().c_str()));

	// TODO: Compile shader program

	ClosecJSONStream(root);

	return shader;
}

// Shader variable prefixes
char* Helper_GetAttributePrefix()
{
	return (char*)"a_"; // for attributes or i_ for inputs
}

char* Helper_GetUniformPrefix()
{
	return (char*)"u_"; // uniforms
}

char* Helper_GetTransferPrefix()
{
	return (char*)"t_"; // for transfers or v_ for varyings
}