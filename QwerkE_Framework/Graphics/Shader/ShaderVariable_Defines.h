#ifndef _ShaderVariable_Defines_H_
#define _ShaderVariable_Defines_H_

//////////////////////////////
// ShaderVariable_Defines.h
// Define shader variable names so finding and loading their
// data is easy. These defines allow quick and easy name changes
// while maintaining consistency throughout the code. No need for
// hard coded "strings" anywhere!

// This data should mirror ShaderVariableDoc.txt.
//////////////////////////////

// List all shader variable strings to read and change them
// easily and maintain consistency.

/* Attributes */
#define vertexPosition "Position"
#define vertexNormal "Normal"
#define vertexUV "UV"
#define vertexColour "Color"

/* Uniforms */
// Transforms
#define worldMatrix "WorldMat"
#define viewMatrix "ViewMat"
#define projectionMatrix "ProjMat"
#define transform2D "Transform2D"

// Materials
#define TexturePrefix "Texture"
#define AmbientName "AmbientTexture"
#define DiffuseName "DiffuseTexture"
#define SpecularName "SpecularTexture"
#define EmissiveName "EmissiveTexture"
#define HeightName "HeightTexture"
#define NormalsName "NormalsTexture"
#define ShininessName "ShininessTexture"
#define OpacityName "OpacityTexture"
#define DisplacementName "DisplacementTexture"
#define LightMapName "LightMapTexture"
#define ReflectionName "ReflectionTexture"

// Camera
#define cameraPosition "CamPos"

// Lighting
#define lightColor "LightColour"
#define lightPosition "LightPos"

// Other
#define objectColor "ObjectColor"

// TODO: How to handle transfer variables
/* Transfers */

#endif // !_ShaderVariable_Defines_H_