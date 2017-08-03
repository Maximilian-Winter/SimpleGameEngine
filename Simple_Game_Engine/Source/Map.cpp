#include "Map.h"


Map::Map()
{
	m_ObjectsCount=0;
	m_Map = 0;
}

Map::Map(const Map& other)
{
}

bool Map::Initialize(char* filename)
{
	

	std::ifstream fin;
	char input;
	int i;


	// Open the model file.
	fin.open(filename);

	// If it could not open the file then exit.
	if(fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while(input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> m_ObjectsCount;

	// Create the model using the vertex count that was read in.
	m_Map = new MapType[m_ObjectsCount];
	if(!m_Map)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	

	// Read in the vertex data.
	for(i=0; i<m_ObjectsCount; i++)
	{
		fin >> m_Map[i].objectfilename;
		fin >> m_Map[i].posx >> m_Map[i].posy >> m_Map[i].posz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void Map::Shutdown()
{

	if(m_Map)
	{
		delete [] m_Map;
		m_Map = 0;
		m_ObjectsCount=0;
	}
}


std::string Map::GetObjectName(int index)
{
	return m_Map[index].objectfilename;
}

DirectX::XMFLOAT3 Map::GetObjectMapData(int index)
{
	return DirectX::XMFLOAT3(m_Map[index].posx,m_Map[index].posy,m_Map[index].posz);
}

int	Map::GetObjectsCount()
{
	return m_ObjectsCount;
}

Map::~Map()
{
}
