#include "LightManager.h"


LightManager::LightManager()
{

}


LightManager::LightManager(const LightManager& other)
{
}


LightManager::~LightManager()
{
	for(int i = 0; i < GetNumberOfImageBasedLights(); i++)
	{
		if (ImageBasedLights[i].DiffuseProbe)
		{
			ImageBasedLights[i].DiffuseProbe->Release();
			ImageBasedLights[i].DiffuseProbe = 0;
		}

		if (ImageBasedLights[i].SpecularProbe)
		{
			ImageBasedLights[i].SpecularProbe->Release();
			ImageBasedLights[i].SpecularProbe = 0;
		}

		if (ImageBasedLights[i].SpecularIntegration)
		{
			ImageBasedLights[i].SpecularIntegration->Release();
			ImageBasedLights[i].SpecularIntegration = 0;
		}

	}
	
}

//Get number of lights
int LightManager::GetNumberOfDirectionalLights()
{
	return DirectionalLights.size();
}
int LightManager::GetNumberOfPointLights()
{
	return PointLights.size();
}
int LightManager::GetNumberOfSpotLights()
{
	return SpotLights.size();
}
int LightManager::GetNumberOfCapsuleLights()
{
	return CapsuleLights.size();
}

int LightManager::GetNumberOfImageBasedLights()
{
	return ImageBasedLights.size();
}

//Add new Lights functions

void LightManager::AddDirectionalLight(float dirX, float dirY, float dirZ,
	float colorR, float colorG, float colorB, float ambientDownColorR,
	float ambientDownColorG, float ambientDownColorB, float ambientColorUpX,
	float ambientColorUpY, float ambientColorUpZ)
{
	DirectionalLightData tempLight(dirX, dirY, dirZ, colorR, colorG, colorB, 
		ambientDownColorR, ambientDownColorG, ambientDownColorB,
		ambientColorUpX, ambientColorUpY, ambientColorUpZ);

	DirectionalLights.push_back(tempLight);
}

void LightManager::AddDirectionalLight(DirectX::FXMVECTOR Direction,
	DirectX::FXMVECTOR Color, DirectX::FXMVECTOR DownColor, DirectX::CXMVECTOR UpColor)
{
	DirectionalLightData tempLight(DirectX::XMVectorGetX(Direction), DirectX::XMVectorGetY(Direction), DirectX::XMVectorGetZ(Direction), DirectX::XMVectorGetX(Color), DirectX::XMVectorGetY(Color), DirectX::XMVectorGetZ(Color),
		DirectX::XMVectorGetX(DownColor), DirectX::XMVectorGetY(DownColor), DirectX::XMVectorGetZ(DownColor),
		DirectX::XMVectorGetX(UpColor), DirectX::XMVectorGetY(UpColor), DirectX::XMVectorGetZ(UpColor));

	DirectionalLights.push_back(tempLight);
}

void LightManager::AddPointLight(float posX, float posY, float posZ,
	float colorR, float colorG, float colorB, float range)
{
	PointLightData tempLight(posX, posY, posZ, colorR, colorG, colorB, range);

	PointLights.push_back(tempLight);
}

void LightManager::AddPointLight(DirectX::FXMVECTOR Position,
	DirectX::FXMVECTOR Color, float range)
{
	PointLightData tempLight(DirectX::XMVectorGetX(Position), DirectX::XMVectorGetY(Position), DirectX::XMVectorGetZ(Position), DirectX::XMVectorGetX(Color), DirectX::XMVectorGetY(Color), DirectX::XMVectorGetZ(Color), range);

	PointLights.push_back(tempLight);
}

void LightManager::AddSpotLight(float posX, float posY, float posZ, float dirX, float dirY, float dirZ, float colorR, float colorG,
	float colorB, float range, float innerAngel, float outerAngel)
{
	SpotLightData tempLight(posX, posY, posZ, colorR, colorG, colorB, dirX, dirY, dirZ, range, innerAngel, outerAngel);

	SpotLights.push_back(tempLight);
}

