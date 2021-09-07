#include "DataFileContainer.h"


DataFileContainer::DataFileContainer()
{
	m_Data.RootSections.clear();
}


DataFileContainer::DataFileContainer(const DataFileContainer& other)
{
}


DataFileContainer::~DataFileContainer()
{
}


bool DataFileContainer::LoadDataContainerFile(const std::string FileName)
{
	//Clear the vector with the Data Data
	m_Data.RootSections.clear();
	std::ifstream InputFile(FileName);
	std::string InputStr;

	if (InputFile.good())
	{
		DataContainerSection* CurrentSection = nullptr;
		bool HasSections = false;

		while (!InputFile.eof())
		{
			//Start to read in the Data from the File
			std::getline(InputFile, InputStr);
			DataContainerParser::RemoveCommentFromLine(InputStr);

			if (!DataContainerParser::IsOnlyWhitespacesLine(InputStr))
			{
				if (DataContainerParser::IsSectionNameLine(InputStr))
				{
					std::string SectionName = DataContainerParser::GetSectionNameFromLine(InputStr);

					if (CheckIfRootSectionExist(SectionName))
					{
						GetRootSectionByName(SectionName, CurrentSection);
					}
					else
					{
						AddSection(DataContainerParser::GetChildSectionNameListFromLine(InputStr), CurrentSection);
					}
					
					HasSections = true;
				}

				if (DataContainerParser::IsChildSectionNameLine(InputStr))
				{
					
					if (CheckIfSectionPathExist(DataContainerParser::GetChildSectionNameListFromLine(InputStr)))
					{
						GetChildBySectionPath(DataContainerParser::GetChildSectionNameListFromLine(InputStr), CurrentSection);
					}
					else
					{
						AddSection(DataContainerParser::GetChildSectionNameListFromLine(InputStr), CurrentSection);
					}
					HasSections = true;
				}

				if (DataContainerParser::IsValueLine(InputStr))
				{
					std::string ValueName = DataContainerParser::GetValueNameFromLine(InputStr);
					if(CurrentSection != nullptr)
					{
						if (DataContainerParser::IsIntValueLine(InputStr))
						{
							int Value = DataContainerParser::GetIntValueFromLine(InputStr);
							
							CurrentSection->AddIntVal(ValueName, Value);
						}

						if (DataContainerParser::IsFloatValueLine(InputStr))
						{
							float Value = DataContainerParser::GetFloatValueFromLine(InputStr);
						
							CurrentSection->AddFloatVal(ValueName, Value);
						}

						if (DataContainerParser::IsBoolValueLine(InputStr))
						{
							bool Value = DataContainerParser::GetBoolValueFromLine(InputStr);
						
							CurrentSection->AddBoolVal(ValueName, Value);
						}

						if (DataContainerParser::IsStringValueLine(InputStr))
						{
							std::string Value = DataContainerParser::GetStringValueFromLine(InputStr);
						
							CurrentSection->AddStringVal(ValueName, Value);
						}

						if (DataContainerParser::IsIntVectorLine(InputStr))
						{
							std::vector<int> Value = DataContainerParser::GetIntVectorFromLine(InputStr);

							CurrentSection->AddIntVector(ValueName, Value);
						}

						if (DataContainerParser::IsFloatVectorLine(InputStr))
						{
							std::vector<float> Value = DataContainerParser::GetFloatVectorFromLine(InputStr);

							CurrentSection->AddFloatVector(ValueName, Value);
						}
					
						if (DataContainerParser::IsBoolVectorLine(InputStr))
						{
							std::vector<bool> Value = DataContainerParser::GetBoolVectorFromLine(InputStr);

							CurrentSection->AddBoolVector(ValueName, Value);
						}

						if (DataContainerParser::IsStringVectorLine(InputStr))
						{
							std::vector<std::string> Value = DataContainerParser::GetStringVectorFromLine(InputStr);

							CurrentSection->AddStringVector(ValueName, Value);
						}
					}
				}

			}
		}
	}
	else
	{
		return false;
	}

	return true;
}


