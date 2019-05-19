#include "DirectionalLight.h"
#include "Shader.h"
#include "Utilities.h"
#include <glm/gtc/matrix_transform.hpp>

DirectionalLight::DirectionalLight()
{
}

DirectionalLight::DirectionalLight(const glm::vec4& aTargetPosition)
{
	setTargetPosition(aTargetPosition);
	setLightType(LightType::Directional);
}

DirectionalLight::~DirectionalLight()
{
}

void DirectionalLight::init()
{
	LightComponent::init();
	setPosition(glm::vec4(0, 0, 0, 1));
	setDiffuse(glm::vec4(1, 1, 1, 1));
	setSpecular(glm::vec4(1, 1, 1, 1));
}

void DirectionalLight::draw(Shader* aShader, const bool& aBindTextures)
{
	aBindTextures;
	if (!meshGenerated())
	{
		createQuadMesh();
	}

	setUniforms(aShader);

	mLightDir = glm::normalize(mTargetPosition - *getPosition());

	LightComponent::draw();
}

void DirectionalLight::update()
{
	float dt = Utility::getDeltaTime();
	float s = sinf(dt * 0.2f);
	float c = cosf(dt * 0.2f);
	glm::vec4 temp = *getPosition();
	glm::vec4 lightPos = *getPosition();
	lightPos.x = temp.x * c - temp.z * s;
	lightPos.z = temp.x * s + temp.z * c;
	lightPos = glm::normalize(lightPos) * 25.f;
	setPosition(lightPos);
	mLightDir = glm::normalize(mTargetPosition - *getPosition());

	if (shadowsAdded())
	{
		glm::mat4 depthViewMatrix = glm::lookAt(glm::vec3(lightPos.xyz),
			glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
		float size = 80;
		glm::mat4 depthProjectionMatrix = glm::ortho<float>(-size, size, -size, size, -100, 100);

		setShadowProViewMatrix(depthProjectionMatrix * depthViewMatrix);
	}
}

void DirectionalLight::gui()
{
	if (ImGui::CollapsingHeader("Directional Light Component"))
	{
		float tempColour[4];
		tempColour[0] = getDiffuse()->x;
		tempColour[1] = getDiffuse()->y;
		tempColour[2] = getDiffuse()->z;
		tempColour[3] = getDiffuse()->w;
		//colour
		if (ImGui::ColorPicker4("Colour", tempColour))
		{
			setDiffuse(glm::vec4(tempColour[0], tempColour[1], tempColour[2], tempColour[3]));
		}

		tempColour[0] = getSpecular()->x;
		tempColour[1] = getSpecular()->y;
		tempColour[2] = getSpecular()->z;
		tempColour[3] = getSpecular()->w;
		//specular
		if (ImGui::ColorPicker4("Specular", tempColour))
		{
			setSpecular(glm::vec4(tempColour[0], tempColour[1], tempColour[2], tempColour[3]));
		}

		//intensity
		if (ImGui::SliderFloat("Intensity", getIntensity(), 0.0f, 10.0f))
		{
		}

	}
}

void DirectionalLight::setTargetPosition(const glm::vec4 & aTargetPosition)
{
	mTargetPosition = aTargetPosition;
}

void DirectionalLight::createQuadMesh()
{
	addTri(glm::vec4(-1.f, -1.f, 0.f, 1.f), glm::vec4(1.f, -1.f, 0.f, 1.f), glm::vec4(-1.f, 1.f, 0.f, 1.f));
	addTri(glm::vec4(1.f, -1.f, 0.f, 1.f), glm::vec4(1.f, 1.f, 0.f, 1.f), glm::vec4(-1.f, 1.f, 0.f, 1.f));
}

void DirectionalLight::setUniforms(Shader* aShader)
{
	aShader->setVec4("dLight.Position", *getPosition(), true);
	aShader->setVec4("dLight.LightDir", mLightDir, true);
	aShader->setVec4("dLight.Diffuse", *getDiffuse(), true);
	aShader->setVec4("dLight.Specular", *getSpecular(), true);
	aShader->setFloat("dLight.Intensity", *getIntensity(), true);

	float l = 1.0f;
	aShader->setFloat("IsDirLight", l, true);
}