void LightManager::AddSpotLight(DirectX::FXMVECTOR Position, DirectX::FXMVECTOR Direction, DirectX::FXMVECTOR Color, float range, float innerAngel, float outerAngel)
{
	SpotLightData tempLight(DirectX::XMVectorGetX(Position), DirectX::XMVectorGetY(Position), DirectX::XMVectorGetZ(Position), DirectX::XMVectorGetX(Color), DirectX::XMVectorGetY(Color), DirectX::XMVectorGetZ(Color), DirectX::XMVectorGetX(Direction), DirectX::XMVectorGetY(Direction), DirectX::XMVectorGetZ(Direction), range, innerAngel, outerAngel);

	SpotLights.push_back(tempLight);
}


void LightManager::AddCapsuleLight(float posX, float posY, float posZ, float colorR, float colorG, float colorB, float dirX, float dirY, float dirZ, float range, float length)
{
	CapsuleLightData tempLight(posX, posY, posZ, colorR, colorG, colorB, dirX, dirY, dirZ, range, length);

	CapsuleLights.push_back(tempLight);

}

void LightManager::AddCapsuleLight(DirectX::FXMVECTOR Position, DirectX::FXMVECTOR Color, DirectX::FXMVECTOR Direction, float range, float length)
{
	CapsuleLightData tempLight(DirectX::XMVectorGetX(Position), DirectX::XMVectorGetY(Position), DirectX::XMVectorGetZ(Position), DirectX::XMVectorGetX(Color), DirectX::XMVectorGetY(Color), DirectX::XMVectorGetZ(Color), DirectX::XMVectorGetX(Direction), DirectX::XMVectorGetY(Direction), DirectX::XMVectorGetZ(Direction), range, length);

	CapsuleLights.push_back(tempLight);

}

bool LightManager::AddImageBasedLight(ID3D11Device* device, std::string DiffuseProbeFileName, std::string SpecularProbeFileName, std::string SpecularIntegrationFilename, DirectX::FXMVECTOR Position,
	float range)
{
	bool result;
	ImageBasedLightData tempLight;

	//Convert the diffuse probe filename into wcstring
	size_t tDiffuseProbeFilename = DiffuseProbeFileName.length() + 1;
	wchar_t * wcDiffuseProbeFilename = new wchar_t[tDiffuseProbeFilename];

	size_t DiffuseProbeFilenameConvertedChars = 0;
	mbstowcs_s(&DiffuseProbeFilenameConvertedChars, wcDiffuseProbeFilename, tDiffuseProbeFilename, DiffuseProbeFileName.c_str(), _TRUNCATE);

	//Convert the specular probe filename into wcstring
	size_t tSpecularFilename = SpecularProbeFileName.length() + 1;
	wchar_t * wcSpecularProbeFileName = new wchar_t[tSpecularFilename];

	size_t SpecularFilenameConvertedChars = 0;
	mbstowcs_s(&SpecularFilenameConvertedChars, wcSpecularProbeFileName, tSpecularFilename, SpecularProbeFileName.c_str(), _TRUNCATE);

	//Convert the specular integration filename into wcstring
	size_t tSpecularIntegrationFilename = SpecularIntegrationFilename.length() + 1;
	wchar_t * wcSpecularIntegrationFileName = new wchar_t[tSpecularIntegrationFilename];

	size_t SpecularIntegrationFilenameConvertedChars = 0;
	mbstowcs_s(&SpecularIntegrationFilenameConvertedChars, wcSpecularIntegrationFileName, tSpecularIntegrationFilename, SpecularIntegrationFilename.c_str(), _TRUNCATE);

	// Load the texture in.
	result = DirectX::CreateDDSTextureFromFile(device, wcDiffuseProbeFilename, nullptr, &tempLight.DiffuseProbe);
	if (FAILED(result))
	{
		return false;
	}

	// Load the texture in.
	result = DirectX::CreateDDSTextureFromFile(device, wcSpecularProbeFileName, nullptr, &tempLight.SpecularProbe);
	if (FAILED(result))
	{
		return false;
	}

	// Load the texture in.
	result = DirectX::CreateDDSTextureFromFile(device, wcSpecularIntegrationFileName, nullptr, &tempLight.SpecularIntegration);
	if (FAILED(result))
	{
		return false;
	}

	ImageBasedLights.push_back(tempLight);

	return true;
}