void DataFileContainer::SaveDataContainerToFile(const std::string FileName, const std::string CommentAtStartOfFile)
{
	std::ofstream fileOutput;
	fileOutput.open(FileName);

	fileOutput << "#" <<CommentAtStartOfFile << std::endl;
	fileOutput << std::endl;
	for (int k = 0; k < m_Data.RootSections.size(); k++)
	{
		PrintSectionToFile(&m_Data.RootSections[k], fileOutput);
	}
	fileOutput.close();
}

bool DataFileContainer::LoadDataContainerBinaryFile(const std::string FileName)
{
	std::ifstream myFile(FileName, std::ios::in | std::ios::binary);

	if (!myFile)
	{
		return false;
	}
	size_t size;
	myFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	m_Data.RootSections.resize(size);
	for (int k = 0; k < m_Data.RootSections.size(); k++)
	{
		m_Data.RootSections[k].LoadFromBinaryInputStream(myFile);
	}
	
	myFile.close();

	return true;
}

bool DataFileContainer::SaveDataContainerToBinaryFile(const std::string FileName)
{
	std::ofstream myFile(FileName, std::ios::out | std::ios::binary);

	if (!myFile)
	{
		return false;
	}

	size_t size = m_Data.RootSections.size();
	myFile.write(reinterpret_cast<char*>(&size), sizeof(size));

	for (int k = 0; k < m_Data.RootSections.size(); k++)
	{
		m_Data.RootSections[k].SaveToBinaryOutputStream(myFile);
	}

	myFile.close();

	return true;
}

bool DataFileContainer::LoadValueSavingInformation(const std::string FileName)
{
	std::ifstream myFile(FileName, std::ios::in | std::ios::binary);

	if (!myFile)
	{
		return false;
	}
	size_t size;
	myFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	for (int k = 0; k < m_Data.RootSections.size(); k++)
	{
		m_Data.RootSections[k].LoadValueSavingInformationToBinaryOutputStream(myFile);
	}

	myFile.close();

	return true;
}

bool DataFileContainer::SaveValueSavingInformation(const std::string FileName)
{
	std::ofstream myFile(FileName, std::ios::out | std::ios::binary);

	if (!myFile)
	{
		return false;
	}

	size_t size = m_Data.RootSections.size();
	myFile.write(reinterpret_cast<char*>(&size), sizeof(size));

	for (int k = 0; k < m_Data.RootSections.size(); k++)
	{
		m_Data.RootSections[k].SaveValueSavingInformationToBinaryOutputStream(myFile);
	}

	myFile.close();

	return true;
}


void DataFileContainer::ClearDataContainer()
{
	m_Data.RootSections.clear();
}


