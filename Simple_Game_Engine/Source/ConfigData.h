// Filename: ConfigData.h
// Date: 07.06.2015
// Author: Maximilian Winter
#ifndef _CONFIGDATA_H_
#define _CONFIGDATA_H_

#include "ConfigDataParser.h"
#include "ConfigDataSection.h"

class ConfigData
{
public:

	ConfigData();
	ConfigData(const ConfigData&);
	~ConfigData();

	bool LoadConfigDataFile(const std::string FileName);
	void SaveConfigDataFile(const std::string FileName, const std::string CommentAtStartOfFile);
	
	void ClearConfigData();

	//Add Value Functions
	bool AddIntValueByName(const std::string SectionName, const std::string ValueName, int Value);
	bool AddFloatValueByName(const std::string SectionName, const std::string ValueName, float Value);
	bool AddBoolValueByName(const std::string SectionName, const std::string ValueName, bool Value);
	bool AddStringValueByName(const std::string SectionName, const std::string ValueName, std::string Value);

	//Add Vector Functions
	bool AddIntVectorByName(const std::string SectionName, const std::string ValueName, std::vector<int> Value);
	bool AddFloatVectorByName(const std::string SectionName, const std::string ValueName, std::vector<float> Value);
	bool AddBoolVectorByName(const std::string SectionName, const std::string ValueName, std::vector<bool> Value);
	bool AddStringVectorByName(const std::string SectionName, const std::string ValueName, std::vector<std::string> Value);

	//Get Section info's
	int GetNumberOfRootSections();
	std::vector<int> GetIndiciePathToRootSecton(int RootSectionIndex);
	int GetNumberOfAllChildSectionsFromRoot(const int RootSectionIndex);
	std::vector<int> GetIndiciePathToChildFromRoot(const int RootSectionIndex, const int ChildIndex);

	bool GetSectionName(const std::vector<int> Indicies, std::string& SectionName);

	//Get count of section values
	int GetNumberOfIntValues(std::vector<int>& Indicies);
	int GetNumberOfIntVectors(std::vector<int>& Indicies);
	int GetNumberOfFloatValues(std::vector<int>& Indicies);
	int GetNumberOfFloatVectors(std::vector<int>& Indicies);
	int GetNumberOfBoolValues(std::vector<int>& Indicies);
	int GetNumberOfBoolVectors(std::vector<int>& Indicies);
	int GetNumberOfStringValues(std::vector<int>& Indicies);
	int GetNumberOfStringVectors(std::vector<int>& Indicies);
	
	//Get section values
	bool GetIntValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetIntValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, int& Value);
	bool GetIntValueByName(const std::string SectionName, const std::string ValueName, int& Value);

	bool GetFloatValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetFloatValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, float& Value);
	bool GetFloatValueByName(const std::string SectionName, const std::string ValueName, float& Value);

	bool GetBoolValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetBoolValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, bool& Value);
	bool GetBoolValueByName(const std::string SectionName, const std::string ValueName, bool& Value);

	bool GetStringValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetStringValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& Value);
	bool GetStringValueByName(const std::string SectionName, const std::string ValueName, std::string& Value);


	//Get section vectors
	bool GetIntVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string &ValueName);
	bool GetIntVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<int> &Value);
	bool GetIntVectorByName(const std::string SectionName, const std::string ValueName, std::vector<int> &Value);

	bool GetFloatVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string &ValueName);
	bool GetFloatVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<float> &Value);
	bool GetFloatVectorByName(const std::string SectionName, const std::string ValueName, std::vector<float> &Value);

	bool GetBoolVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetBoolVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<bool> &Value);
	bool GetBoolVectorByName(const std::string SectionName, const std::string ValueName, std::vector<bool> &Value);

	bool GetStringVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetStringVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<std::string> &Value);
	bool GetStringVectorByName(const std::string SectionName, const std::string ValueName, std::vector<std::string> &Value);

private:

	// Add section by section path list to the tree.
	bool AddSection(std::vector<std::string> SectionPathList, ConfigDataSection*& AddedSection);

	bool GetRootSectionByName(std::string SectionName, ConfigDataSection*& RootSection);
	bool GetRootSectionIndex(std::string SectionName, int& RootSectionIndex);

	bool GetChildSectionFromParent(ConfigDataSection* ParentSection, std::string SectionName, ConfigDataSection*& ChildSection);
	bool GetChildBySectionPath(std::vector<std::string> SectionPath, ConfigDataSection*& ChildSection);

	void GetChildSectionIndicieVector(ConfigDataSection* RootSection, const int ChildIndex, std::vector<int>& Indicies, int& ChildCount);
	void GetChildByIndex(ConfigDataSection* RootSection, int Index, ConfigDataSection*& ChildSection);
	bool GetChildByIndicieVector(const std::vector<int>& Indicies, ConfigDataSection*& ChildSection);

	bool CheckIfRootSectionExist(std::string SectionName);
	bool CheckIfChildSectionExist(ConfigDataSection* ParentSection, std::string SectionName);
	bool CheckIfSectionPathExist(std::vector<std::string> SectionPath);

	void CalculateNumberOfAllChildsFromRoot(ConfigDataSection* RootSection, int& NumberOfChildSections);
	void PrintSectionToFile(ConfigDataSection* SectionData, std::ofstream& OutputFile);
private:
	struct Config
	{
		std::vector<ConfigDataSection> RootSections;
	};
	Config configData;
};

#endif