//Set & Get functions for Directional Ambient Lights

void LightManager::SetDirectionalLightDirection(int LightIndexNumber, float dirX, float dirY, float dirZ)
{
	DirectionalLights[LightIndexNumber].AmbientLightDirection.x = dirX;
	DirectionalLights[LightIndexNumber].AmbientLightDirection.y = dirY;
	DirectionalLights[LightIndexNumber].AmbientLightDirection.z = dirZ;
}

void LightManager::SetDirectionalLightColor(int LightIndexNumber, float colorR, float colorG, float colorB)
{
	DirectionalLights[LightIndexNumber].AmbientLightColor.x = colorR;
	DirectionalLights[LightIndexNumber].AmbientLightColor.y = colorG;
	DirectionalLights[LightIndexNumber].AmbientLightColor.z = colorB;

}

void LightManager::SetDirectionalLightDown(int LightIndexNumber, float downColorR, float downColorG, float downColorB)
{
	DirectionalLights[LightIndexNumber].AmbientDown.x = downColorR;
	DirectionalLights[LightIndexNumber].AmbientDown.y = downColorG;
	DirectionalLights[LightIndexNumber].AmbientDown.z = downColorB;
}

void LightManager::SetDirectionalLightUp(int LightIndexNumber, float upX, float upY, float upZ)
{
	DirectionalLights[LightIndexNumber].AmbientUp.x = upX;
	DirectionalLights[LightIndexNumber].AmbientUp.y = upY;
	DirectionalLights[LightIndexNumber].AmbientUp.z = upZ;
}

DirectX::XMVECTOR LightManager::GetDirectionalLightDirection(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&DirectionalLights[LightIndexNumber].AmbientLightDirection);
}

DirectX::XMVECTOR LightManager::GetDirectionalLightColor(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&DirectionalLights[LightIndexNumber].AmbientLightColor);
}

DirectX::XMVECTOR LightManager::GetDirectionalLightDown(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&DirectionalLights[LightIndexNumber].AmbientDown);
}

DirectX::XMVECTOR LightManager::GetDirectionalLightUp(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&DirectionalLights[LightIndexNumber].AmbientUp);
}

DirectionalLightData LightManager::GetDirectionalLightData(int LightIndexNumber)
{
	return DirectionalLights[LightIndexNumber];
}

//Set & Get Functions for Point Lights

void LightManager::SetPointLightPosition(int LightIndexNumber, float posX, float posY, float posZ)
{
	PointLights[LightIndexNumber].PointLightPosition.x = posX;
	PointLights[LightIndexNumber].PointLightPosition.y = posY;
	PointLights[LightIndexNumber].PointLightPosition.z = posZ;
}

void LightManager::SetPointLightColor(int LightIndexNumber, float colorR, float colorG, float colorB)
{
	PointLights[LightIndexNumber].PointLightColor.x = colorR;
	PointLights[LightIndexNumber].PointLightColor.y = colorG;
	PointLights[LightIndexNumber].PointLightColor.z = colorB;
}

void LightManager::SetPointLightRange(int LightIndexNumber, float range)
{
	PointLights[LightIndexNumber].PointLightRange = range;
}

DirectX::XMVECTOR LightManager::GetPointLightPosition(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&PointLights[LightIndexNumber].PointLightPosition);
}

DirectX::XMVECTOR LightManager::GetPointLightColor(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&PointLights[LightIndexNumber].PointLightColor);
}

float LightManager::GetPointLightRange(int LightIndexNumber)
{
	return PointLights[LightIndexNumber].PointLightRange;
}

PointLightData LightManager::GetPointLightData(int LightIndexNumber)
{
	return PointLights[LightIndexNumber];
}

//Set & Get Functions for Spot Ligths

void LightManager::SetSpotLightPosition(int LightIndexNumber, float posX, float posY, float posZ)
{
	SpotLights[LightIndexNumber].SpotLightPosition.x = posX;
	SpotLights[LightIndexNumber].SpotLightPosition.y = posY;
	SpotLights[LightIndexNumber].SpotLightPosition.z = posZ;

}