bool DataFileContainer::AddIntValueByName(const std::string SectionName, const std::string ValueName, int Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddIntVal(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::AddFloatValueByName(const std::string SectionName, const std::string ValueName, float Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddFloatVal(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::AddBoolValueByName(const std::string SectionName, const std::string ValueName, bool Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddBoolVal(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::AddStringValueByName(const std::string SectionName, const std::string ValueName, std::string Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddStringVal(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::AddIntVectorByName(const std::string SectionName, const std::string ValueName, std::vector<int> Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddIntVector(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::AddFloatVectorByName(const std::string SectionName, const std::string ValueName, std::vector<float> Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddFloatVector(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::AddBoolVectorByName(const std::string SectionName, const std::string ValueName, std::vector<bool> Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddBoolVector(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::AddStringVectorByName(const std::string SectionName, const std::string ValueName, std::vector<std::string> Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddStringVector(ValueName, Value);
	}
	else
	{
		return false;
	}
}


int DataFileContainer::GetNumberOfRootSections()
{
	return m_Data.RootSections.size();
}


std::vector<int> DataFileContainer::GetIndiciePathToRootSecton(int RootSectionIndex)
{
	std::vector<int> Indicies;
	Indicies.push_back(RootSectionIndex);

	return Indicies;
}


int DataFileContainer::GetNumberOfAllChildSectionsFromRoot(const int RootSectionIndex)
{
	DataContainerSection* Root = &m_Data.RootSections[RootSectionIndex];

	int NumberOfChilds = 0;

	CalculateNumberOfAllChildsFromRoot(Root, NumberOfChilds);

	return NumberOfChilds;
}


bool DataFileContainer::GetSectionName(const std::vector<int> Indicies, std::string& SectionName)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		SectionName = TempSection->SectionName;
		return true;
	}
	else
	{
		return false;
	}
}


std::vector<int> DataFileContainer::GetIndiciePathToChildFromRoot(const int RootSectionIndex, const int ChildIndex)
{
	std::vector<int> Indicies;

	DataContainerSection* Root = &m_Data.RootSections[RootSectionIndex];
	
	int TempChildCount = 0;

	GetChildSectionIndicieVector(Root, ChildIndex, Indicies, TempChildCount);

	return Indicies;
}


int DataFileContainer::GetNumberOfIntValues(std::vector<int>&  Indicies)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfIntVals();
	}
	else
	{
		return 0;
	}
}


int DataFileContainer::GetNumberOfIntVectors(std::vector<int>&  Indicies)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfIntVectors();
	}
	else
	{
		return 0;
	}
}


int DataFileContainer::GetNumberOfFloatValues(std::vector<int>&  Indicies)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfFloatVals();
	}
	else
	{
		return 0;
	}
}


int DataFileContainer::GetNumberOfFloatVectors(std::vector<int>&  Indicies)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfFloatVectors();
	}
	else
	{
		return 0;
	}
}


int DataFileContainer::GetNumberOfBoolValues(std::vector<int>&  Indicies)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfBoolVals();
	}
	else
	{
		return 0;
	}
}


int DataFileContainer::GetNumberOfBoolVectors(std::vector<int>&  Indicies)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfBoolVectors();
	}
	else
	{
		return 0;
	}

}


int DataFileContainer::GetNumberOfStringValues(std::vector<int>&  Indicies)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfStringVals();
	}
	else
	{
		return 0;
	}
}


int DataFileContainer::GetNumberOfStringVectors(std::vector<int>&  Indicies)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfStringVectors();
	}
	else
	{
		return 0;
	}
}


bool DataFileContainer::GetIntValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetIntValName(ValueIndex, ValueName);
	}
	else
	{
		return false;

	}
}


bool DataFileContainer::GetIntValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, int& Value)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetIntVal(ValueIndex, Value);
	}
	else
	{
		return false;

	}
}


