#ifndef _OpenAL_Helpers_H_
#define _OpenAL_Helpers_H_

#include "../../Libraries/OpenAL/include/al.h"
#include "../../Libraries/OpenAL/include/alc.h"
#include "../FileSystem/FileSystem.h"

#include <string>

namespace QwerkE {

    void CheckForOpenALErrors(const char* file, int line);
    std::string list_audio_devices(const ALCchar* devices);
    ALuint OpenAL_LoadSound(const QSoundFile& soundFile);

}
#endif // !_OpenAL_Helpers_H_