void LightManager::SetSpotLightColor(int LightIndexNumber, float colorR, float colorG, float colorB)
{
	SpotLights[LightIndexNumber].SpotLightColor.x = colorR;
	SpotLights[LightIndexNumber].SpotLightColor.y = colorG;
	SpotLights[LightIndexNumber].SpotLightColor.z = colorB;
}

void LightManager::SetSpotLightDirection(int LightIndexNumber, float dirX, float dirY, float dirZ)
{
	SpotLights[LightIndexNumber].SpotLightDir.x = dirX;
	SpotLights[LightIndexNumber].SpotLightDir.y = dirY;
	SpotLights[LightIndexNumber].SpotLightDir.z = dirZ;
}

void LightManager::SetSpotLightRange(int LightIndexNumber, float range)
{
	SpotLights[LightIndexNumber].SpotLightRange = range;
}

void LightManager::SetSpotLightInnerAngel(int LightIndexNumber, float innerAngel)
{
	SpotLights[LightIndexNumber].SpotLightInnerAngel = innerAngel;
}

void LightManager::SetSpotLightOuterAngel(int LightIndexNumber, float outerAngel)
{
	SpotLights[LightIndexNumber].SpotLightOuterAngel = outerAngel;
}

DirectX::XMVECTOR LightManager::GetSpotLightPosition(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&SpotLights[LightIndexNumber].SpotLightPosition);
}

DirectX::XMVECTOR LightManager::GetSpotLightColor(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&SpotLights[LightIndexNumber].SpotLightColor);
}

DirectX::XMVECTOR LightManager::GetSpotLightDirection(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&SpotLights[LightIndexNumber].SpotLightDir);
}

float LightManager::GetSpotLightRange(int LightIndexNumber)
{
	return SpotLights[LightIndexNumber].SpotLightRange;
}

float LightManager::GetSpotLightInnerAngel(int LightIndexNumber)
{
	return SpotLights[LightIndexNumber].SpotLightInnerAngel;
}

float LightManager::GetSpotLightOuterAngel(int LightIndexNumber)
{
	return SpotLights[LightIndexNumber].SpotLightOuterAngel;
}

SpotLightData LightManager::GetSpotLightData(int LightIndexNumber)
{
	return SpotLights[LightIndexNumber];
}

//Set & Get Functions for Capsule Lights
void LightManager::SetCapsuleLightPosition(int LightIndexNumber, float posX, float posY, float posZ)
{
	CapsuleLights[LightIndexNumber].CapsuleLightPosition.x = posX;
	CapsuleLights[LightIndexNumber].CapsuleLightPosition.y = posY;
	CapsuleLights[LightIndexNumber].CapsuleLightPosition.z = posZ;
}

void LightManager::SetCapsuleLightColor(int LightIndexNumber, float colorR, float colorG, float colorB)
{
	CapsuleLights[LightIndexNumber].CapsuleLightColor.x = colorR;
	CapsuleLights[LightIndexNumber].CapsuleLightColor.y = colorG;
	CapsuleLights[LightIndexNumber].CapsuleLightColor.z = colorB;

}

void LightManager::SetCapsuleLightDirection(int LightIndexNumber, float dirX, float dirY, float dirZ)
{
	CapsuleLights[LightIndexNumber].CapsuleLightDir.x = dirX;
	CapsuleLights[LightIndexNumber].CapsuleLightDir.x = dirY;
	CapsuleLights[LightIndexNumber].CapsuleLightDir.x = dirZ;
}

void LightManager::SetCapsuleLightRange(int LightIndexNumber, float range)
{
	CapsuleLights[LightIndexNumber].CapsuleLightRange = range;
}

void LightManager::SetCapsuleLightLength(int LightIndexNumber, float length)
{
	CapsuleLights[LightIndexNumber].CapsuleLightLength = length;
}

DirectX::XMVECTOR LightManager::GetCapsuleLightPosition(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&CapsuleLights[LightIndexNumber].CapsuleLightPosition);
}

DirectX::XMVECTOR LightManager::GetCapsuleLightColor(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&CapsuleLights[LightIndexNumber].CapsuleLightColor);
}

