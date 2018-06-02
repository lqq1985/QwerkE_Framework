#ifndef _LightData_H_
#define _LightData_H_

#include "../../QwerkE_Common/Math_Includes.h"

struct LightData
{
	vec3 position;

	vec3 ambient = vec3(1, 1, 1);
	vec3 diffuse = vec3(1, 1, 1);
	vec3 specular = vec3(1, 1, 1);
};

#endif //__LightData_H__
