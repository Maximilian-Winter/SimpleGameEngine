//Light Manager Class
//Class to hold the Data of the Lights in the Scene

#ifndef _LIGHTMANAGER_H_
#define _LIGHTMANAGER_H_


//Includes
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <string>
#include "DDSUtil\DDSTextureLoader\DDSTextureLoader.h"


struct DirectionalLightData
{
	DirectionalLightData(float dirX, float dirY, float dirZ,
	float colorR, float colorG, float colorB, float downR, float downG, float downB,
	float upX, float upY, float upZ)
	:AmbientLightDirection(dirX, dirY, dirZ),
	AmbientLightColor(colorR, colorG, colorB),
	AmbientDown(downR, downG, downB),
	AmbientUp(upX, upY, upZ)
	{
	};

	DirectionalLightData(const DirectionalLightData& other)
	{
		*this = other;
	};

	DirectX::XMFLOAT3 AmbientLightDirection;
	DirectX::XMFLOAT3 AmbientLightColor;
	DirectX::XMFLOAT3 AmbientDown;
	DirectX::XMFLOAT3 AmbientUp;
};

struct PointLightData
{
	PointLightData(float posX, float posY, float posZ, float colorR, float colorG,
	float colorB, float range)
	:PointLightPosition(posX, posY, posZ),
	PointLightColor(colorR, colorG, colorB),
	PointLightRange(range)
	{
	};

	PointLightData(const PointLightData& other)
	{
		*this = other;
	};

	DirectX::XMFLOAT3 PointLightPosition;
	DirectX::XMFLOAT3 PointLightColor;
	float PointLightRange;
};

struct SpotLightData
{
	SpotLightData(float posX, float posY, float posZ, float colorR, float colorG,
	float colorB, float dirX, float dirY, float dirZ, float range, float innerAngel,
	float outerAngel)
	:SpotLightPosition(posX, posY, posZ),
	SpotLightColor(colorR, colorG, colorB),
	SpotLightDir(dirX, dirY, dirZ),
	SpotLightRange(range),
	SpotLightInnerAngel(innerAngel),
	SpotLightOuterAngel(outerAngel)
	{
	};

	SpotLightData(const SpotLightData& other)
	{
		*this = other;
	};

	DirectX::XMFLOAT3 SpotLightPosition;
	DirectX::XMFLOAT3 SpotLightColor;
	DirectX::XMFLOAT3 SpotLightDir;
	float SpotLightRange;
	float SpotLightInnerAngel;
	float SpotLightOuterAngel;
};

struct CapsuleLightData
{
	CapsuleLightData(float posX, float posY, float posZ, float colorR, float colorG,
	float colorB, float dirX, float dirY, float dirZ, float range, float length)
	:CapsuleLightPosition(posX, posY, posZ),
	CapsuleLightColor(colorR, colorG, colorB),
	CapsuleLightDir(dirX, dirY, dirZ),
	CapsuleLightLength(length),
	CapsuleLightRange(range)
	{
	};
	
	CapsuleLightData(const CapsuleLightData& other)
	{
		*this = other;
	};

	DirectX::XMFLOAT3 CapsuleLightPosition;
	DirectX::XMFLOAT3 CapsuleLightColor;
	DirectX::XMFLOAT3 CapsuleLightDir;
	float CapsuleLightLength;
	float CapsuleLightRange;
};

struct ImageBasedLightData
{
	ImageBasedLightData()
		:DiffuseProbe(0),
		SpecularProbe(0),
		SpecularIntegration(0),
		IBLPosition(0, 0, 0),
		IBLRange(0)
	{
	};

	ImageBasedLightData(ID3D11ShaderResourceView* diffuseProbe, ID3D11ShaderResourceView* specularProbe, ID3D11ShaderResourceView* specularIntegration,float posX, float posY, float posZ, float range)
		:DiffuseProbe(diffuseProbe),
		SpecularProbe(specularProbe),
		SpecularIntegration(specularIntegration),
		IBLPosition(posX, posY, posZ),
		IBLRange(range)
	{
	};

	ImageBasedLightData(const ImageBasedLightData& other)
	{
		*this = other;
	};
	
	ID3D11ShaderResourceView* DiffuseProbe;
	ID3D11ShaderResourceView* SpecularProbe;

	ID3D11ShaderResourceView* SpecularIntegration;

