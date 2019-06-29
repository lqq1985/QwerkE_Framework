#ifndef _OpenAL_Audio_Manager_H_
#define _OpenAL_Audio_Manager_H_

#include "AudioManager.h"

#include "../../QwerkE_Common/Libraries/OpenAL/include/al.h"
#include "../../QwerkE_Common/Libraries/OpenAL/include/alc.h"
#include "../../QwerkE_Common/DataTypes/Vector.h"

#include <vector>

class AudioSource;

class OpenALAudioManager : public AudioManager
{
public:
    OpenALAudioManager();
    ~OpenALAudioManager();

    void PlaySound(const char* name);
    // music, effect, dialogue

    void SetListenerOrientation(vec3 position, vec3 velocity);

private:

    ALCdevice* Device = nullptr;
    ALCcontext* Context = nullptr;

    // ALboolean g_bEAX; // Why?

    // buffer data
    ALboolean loop = AL_FALSE;
    AudioSource* m_Source = nullptr;
};

#endif // _OpenAL_Audio_Manager_H_