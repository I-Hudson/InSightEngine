#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "GBuffer.h"
#include <imgui.h>

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <memory>
#include <map>

class Entity;
class MeshComponent;
class AssimpMesh;
class LightVolumeManager;

enum ShaderPriority
{
	PRI_0, // draw first 
	PRI_1,
	PRI_2,
	PRI_3,
	PRI_4,
	PRI_5 //draw last
};

enum RenderMode
{
	Opaque,
	Transparent
};

class Shader
{
public:				

	std::map<std::string, float*> mFloatUniforms;
	std::map<std::string, unsigned int*> mUnsignedIntUniforms;

	//Construcotrs
	Shader();
	Shader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath, 
		   const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		   const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);
	//Destrucotr
	~Shader();

	//
	void					setAttributes();

	//Add a mesh to render
	void					addMesh(MeshComponent* aMesh);
	//Remove a mesh to render
	void					removeMesh(MeshComponent* aMesh);

	//Sort the meshes list
	void					sortMeshOrder();

	//Set the draw buffers for the shader
	void					setDrawBuffers();
	//Draw the meshes which have been added to the mMeshes vector
	void					drawMeshes(const bool& aBindTextures = true, const glm::mat4& aModelMatrx = glm::mat4());
	//Draw the meshes from aMeshes vector
	void					drawMeshes(std::vector<MeshComponent*>& aMeshes, const bool& aBindTextures = true, const glm::mat4& aModelMatrx = glm::mat4());

	//gui
	virtual void			gui();
	//use this shader (Bind, clear, setUnifroms, draw)
	virtual void			useShader(bool aClear, const bool& aActiveTextures, Entity* aE);
	virtual void			useShader(bool aClear, const bool& aActiveTextures);
	virtual void			useShader(bool aClear, const int& aDrawMode);
	virtual void			useShader(bool aClear = false);
	virtual void			useShaderPP(bool aClear = false);


	//Return the shader 
	unsigned int			getShader() const { return mShaderID; }
	//set the light volume for this shader
	virtual void			setLightVolumeManager(LightVolumeManager* aManager);

	//Create a frame buffer for this shader to render into 
	void					createFrameBuffer();
	//return a pointer to this shader's frame buffer
	unsigned int*			getFramebuffer() { return &mFrameBuffer; }
	//Attach a texture for this shader
	unsigned int			attachTextureToFrameBuffer(int aLevel, int aInternalFormat,
														int aWidth, int aHeight,
														int aBoarder, GLenum aFormat,
														GLenum aType, const void* aPixels,
														GLenum aAttachment);

	//set the entity this shader is attahced to
	void					setMeshComponent(MeshComponent* aMeshComponent);
	//get the mesh component 
	MeshComponent*			getMeshComponent();

	//set a gbuffer for this shader
	void					setGBuffer(GBuffer* aGBuffer);
	//return a pointer for the gbuffer 
	GBuffer*				getGBuffer();
		
	//set this shader's priority
	void					setShaderPriority(ShaderPriority a_shaderPriority = ShaderPriority::PRI_5);
	//get this shader's priority
	ShaderPriority			getShaderPriority();

	//get a render target from the framebuffer
	unsigned int*			getTexture(int aIndex);
	//return the number of render targets attached
	unsigned int			numOfTexturesAttached() const;
	//get all the render targets
	std::vector<FBTexture>* getAllTextures();

	//set all the uniforms for this shader
	void					setUniforms(unsigned int aProgramID = -1);

	/*
		Set Uniforms for different values type
	*/
	void					setBool(const std::string& aName, bool& aValue);
	void					setUnsignedIntUniforms(const std::string &aName, unsigned int& aValue);
	void					setUnsignedIntUniformsOverride(const std::string &aName, unsigned int& aValue);
	void					setFloat(const std::string &name, float& value, bool aSetEveryFrame = false);
	void					setFloatOverride(const std::string& aName, float& aValue);

	void					setVec2(const std::string& aName, glm::vec2 &value);
	void					setVec2(const std::string& aName, float x, float y);
													    
	void					setVec3(const std::string& aName, glm::vec3 &value);
	void					setVec3(const std::string& aName, float x, float y, float z);
													    
	void					setVec4(const std::string& aName, glm::vec4 &value, bool aSetEveryFrame = false);
	void					setVec4(const std::string& aName, float x, float y, float z, float w);
													    
	void					setMat2(const std::string& aName, glm::mat2& mat);
	void					setMat3(const std::string& aName, glm::mat3& mat);
	void					setMat4(const std::string& aName, glm::mat4& mat, bool aSetEveryFrame = false);
	void					setMat4Override(const std::string& aName, glm::mat4& mat);

protected:

	//framebuffer 
	unsigned int mFrameBuffer = -1;
	//shader
	unsigned int mShaderID;
	//light volume pointer
	LightVolumeManager* mLightVolumes;

	//matrix 4 uniform store
	std::map<std::string, glm::mat4*>mMat4Uniforms;
	//gbuffer
	GBuffer* mGBuffer;
	//all shader meshes
	std::vector<MeshComponent*> mMeshes;

private:

	//mesh component pointer 
	MeshComponent* mMeshComponent;

	//Which mode to render
	RenderMode mModeToRender;

	//Shader Priority
	ShaderPriority mShaderPriority;

	//all render targets
	std::vector<FBTexture> mFrameBufferTextures;

	/*
		Vectors for all the uniform types
	*/
	std::map<std::string, bool*> mBoolUniforms;

	std::map<std::string, glm::vec2*>mVec2Uniforms;
	std::map<std::string, glm::vec3*>mVec3Uniforms;
	std::map<std::string, glm::vec4*>mVec4Uniforms;

	std::map<std::string, glm::mat2*>mMat2Uniforms;
	std::map<std::string, glm::mat3*>mMat3Uniforms;

	//draw buffer
	std::vector<GLenum> drawBuffers;
};