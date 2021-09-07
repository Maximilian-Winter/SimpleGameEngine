// Filename: DataFileContainer.h
// Date: 07.06.2015
// Author: Maximilian Winter
#ifndef _DATAFILECONTAINER_H_
#define _DATAFILECONTAINER_H_

#include "DataContainerParser.h"
#include "DataContainerSection.h"

#include <fstream>


class DataFileContainer
{
public:

	DataFileContainer();
	DataFileContainer(const DataFileContainer&);
	~DataFileContainer();

	bool LoadDataContainerFile(const std::string FileName);
	void SaveDataContainerToFile(const std::string FileName, const std::string CommentAtStartOfFile);

	bool LoadDataContainerBinaryFile(const std::string FileName);
	bool SaveDataContainerToBinaryFile(const std::string FileName);

	bool LoadValueSavingInformation(const std::string FileName);
	bool SaveValueSavingInformation(const std::string FileName);
	
	void ClearDataContainer();

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
	bool GetBinarySavingValueInformationFromIntValueByName(const std::string SectionName, const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value);

	bool GetFloatValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetFloatValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, float& Value);
	bool GetFloatValueByName(const std::string SectionName, const std::string ValueName, float& Value);
	bool GetBinarySavingValueInformationFromFloatValueByName(const std::string SectionName, const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value);

	bool GetBoolValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetBoolValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, bool& Value);
	bool GetBoolValueByName(const std::string SectionName, const std::string ValueName, bool& Value);
	bool GetBinarySavingValueInformationFromBoolValueByName(const std::string SectionName, const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value);

	bool GetStringValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetStringValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& Value);
	bool GetStringValueByName(const std::string SectionName, const std::string ValueName, std::string& Value);
	bool GetBinarySavingValueInformationFromStringValueByName(const std::string SectionName, const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value);


	//Get section vectors
	bool GetIntVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string &ValueName);
	bool GetIntVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<int> &Value);
	bool GetIntVectorByName(const std::string SectionName, const std::string ValueName, std::vector<int> &Value);
	bool GetBinarySavingValueInformationFromIntVectorByName(const std::string SectionName, const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value);

	bool GetFloatVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string &ValueName);
	bool GetFloatVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<float> &Value);
	bool GetFloatVectorByName(const std::string SectionName, const std::string ValueName, std::vector<float> &Value);
	bool GetBinarySavingValueInformationFromFloatVectorByName(const std::string SectionName, const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value);

	bool GetBoolVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetBoolVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<bool> &Value);
	bool GetBoolVectorByName(const std::string SectionName, const std::string ValueName, std::vector<bool> &Value);
	bool GetBinarySavingValueInformationFromBoolVectorByName(const std::string SectionName, const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value);

	bool GetStringVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName);
	bool GetStringVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<std::string> &Value);
	bool GetStringVectorByName(const std::string SectionName, const std::string ValueName, std::vector<std::string> &Value);
	bool GetBinarySavingValueInformationFromStringVectorByName(const std::string SectionName, const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value);

	struct Data
	{
		std::vector<DataContainerSection> RootSections;
	};
	Data m_Data;

private:

	// Add section by section path list to the tree.
	bool AddSection(std::vector<std::string> SectionPathList, DataContainerSection*& AddedSection);

	bool GetRootSectionByName(std::string SectionName, DataContainerSection*& RootSection);
	bool GetRootSectionIndex(std::string SectionName, int& RootSectionIndex);

	bool GetChildSectionFromParent(DataContainerSection* ParentSection, std::string SectionName, DataContainerSection*& ChildSection);
	bool GetChildBySectionPath(std::vector<std::string> SectionPath, DataContainerSection*& ChildSection);

	void GetChildSectionIndicieVector(DataContainerSection* RootSection, const int ChildIndex, std::vector<int>& Indicies, int& ChildCount);
	void GetChildByIndex(DataContainerSection* RootSection, int Index, DataContainerSection*& ChildSection);
	bool GetChildByIndicieVector(const std::vector<int>& Indicies, DataContainerSection*& ChildSection);

	bool CheckIfRootSectionExist(std::string SectionName);
	bool CheckIfChildSectionExist(DataContainerSection* ParentSection, std::string SectionName);
	bool CheckIfSectionPathExist(std::vector<std::string> SectionPath);

	void CalculateNumberOfAllChildsFromRoot(DataContainerSection* RootSection, int& NumberOfChildSections);
	void PrintSectionToFile(DataContainerSection* SectionData, std::ofstream& OutputFile);
	


};

#endif