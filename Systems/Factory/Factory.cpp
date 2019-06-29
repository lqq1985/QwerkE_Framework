#include "Factory.h"
#include "../../Systems/ResourceManager/ResourceManager.h"
#include "../../Systems/ServiceLocator.h"
#include "../../Entities/Components/RenderComponent.h"
#include "../../Graphics/GraphicsUtilities/GraphicsHelpers.h"
#include "../../QwerkE_Common/Utilities/StringHelpers.h"
#include "../../QwerkE_Common/Utilities/FileIO/FileUtilities.h"

Factory::Factory()
{
	m_pResources = (ResourceManager*)QwerkE::ServiceLocator::GetService(eEngineServices::Resource_Manager);
}

Factory::~Factory()
{
}

void Factory::ClearResources()
{
	m_pResources->DeleteAllResources(); // TODO: Shouldn't have access to resources through factory
}
GameObject* Factory::CreateGameObject(Scene* scene)
{
	return new GameObject(scene);
}
GameObject* Factory::CreateGameObject(Scene* scene, vec3 position)
{
	return new GameObject(scene, position);
}

GameObject* Factory::CreateObjectFromSchematic(const char* schematicName, Scene* scene, vec3 position)
{
	GameObject* object = CreateGameObject(scene, position);
	object->SetName(GetFileNameWithExt(schematicName));
	object->AddComponent(LoadRenderComponentFromSchematic(ObjectSchematicFolderPath(schematicName)));
	return object;
}

RenderComponent* Factory::AddModelComponentFromSchematic(GameObject* entity, const char* objectRecipeName)
{
	RenderComponent* mComp = LoadRenderComponentFromSchematic(ObjectSchematicFolderPath(objectRecipeName));
	entity->AddComponent((Component*)mComp);
	return mComp;
}