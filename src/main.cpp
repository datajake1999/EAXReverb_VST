
#ifndef __EAXReverb__
#include "EAXReverb.h"
#endif


AudioEffect* createEffectInstance (audioMasterCallback audioMaster)
{
    return new EAXReverb (audioMaster);
}

