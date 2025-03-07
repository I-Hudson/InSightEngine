#pragma once

#include "Shader/BaseShader.h"
#include "Shader/DeferredShader.h"
#include "Shader/VertexShader.h"
#include "LightVolumeManager.h"

#include <vector>

class DeferredLightPassShader : public BaseShader
{
public:

	//Constructors
	DeferredLightPassShader();
	DeferredLightPassShader(const char* aVertexPath, const char* aControlpath, const char* aEvaluationpath,
		const char* aGeometryPath, const char* aFragmentPath, unsigned int aInputCount = 3,
		const char* aInputs[] = nullptr, unsigned int aOutputCount = 1, const char* aOutputs[] = nullptr);

	//Destructor
	~DeferredLightPassShader();

	//destroy
	void				destroy() override;

	//use this BaseShader (Bind, clear, setUnifroms, draw)
	void				useShader(bool aClear = false) override;
	//void				useShader(bool aClear = false, PointLight* aP = nullptr);

	//return thr stencil BaseShader
	BaseShader*				getStencilShader();
	//set the deferred BaseShader
	//void				setDeferredShader(DeferredShader* aShader);

	//override setLightManager function
	void				setLightVolumeManager(LightVolumeManager* aManager) override;

	//set the hdr state for the light BaseShader
	void setHDRState(bool aState);

	//get the hdr state for the light BaseShader
	bool getHDRState();

protected:

	/*
		Debug drawing
	*/

	//Add a Line
	void				addLine(glm::vec4 aP0, glm::vec4 aP1);
	//Add a tri
	void				addTri(glm::vec4 aP0, glm::vec4 aP1, glm::vec4 aP2);
	//Add a sphere
	//void				addPointLight(glm::vec4 aPosition, float aRadius);
	//Add a fullscreen rect/quad
	//void				addFullScreenQuad();

private:

	//light gbuffer 
	GBuffer* mLightBuffer;

	bool mHDRState;

	//Stencil BaseShader
	VertexShader* mStencilShader;
	//Post light BaseShader to render point lights
	BaseShader* mPostLightShader;

	//Line struct
	struct Line
	{
		glm::vec4 p0;
		glm::vec4 p1;
	};

	//Tri struct
	struct Tri
	{
		glm::vec4 p1;
		glm::vec4 p2;
		glm::vec4 p3;
	};

	//Fullscreen quad vertex
	struct FSQuadVertex
	{
		glm::vec4 Position;
		glm::vec2 UV;
		FSQuadVertex() {}
		FSQuadVertex(glm::vec4 aPosition, glm::vec2 aUV) { Position = aPosition; UV = aUV; }
	};

	//Line VABO, VBO, Line count and pointer to line array
	unsigned int mLineVAO;
	unsigned int mLineVBO;
	unsigned int mLineCount;
	Line* mLines;

	//Tri VABO, VBO, Tri count and pointer to Tri array
	unsigned int mTriVAO;
	unsigned int mTriVBO;
	unsigned int mTriCount;
	Tri* mTris;

	//Fullscreen quad count
	unsigned int mFSQuadCount;
};