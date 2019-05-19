#pragma once

#ifndef __SPOT_LIGHT_H__
#define __SPOT_LIGHT_H__

#include <glm/ext.hpp>
#include "LightComponent.h"

class SpotLight : public LightComponent
{
public:
	//Constructor
	SpotLight();
	SpotLight(const glm::vec4& aLightTarget,
			 const float& aInnerCutOff, const float& aOuterCutoff, const float& aLength);
	//Destructor
	~SpotLight();

	void		init();
	void		draw(Shader* aShader, const bool& aBindTextures = true) override;
	void		update() override;
	void		gui()override;

private:
	//create the cone mesh for the light volume
	void		createConeMesh();
	//set all the uniforms for this light
	void		setUniforms(Shader* aShader);

	//cutoff for the inner edge of the spot light
	float mInnerCutoff;
	//cutoff for the outer edge of the spot light
	float mOuterCutoff;
	//the length the spot light extends
	float mLength;

	//Constatnt for attenuation
	float mConstant;
	//Linear for attenuation 
	float mLinear;
	//Quadratic for attenuation 
	float mQuadratic;
	//Ambient for light volume
	glm::vec4 mAmbient;
};

#endif // __SPOT_LIGHT_H__