bool DataFileContainer::GetIntValueByName(const std::string SectionName, const std::string ValueName, int& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetIntVal(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool DataFileContainer::GetBinarySavingValueInformationFromIntValueByName(const std::string SectionName,
	const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetIntValSavingValueInformation(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetIntVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetIntVectorName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetIntVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<int>& Value)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetIntVector(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetIntVectorByName(const std::string SectionName, const std::string ValueName, std::vector<int> &Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetIntVector(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool DataFileContainer::GetBinarySavingValueInformationFromIntVectorByName(const std::string SectionName,
	const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetIntVectorSavingValueInformation(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetFloatValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetFloatValName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetFloatValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, float& Value)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetFloatVal(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetFloatValueByName(const std::string SectionName, const std::string ValueName, float& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetFloatVal(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool DataFileContainer::GetBinarySavingValueInformationFromFloatValueByName(const std::string SectionName,
	const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetFloatValSavingValueInformation(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetFloatVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetFloatVectorName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetFloatVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<float>& Value)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetFloatVector(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetFloatVectorByName(const std::string SectionName, const std::string ValueName, std::vector<float> &Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetFloatVector(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool DataFileContainer::GetBinarySavingValueInformationFromFloatVectorByName(const std::string SectionName,
	const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetFloatVectorSavingValueInformation(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetBoolValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetBoolValName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetBoolValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, bool& Value)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetBoolVal(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetBoolValueByName(const std::string SectionName, const std::string ValueName, bool& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetBoolVal(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool DataFileContainer::GetBinarySavingValueInformationFromBoolValueByName(const std::string SectionName,
	const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetBoolValSavingValueInformation(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetBoolVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetBoolVectorName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetBoolVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<bool> &Value)
{

	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetBoolVector(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetBoolVectorByName(const std::string SectionName, const std::string ValueName, std::vector<bool> &Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetBoolVector(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool DataFileContainer::GetBinarySavingValueInformationFromBoolVectorByName(const std::string SectionName,
	const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetBoolVectorSavingValueInformation(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetStringValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetStringValName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetStringValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& Value)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetStringVal(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetStringValueByName(const std::string SectionName, const std::string ValueName, std::string& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetStringVal(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool DataFileContainer::GetBinarySavingValueInformationFromStringValueByName(const std::string SectionName,
	const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetStringValSavingValueInformation(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetStringVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetStringVectorName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetStringVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<std::string> &Value)
{
	DataContainerSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetStringVector(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::GetStringVectorByName(const std::string SectionName, const std::string ValueName, std::vector<std::string> &Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetStringVector(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}

bool DataFileContainer::GetBinarySavingValueInformationFromStringVectorByName(const std::string SectionName,
	const std::string ValueName, DataContainerSection::BinarySavingValueInformation& Value)
{
	DataContainerSection* TempSection;

	if (DataContainerParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(DataContainerParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
		{
			return TempSection->GetStringVectorSavingValueInformation(ValueName, Value);
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
}


bool DataFileContainer::AddSection(std::vector<std::string> SectionPathList, DataContainerSection*& AddedSection)
{
	DataContainerSection* TempSection;

	if (GetRootSectionByName(SectionPathList[0], TempSection))
	{
		DataContainerSection* ChildTempSection = TempSection;

		std::vector<int> TempIndicies;

		int RootIndex;
		GetRootSectionIndex(SectionPathList[0], RootIndex);

		TempIndicies.push_back(RootIndex);

		for (int SectionNameListIndex = 1; SectionNameListIndex < SectionPathList.size(); SectionNameListIndex++)
		{
			DataContainerSection* TempSection = nullptr;
			if (CheckIfChildSectionExist(ChildTempSection, SectionPathList[SectionNameListIndex]))
			{
				GetChildSectionFromParent(ChildTempSection, SectionPathList[SectionNameListIndex], TempSection);
				ChildTempSection = TempSection;

				TempIndicies = ChildTempSection->SectionIndiciePath;
			}
			else
			{
				DataContainerSection Temp(SectionPathList[SectionNameListIndex]);

				TempIndicies.push_back(ChildTempSection->ChildSections.size());

				for (int k = 0; k <= SectionNameListIndex; k++)
				{
					Temp.SectionPath.push_back(SectionPathList[k]);
				}

				for (int k = 0; k < TempIndicies.size(); k++)
				{
					Temp.SectionIndiciePath.push_back(TempIndicies[k]);
				}

				ChildTempSection->ChildSections.push_back(Temp);

				ChildTempSection = &ChildTempSection->ChildSections[ChildTempSection->ChildSections.size() - 1];

			}
		}

		AddedSection = ChildTempSection;
	}
	else
	{
		m_Data.RootSections.push_back(DataContainerSection(SectionPathList[0]));

		DataContainerSection* ChildTempSection = &m_Data.RootSections[m_Data.RootSections.size() - 1];
		std::vector<int> TempIndicies;
		int RootIndex;
		ChildTempSection->SectionPath.push_back(SectionPathList[0]);

		GetRootSectionIndex(SectionPathList[0], RootIndex);

		TempIndicies.push_back(RootIndex);
		ChildTempSection->SectionIndiciePath = TempIndicies;

		for (int SectionNameListIndex = 1; SectionNameListIndex < SectionPathList.size(); SectionNameListIndex++)
		{
			DataContainerSection* TempSection = nullptr;
			if (CheckIfChildSectionExist(ChildTempSection, SectionPathList[SectionNameListIndex]))
			{
				GetChildSectionFromParent(ChildTempSection, SectionPathList[SectionNameListIndex], TempSection);
				ChildTempSection = TempSection;

				TempIndicies = ChildTempSection->SectionIndiciePath;
			}
			else
			{
				DataContainerSection Temp(SectionPathList[SectionNameListIndex]);

				TempIndicies.push_back(ChildTempSection->ChildSections.size());

				for (int k = 0; k < SectionNameListIndex; k++)
				{
					Temp.SectionPath.push_back(SectionPathList[k]);
				}

				for (int k = 0; k < TempIndicies.size(); k++)
				{
					Temp.SectionIndiciePath.push_back(TempIndicies[k]);
				}

				ChildTempSection->ChildSections.push_back(Temp);

				ChildTempSection = &ChildTempSection->ChildSections[ChildTempSection->ChildSections.size() - 1];

			}
		}
		AddedSection = ChildTempSection;
	}

	return true;
}


bool DataFileContainer::GetRootSectionByName(std::string SectionName, DataContainerSection*& RootSection)
{
	RootSection = nullptr;

	auto it = std::find_if(m_Data.RootSections.begin(), m_Data.RootSections.end(), [&SectionName](const DataContainerSection& obj) {return obj.SectionName == SectionName; });

	if (it != m_Data.RootSections.end())
	{
		auto index = std::distance(m_Data.RootSections.begin(), it);
		RootSection = &m_Data.RootSections[index];

		return true;
	}

	return false;
}


bool DataFileContainer::GetRootSectionIndex(std::string SectionName, int& RootSectionIndex)
{

	auto it = std::find_if(m_Data.RootSections.begin(), m_Data.RootSections.end(), [&SectionName](const DataContainerSection& obj) {return obj.SectionName == SectionName; });

	if (it != m_Data.RootSections.end())
	{
		auto index = std::distance(m_Data.RootSections.begin(), it);
		RootSectionIndex = index;

		return true;
	}

	return false;

	int SectionIndex = -1;
	for (int i = 0; i < m_Data.RootSections.size(); i++)
	{
		if (m_Data.RootSections[i].SectionName == SectionName)
		{
			SectionIndex = i;
		}
	}
	if (SectionIndex == -1)
	{
		return false;
	}

	RootSectionIndex = SectionIndex;

	return true;
}


bool DataFileContainer::GetChildSectionFromParent(DataContainerSection* ParentSection, std::string SectionName, DataContainerSection*& ChildSection)
{
	ChildSection = nullptr;

	auto it = std::find_if(ParentSection->ChildSections.begin(), ParentSection->ChildSections.end(), [&SectionName](const DataContainerSection& obj) {return obj.SectionName == SectionName; });

	if (it != ParentSection->ChildSections.end())
	{
		auto index = std::distance(ParentSection->ChildSections.begin(), it);
		ChildSection = &ParentSection->ChildSections[index];

		return true;
	}

	return false;
}


void DataFileContainer::GetChildByIndex(DataContainerSection* RootSection, int Index, DataContainerSection*& ChildSection)
{
	ChildSection = &RootSection->ChildSections[Index];
}


bool DataFileContainer::GetChildBySectionPath(std::vector<std::string> SectionPath, DataContainerSection*& ChildSection)
{
	DataContainerSection* RootTempSection;

	if (GetRootSectionByName(SectionPath[0], RootTempSection))
	{
		DataContainerSection* ChildTempSection = RootTempSection;

		if (SectionPath.size() == 1)
		{
			ChildSection = RootTempSection;
			return true;
		}

		for (int SectionNameListIndex = 1; SectionNameListIndex < SectionPath.size(); SectionNameListIndex++)
		{
			DataContainerSection* TempSection = nullptr;
			if (CheckIfChildSectionExist(ChildTempSection, SectionPath[SectionNameListIndex]))
			{
				GetChildSectionFromParent(ChildTempSection, SectionPath[SectionNameListIndex], TempSection);
				ChildTempSection = TempSection;

				ChildSection = ChildTempSection;
			}
			else
			{
				return false;
			}
		}


	}
	else
	{
		return false;
	}

	return true;
}


void DataFileContainer::GetChildSectionIndicieVector(DataContainerSection* RootSection, const int ChildIndex, std::vector<int>& Indicies, int& ChildCount)
{
	for (int i = 0; i < RootSection->ChildSections.size(); i++)
	{
		if (ChildCount == ChildIndex)
		{
			ChildCount++;
			Indicies = RootSection->ChildSections[i].SectionIndiciePath;
			i = RootSection->ChildSections.size();
		}
		else
		{
			ChildCount++;
			GetChildSectionIndicieVector(&RootSection->ChildSections[i], ChildIndex, Indicies, ChildCount);
		}
		
	}
}


bool DataFileContainer::GetChildByIndicieVector(const std::vector<int>& Indicies, DataContainerSection*& ChildSection)
{
	if (Indicies[0] < m_Data.RootSections.size())
	{
		DataContainerSection* TempSection = &m_Data.RootSections[Indicies[0]];

		if (Indicies.size() == 1)
		{
			ChildSection = TempSection;
			return true;
		}

		for (int i = 1; i < Indicies.size(); i++)
		{
			if (i == Indicies.size() - 1)
			{
				if (Indicies[i] < TempSection->ChildSections.size())
				{
					GetChildByIndex(TempSection, Indicies[i], ChildSection);
					return true;
				}
				else
				{
					return false;
				}
			}
			else
			{
				if (Indicies[i] < TempSection->ChildSections.size())
				{
					GetChildByIndex(TempSection, Indicies[i], TempSection);
				}
				else
				{
					return false;
				}
			}
		}
	}
	else
	{
		return false;
	}
}


void DataFileContainer::CalculateNumberOfAllChildsFromRoot(DataContainerSection* RootSection, int& NumberOfChildSections)
{
	for (int i = 0; i < RootSection->ChildSections.size(); i++)
	{
		CalculateNumberOfAllChildsFromRoot(&RootSection->ChildSections[i], NumberOfChildSections);
		NumberOfChildSections++;
	}
	
}


bool  DataFileContainer::CheckIfRootSectionExist(std::string SectionName)
{

	auto it = std::find_if(m_Data.RootSections.begin(), m_Data.RootSections.end(), [&SectionName](const DataContainerSection& obj) {return obj.SectionName == SectionName; });

	if (it != m_Data.RootSections.end())
	{
		return true;
	}

	return false;
}


bool DataFileContainer::CheckIfChildSectionExist(DataContainerSection* ParentSection, std::string SectionName)
{
	auto it = std::find_if(ParentSection->ChildSections.begin(), ParentSection->ChildSections.end(), [&SectionName](const DataContainerSection& obj) {return obj.SectionName == SectionName; });

	if (it != ParentSection->ChildSections.end())
	{
		return true;
	}

	return false;
}


bool DataFileContainer::CheckIfSectionPathExist(std::vector<std::string> SectionPath)
{
	DataContainerSection* RootTempSection;

	if (GetRootSectionByName(SectionPath[0], RootTempSection))
	{
		DataContainerSection* ChildTempSection = RootTempSection;

		for (int SectionNameListIndex = 1; SectionNameListIndex < SectionPath.size(); SectionNameListIndex++)
		{
			DataContainerSection* TempSection = nullptr;
			if (CheckIfChildSectionExist(ChildTempSection, SectionPath[SectionNameListIndex]))
			{
				GetChildSectionFromParent(ChildTempSection, SectionPath[SectionNameListIndex], TempSection);
				ChildTempSection = TempSection;
			}
			else
			{
				return false;
			}
		}


	}
	else
	{
		return false;
	}

	return true;
}


void DataFileContainer::PrintSectionToFile(DataContainerSection* SectionData, std::ofstream& OutputFile)
{
	for (int i = 0; i < SectionData->SectionPath.size(); i++)
	{
		if (i == SectionData->SectionPath.size() - 1)
		{
			OutputFile << "[" << SectionData->SectionPath[i] << "]";
		}
		else
		{
			OutputFile << "[" << SectionData->SectionPath[i] << "].";
		}
	}

	OutputFile << std::endl;

	for (int i = 0; i < SectionData->IntVals.size(); i++)
	{
		int Val = SectionData->IntVals[i].Val;
		std::string ValName = SectionData->IntVals[i].Name;

		OutputFile << ValName << " = " << std::to_string(Val) << std::endl;
	}

	for (int i = 0; i < SectionData->FloatVals.size(); i++)
	{
		float Val = SectionData->FloatVals[i].Val;
		std::string ValName = SectionData->FloatVals[i].Name;

		OutputFile << ValName << " = " << std::to_string(Val) << std::endl;
	}

	for (int i = 0; i < SectionData->BoolVals.size(); i++)
	{
		bool Val = SectionData->BoolVals[i].Val;
		std::string ValName = SectionData->BoolVals[i].Name;

		OutputFile << ValName << " = " << std::to_string(Val) << std::endl;
	}

	for (int i = 0; i < SectionData->StringVals.size(); i++)
	{
		std::string Val = SectionData->StringVals[i].Val;
		std::string ValName = SectionData->StringVals[i].Name;

		OutputFile << ValName << " = \"" << Val << "\"" << std::endl;
	}

	for (int i = 0; i < SectionData->IntVectors.size(); i++)
	{
		std::vector<int> Val = SectionData->IntVectors[i].Val;
		std::string ValName = SectionData->IntVectors[i].Name;

		OutputFile << ValName << " = { ";
		for (int t = 0; t < Val.size(); t++)
		{
			if (t == Val.size() - 1)
			{
				OutputFile << std::to_string(Val[t]);
			}
			else
			{
				OutputFile << std::to_string(Val[t]) << ", ";
			}
		}
		OutputFile << " }" << std::endl;

	}

	for (int i = 0; i < SectionData->FloatVectors.size(); i++)
	{
		std::vector<float> Val = SectionData->FloatVectors[i].Val;
		std::string ValName = SectionData->FloatVectors[i].Name;

		OutputFile << ValName << " = { ";
		for (int t = 0; t < Val.size(); t++)
		{
			if (t == Val.size() - 1)
			{
				OutputFile << std::to_string(Val[t]);
			}
			else
			{
				OutputFile << std::to_string(Val[t]) << ", ";
			}
		}
		OutputFile << " }" << std::endl;

	}

	for (int i = 0; i < SectionData->BoolVectors.size(); i++)
	{
		std::vector<bool> Val = SectionData->BoolVectors[i].Val;
		std::string ValName = SectionData->BoolVectors[i].Name;

		OutputFile << ValName << " = { ";
		for (int t = 0; t < Val.size(); t++)
		{
			if (t == Val.size() - 1)
			{
				OutputFile << std::to_string(Val[t]);
			}
			else
			{
				OutputFile << std::to_string(Val[t]) << ", ";
			}
		}
		OutputFile << " }" << std::endl;


	}

	for (int i = 0; i < SectionData->StringVectors.size(); i++)
	{
		std::vector<std::string> Val = SectionData->StringVectors[i].Val;
		std::string ValName = SectionData->StringVectors[i].Name;

		OutputFile << ValName << " = { ";
		for (int t = 0; t < Val.size(); t++)
		{
			if (t == Val.size() - 1)
			{
				OutputFile << "\"" << Val[t] << "\"";
			}
			else
			{
				OutputFile << "\"" << Val[t] << "\"" << ", ";
			}
		}
		OutputFile << " }" << std::endl;
	}

	OutputFile << std::endl;

	for (int k = 0; k < SectionData->ChildSections.size(); k++)
	{
		PrintSectionToFile(&SectionData->ChildSections[k], OutputFile);
	}
}


