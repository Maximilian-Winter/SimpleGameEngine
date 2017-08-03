#ifndef _MAP_H_
#define _MAP_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <iostream>
#include <string>

#pragma once
class Map
{

private:

	struct MapType
	{
		std::string objectfilename;
		float posx, posy, posz;
	};
public:
	Map();
	Map(const Map&);
	~Map();

	
	bool Initialize(char*);
	void Shutdown();

	DirectX::XMFLOAT3 GetObjectMapData(int index);
	std::string GetObjectName(int index);
	int	GetObjectsCount();
	MapType* m_Map;

private:
	int m_ObjectsCount;
};

#endif
