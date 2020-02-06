#include "JobQueuedEvent.h"
#include "../Resources/Resources.h"
#include "../Services.h"

namespace QwerkE {

    JobQueuedEvent::JobQueuedEvent()
    {
        m_EventType = eEventTypes::eEvent_JobQueued;
    }

    JobQueuedEvent::~JobQueuedEvent()
    {
    }

    void JobQueuedEvent::Process()
    {
        ((JobManager*)QwerkE::Services::GetService(eEngineServices::JobManager))->ProcessTasks();
    }

}
