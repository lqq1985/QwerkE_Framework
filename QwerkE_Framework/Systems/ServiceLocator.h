#ifndef _ServiceLocator_H_
#define _ServiceLocator_H_

// Class ServiceLocator provides global access for systems
// necessary throughout the code such as memory management,
// audio playback, input, etc.
// To add a system add it to RegisterService(), UnregisterService,
// GetService(), and ServicesLoaded().

#include "../QwerkE_Enums.h"
#include "./../../QwerkE_Common/Utilities/PrintFunctions.h"
#include "ResourceManager/ResourceManager.h"
#include "Input/InputManager.h"

class ResourceManager;
class InputManager;
class EventManager;
class SceneManager;
class Factory;
class PhysicsManager;
class MessageManager;
class Renderer;
class AudioManager;
class JobManager;
class Window;
class NetworkManager;
class ShaderFactory;
class DataManager;
class FileSystem;

namespace QwerkE
{
	// TODO: Improve service accessing
	// Think of toggling register/unregister
    // Should code ask for system*s to keep or is this dangerous because
    //  a system could change during runtime?
    // Consider inline ing service locator sunctions for runtime efficiency

    // TODO: To avoid dependencies redesign how services are registered/unregistered
    // for order dependency issues.

    // TODO: Consider returning NULL objects if service is nullptr like NullAudioManager() or
	// something more elegant.

    // TODO: Try to avoid if(null) check inside a getter(). Make sure services are
    // initialized at start to null and have null checks inside a register() call.

    // TODO: Consider limiting accessible scope on systems like NetworkManager to only a
    // few classes. Maybe create a INetwork interface that needs to be implemented
    // by a class to access the network service... or something like that.

	// TODO: May want to create a ServicePtr<Resource*> that points to another pointer
	// so that services can be easily swapped. This ServicePtr can also act
	// as a smart pointer to reference count and ensure deletion and global pointer
	// re-assignment if needed. Also return these ServicePtrs as const to avoid altering.
	// Note: By abstracting to raw pointer value order dependency becomes less important
	// during initialization unless the services really rely on each other, rather than
	// just storing convenience pointers.

	class ServiceLocator
	{
	public:
		// Services need to be instantiated, then registered
		// TODO: Services...
		// Networking, Graphics, Utilities (some type of file managers?),
		// Debug, Memory, Application
		static void RegisterService(eEngineServices serviceType, void* service);

		static void* UnregisterService(eEngineServices serviceType); // TODO: Potentially dangerous!

		static void* GetService(eEngineServices serviceType);
		// TODO: Improve logic and code design
		static eEngineMessage ServicesLoaded(); // determine if all services are loaded

		static void LockServices(bool locking) { m_IsLocked = locking; }

	private:
		static bool m_IsLocked; // TODO: Improve security/error prevention

		static ResourceManager* m_ResourceManager;
		static InputManager* m_InputManager;
        static EventManager* m_EventManager;
        static SceneManager* m_SceneManager;
        static Factory* m_Factory;
		static ShaderFactory* m_ShaderFactory;
		static PhysicsManager* m_PhysicsManager;
		static MessageManager* m_MessageManager;
		static Renderer* m_Renderer;
        static AudioManager* m_AudioManager;
		static JobManager* m_JobManager;
		static Window* m_Window;
        static NetworkManager* m_NetworkManager;
		static DataManager* m_DataManager;
		static FileSystem* m_FileSystem;
	};
}

#endif //!_ServiceLocator_H_