	DirectX::XMFLOAT3 IBLPosition;
	float IBLRange;
};

class LightManager
{
	

public:
	LightManager();
	LightManager(const LightManager&);
	~LightManager();

	//Get number of lights
	int GetNumberOfDirectionalLights();
	int GetNumberOfPointLights();
	int GetNumberOfSpotLights();
	int GetNumberOfCapsuleLights();
	int GetNumberOfImageBasedLights();


	//Add new Lights functions
	void AddDirectionalLight(float , float , float , float , float , float , float , float , float ,float ,float , float );
	void AddDirectionalLight(DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::CXMVECTOR);

	void AddPointLight(float, float, float, float, float, float, float);
	void AddPointLight(DirectX::FXMVECTOR, DirectX::FXMVECTOR, float);

	void AddSpotLight(float, float, float, float, float, float, float, float, float, float, float, float);
	void AddSpotLight(DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, float, float, float);

	void AddCapsuleLight(float, float, float, float, float, float, float, float, float, float, float);
	void AddCapsuleLight(DirectX::FXMVECTOR, DirectX::FXMVECTOR, DirectX::FXMVECTOR, float, float);

	bool AddImageBasedLight(ID3D11Device*, std::string, std::string, std::string, DirectX::FXMVECTOR, float);

	//Set & Get functions for Directional Ambient Lights
	void SetDirectionalLightDirection(int, float, float, float);
	void SetDirectionalLightColor(int, float, float, float);
	void SetDirectionalLightDown(int, float, float, float);
	void SetDirectionalLightUp(int, float, float, float);

	DirectX::XMVECTOR GetDirectionalLightDirection(int);
	DirectX::XMVECTOR GetDirectionalLightColor(int);
	DirectX::XMVECTOR GetDirectionalLightDown(int);
	DirectX::XMVECTOR GetDirectionalLightUp(int);
	DirectionalLightData GetDirectionalLightData(int);

	//Set & Get Functions for Point Lights
	void SetPointLightPosition(int, float, float, float);
	void SetPointLightColor(int, float, float, float);
	void SetPointLightRange(int, float);

	DirectX::XMVECTOR GetPointLightPosition(int);
	DirectX::XMVECTOR GetPointLightColor(int);
	float GetPointLightRange(int);
	PointLightData GetPointLightData(int);

	//Set & Get Functions for Spot Ligths
	void SetSpotLightPosition(int, float, float, float);
	void SetSpotLightColor(int, float, float, float);
	void SetSpotLightDirection(int, float, float, float);
	void SetSpotLightRange(int, float);
	void SetSpotLightInnerAngel(int, float);
	void SetSpotLightOuterAngel(int, float);

	DirectX::XMVECTOR GetSpotLightPosition(int);
	DirectX::XMVECTOR GetSpotLightColor(int);
	DirectX::XMVECTOR GetSpotLightDirection(int);
	float GetSpotLightRange(int);
	float GetSpotLightInnerAngel(int);
	float GetSpotLightOuterAngel(int);
	SpotLightData GetSpotLightData(int);

	//Set & Get Functions for Capsule Lights
	void SetCapsuleLightPosition(int, float, float, float);
	void SetCapsuleLightColor(int, float, float, float);
	void SetCapsuleLightDirection(int, float, float, float);
	void SetCapsuleLightRange(int, float);
	void SetCapsuleLightLength(int, float);

	DirectX::XMVECTOR GetCapsuleLightPosition(int);
	DirectX::XMVECTOR GetCapsuleLightColor(int);
	DirectX::XMVECTOR GetCapsuleLightDirection(int);
	float GetCapsuleLightRange(int);
	float GetCapsuleLightLength(int);
	CapsuleLightData GetCapsuleLightData(int);


	//Set & Get Functions for IBL Lights
	void SetIBLPosition(int, float, float, float);
	bool SetIBLCubemaps(int, ID3D11Device*, std::string, std::string, std::string);
	void SetIBLRange(int, float);

	DirectX::XMVECTOR GetIBLPosition(int);
	float GetIBLRange(int);
	ImageBasedLightData GetIBLData(int);

private:

	std::vector<DirectionalLightData> DirectionalLights;
	std::vector<PointLightData> PointLights;
	std::vector<SpotLightData> SpotLights;
	std::vector<CapsuleLightData> CapsuleLights;
	std::vector<ImageBasedLightData> ImageBasedLights;

};

#endif