DirectX::XMVECTOR LightManager::GetCapsuleLightDirection(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&CapsuleLights[LightIndexNumber].CapsuleLightDir);
}
float LightManager::GetCapsuleLightRange(int LightIndexNumber)
{
	return CapsuleLights[LightIndexNumber].CapsuleLightRange;
}
float LightManager::GetCapsuleLightLength(int LightIndexNumber)
{
	return CapsuleLights[LightIndexNumber].CapsuleLightLength;
}

CapsuleLightData LightManager::GetCapsuleLightData(int LightIndexNumber)
{
	return CapsuleLights[LightIndexNumber];
}

//Set & Get Functions for Image Based Lights

void LightManager::SetIBLPosition(int LightIndexNumber, float posX, float posY, float posZ)
{
	ImageBasedLights[LightIndexNumber].IBLPosition.x = posX;
	ImageBasedLights[LightIndexNumber].IBLPosition.y = posY;
	ImageBasedLights[LightIndexNumber].IBLPosition.z = posZ;
}

void LightManager::SetIBLRange(int LightIndexNumber, float range)
{
	ImageBasedLights[LightIndexNumber].IBLRange = range;
}

DirectX::XMVECTOR LightManager::GetIBLPosition(int LightIndexNumber)
{
	return DirectX::XMLoadFloat3(&ImageBasedLights[LightIndexNumber].IBLPosition);
}

float LightManager::GetIBLRange(int LightIndexNumber)
{
	return ImageBasedLights[LightIndexNumber].IBLRange;
}

ImageBasedLightData LightManager::GetIBLData(int LightIndexNumber)
{
	return ImageBasedLights[LightIndexNumber];
}

bool LightManager::SetIBLCubemaps(int LightIndexNumber, ID3D11Device* device, std::string DiffuseProbeFileName, std::string SpecularProbeFileName, std::string SpecularIntegrationFilename)
{
	bool result;

	//Convert the diffuse probe filename into wcstring
	size_t tDiffuseProbeFilename = DiffuseProbeFileName.length() + 1;
	wchar_t * wcDiffuseProbeFilename = new wchar_t[tDiffuseProbeFilename];

	size_t DiffuseProbeFilenameConvertedChars = 0;
	mbstowcs_s(&DiffuseProbeFilenameConvertedChars, wcDiffuseProbeFilename, tDiffuseProbeFilename, DiffuseProbeFileName.c_str(), _TRUNCATE);

	//Convert the specular probe filename into wcstring
	size_t tSpecularFilename = SpecularProbeFileName.length() + 1;
	wchar_t * wcSpecularProbeFileName = new wchar_t[tSpecularFilename];

	size_t SpecularFilenameConvertedChars = 0;
	mbstowcs_s(&SpecularFilenameConvertedChars, wcSpecularProbeFileName, tSpecularFilename, SpecularProbeFileName.c_str(), _TRUNCATE);

	//Convert the specular integration filename into wcstring
	size_t tSpecularIntegrationFilename = SpecularIntegrationFilename.length() + 1;
	wchar_t * wcSpecularIntegrationFileName = new wchar_t[tSpecularIntegrationFilename];

	size_t SpecularIntegrationFilenameConvertedChars = 0;
	mbstowcs_s(&SpecularIntegrationFilenameConvertedChars, wcSpecularIntegrationFileName, tSpecularIntegrationFilename, SpecularIntegrationFilename.c_str(), _TRUNCATE);

	// Load the texture in.
	result = DirectX::CreateDDSTextureFromFile(device, wcDiffuseProbeFilename, nullptr, &ImageBasedLights[LightIndexNumber].DiffuseProbe);
	if (FAILED(result))
	{
		return false;
	}

	// Load the texture in.
	result = DirectX::CreateDDSTextureFromFile(device, wcSpecularProbeFileName, nullptr, &ImageBasedLights[LightIndexNumber].SpecularProbe);
	if (FAILED(result))
	{
		return false;
	}

	// Load the texture in.
	result = DirectX::CreateDDSTextureFromFile(device, wcSpecularIntegrationFileName, nullptr, &ImageBasedLights[LightIndexNumber].SpecularIntegration);
	if (FAILED(result))
	{
		return false;
	}

	return true;
}