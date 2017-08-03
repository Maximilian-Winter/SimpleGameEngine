#include "ConfigData.h"


ConfigData::ConfigData()
{
	configData.RootSections.clear();
}


ConfigData::ConfigData(const ConfigData& other)
{
}


ConfigData::~ConfigData()
{
}


bool ConfigData::LoadConfigDataFile(const std::string FileName)
{
	//Clear the vector with the Config Data
	configData.RootSections.clear();
	std::ifstream InputFile(FileName);
	std::string InputStr;

	if (InputFile.good())
	{
		ConfigDataSection* CurrentSection = nullptr;
		bool HasSections = false;

		while (!InputFile.eof())
		{
			//Start to read in the Data from the File
			std::getline(InputFile, InputStr);
			ConfigDataParser::RemoveCommentFromLine(InputStr);

			if (!ConfigDataParser::IsOnlyWhitespacesLine(InputStr))
			{
				if (ConfigDataParser::IsSectionNameLine(InputStr))
				{
					std::string SectionName = ConfigDataParser::GetSectionNameFromLine(InputStr);

					if (CheckIfRootSectionExist(SectionName))
					{
						GetRootSectionByName(SectionName, CurrentSection);
					}
					else
					{
						AddSection(ConfigDataParser::GetChildSectionNameListFromLine(InputStr), CurrentSection);
					}
					
					HasSections = true;
				}

				if (ConfigDataParser::IsChildSectionNameLine(InputStr))
				{
					
					if (CheckIfSectionPathExist(ConfigDataParser::GetChildSectionNameListFromLine(InputStr)))
					{
						GetChildBySectionPath(ConfigDataParser::GetChildSectionNameListFromLine(InputStr), CurrentSection);
					}
					else
					{
						AddSection(ConfigDataParser::GetChildSectionNameListFromLine(InputStr), CurrentSection);
					}
					HasSections = true;
				}

				if (ConfigDataParser::IsValueLine(InputStr))
				{
					std::string ValueName = ConfigDataParser::GetValueNameFromLine(InputStr);
					if(CurrentSection != nullptr)
					{
						if (ConfigDataParser::IsIntValueLine(InputStr))
						{
							int Value = ConfigDataParser::GetIntValueFromLine(InputStr);
							
							CurrentSection->AddIntVal(ValueName, Value);
						}

						if (ConfigDataParser::IsFloatValueLine(InputStr))
						{
							float Value = ConfigDataParser::GetFloatValueFromLine(InputStr);
						
							CurrentSection->AddFloatVal(ValueName, Value);
						}

						if (ConfigDataParser::IsBoolValueLine(InputStr))
						{
							bool Value = ConfigDataParser::GetBoolValueFromLine(InputStr);
						
							CurrentSection->AddBoolVal(ValueName, Value);
						}

						if (ConfigDataParser::IsStringValueLine(InputStr))
						{
							std::string Value = ConfigDataParser::GetStringValueFromLine(InputStr);
						
							CurrentSection->AddStringVal(ValueName, Value);
						}

						if (ConfigDataParser::IsIntVectorLine(InputStr))
						{
							std::vector<int> Value = ConfigDataParser::GetIntVectorFromLine(InputStr);

							CurrentSection->AddIntVector(ValueName, Value);
						}

						if (ConfigDataParser::IsFloatVectorLine(InputStr))
						{
							std::vector<float> Value = ConfigDataParser::GetFloatVectorFromLine(InputStr);

							CurrentSection->AddFloatVector(ValueName, Value);
						}
					
						if (ConfigDataParser::IsBoolVectorLine(InputStr))
						{
							std::vector<bool> Value = ConfigDataParser::GetBoolVectorFromLine(InputStr);

							CurrentSection->AddBoolVector(ValueName, Value);
						}

						if (ConfigDataParser::IsStringVectorLine(InputStr))
						{
							std::vector<std::string> Value = ConfigDataParser::GetStringVectorFromLine(InputStr);

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


void ConfigData::SaveConfigDataFile(const std::string FileName, const std::string CommentAtStartOfFile)
{
	std::ofstream fileOutput;
	fileOutput.open(FileName);

	fileOutput << "#" <<CommentAtStartOfFile << std::endl;
	fileOutput << std::endl;
	for (int k = 0; k < configData.RootSections.size(); k++)
	{
		PrintSectionToFile(&configData.RootSections[k], fileOutput);
	}
	fileOutput.close();
}


void ConfigData::ClearConfigData()
{
	configData.RootSections.clear();
}


bool ConfigData::AddIntValueByName(const std::string SectionName, const std::string ValueName, int Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddIntVal(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::AddFloatValueByName(const std::string SectionName, const std::string ValueName, float Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddFloatVal(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::AddBoolValueByName(const std::string SectionName, const std::string ValueName, bool Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddBoolVal(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::AddStringValueByName(const std::string SectionName, const std::string ValueName, std::string Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddStringVal(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::AddIntVectorByName(const std::string SectionName, const std::string ValueName, std::vector<int> Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddIntVector(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::AddFloatVectorByName(const std::string SectionName, const std::string ValueName, std::vector<float> Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddFloatVector(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::AddBoolVectorByName(const std::string SectionName, const std::string ValueName, std::vector<bool> Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddBoolVector(ValueName, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::AddStringVectorByName(const std::string SectionName, const std::string ValueName, std::vector<std::string> Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		AddSection(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection);
		return TempSection->AddStringVector(ValueName, Value);
	}
	else
	{
		return false;
	}
}


int ConfigData::GetNumberOfRootSections()
{
	return configData.RootSections.size();
}


std::vector<int> ConfigData::GetIndiciePathToRootSecton(int RootSectionIndex)
{
	std::vector<int> Indicies;
	Indicies.push_back(RootSectionIndex);

	return Indicies;
}


int ConfigData::GetNumberOfAllChildSectionsFromRoot(const int RootSectionIndex)
{
	ConfigDataSection* Root = &configData.RootSections[RootSectionIndex];

	int NumberOfChilds = 0;

	CalculateNumberOfAllChildsFromRoot(Root, NumberOfChilds);

	return NumberOfChilds;
}


bool ConfigData::GetSectionName(const std::vector<int> Indicies, std::string& SectionName)
{
	ConfigDataSection* TempSection;
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


std::vector<int> ConfigData::GetIndiciePathToChildFromRoot(const int RootSectionIndex, const int ChildIndex)
{
	std::vector<int> Indicies;

	ConfigDataSection* Root = &configData.RootSections[RootSectionIndex];
	
	int TempChildCount = 0;

	GetChildSectionIndicieVector(Root, ChildIndex, Indicies, TempChildCount);

	return Indicies;
}


int ConfigData::GetNumberOfIntValues(std::vector<int>&  Indicies)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfIntVals();
	}
	else
	{
		return 0;
	}
}


int ConfigData::GetNumberOfIntVectors(std::vector<int>&  Indicies)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfIntVectors();
	}
	else
	{
		return 0;
	}
}


int ConfigData::GetNumberOfFloatValues(std::vector<int>&  Indicies)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfFloatVals();
	}
	else
	{
		return 0;
	}
}


int ConfigData::GetNumberOfFloatVectors(std::vector<int>&  Indicies)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfFloatVectors();
	}
	else
	{
		return 0;
	}
}


int ConfigData::GetNumberOfBoolValues(std::vector<int>&  Indicies)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfBoolVals();
	}
	else
	{
		return 0;
	}
}


int ConfigData::GetNumberOfBoolVectors(std::vector<int>&  Indicies)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfBoolVectors();
	}
	else
	{
		return 0;
	}

}


int ConfigData::GetNumberOfStringValues(std::vector<int>&  Indicies)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfStringVals();
	}
	else
	{
		return 0;
	}
}


int ConfigData::GetNumberOfStringVectors(std::vector<int>&  Indicies)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetNumberOfStringVectors();
	}
	else
	{
		return 0;
	}
}


bool ConfigData::GetIntValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetIntValName(ValueIndex, ValueName);
	}
	else
	{
		return false;

	}
}


bool ConfigData::GetIntValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, int& Value)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetIntVal(ValueIndex, Value);
	}
	else
	{
		return false;

	}
}


bool ConfigData::GetIntValueByName(const std::string SectionName, const std::string ValueName, int& Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
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


bool ConfigData::GetIntVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetIntVectorName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetIntVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<int>& Value)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetIntVector(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetIntVectorByName(const std::string SectionName, const std::string ValueName, std::vector<int> &Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
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


bool ConfigData::GetFloatValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetFloatValName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetFloatValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, float& Value)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetFloatVal(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetFloatValueByName(const std::string SectionName, const std::string ValueName, float& Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
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


bool ConfigData::GetFloatVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetFloatVectorName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetFloatVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<float>& Value)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetFloatVector(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetFloatVectorByName(const std::string SectionName, const std::string ValueName, std::vector<float> &Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
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


bool ConfigData::GetBoolValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetBoolValName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetBoolValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, bool& Value)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetBoolVal(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetBoolValueByName(const std::string SectionName, const std::string ValueName, bool& Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
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


bool ConfigData::GetBoolVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetBoolVectorName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetBoolVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<bool> &Value)
{

	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetBoolVector(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetBoolVectorByName(const std::string SectionName, const std::string ValueName, std::vector<bool> &Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
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


bool ConfigData::GetStringValueNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetStringValName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetStringValueByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& Value)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetStringVal(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetStringValueByName(const std::string SectionName, const std::string ValueName, std::string& Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
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


bool ConfigData::GetStringVectorNameByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::string& ValueName)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetStringVectorName(ValueIndex, ValueName);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetStringVectorByIndex(const std::vector<int>& Indicies, const int ValueIndex, std::vector<std::string> &Value)
{
	ConfigDataSection* TempSection;
	if (GetChildByIndicieVector(Indicies, TempSection))
	{
		return TempSection->GetStringVector(ValueIndex, Value);
	}
	else
	{
		return false;
	}
}


bool ConfigData::GetStringVectorByName(const std::string SectionName, const std::string ValueName, std::vector<std::string> &Value)
{
	ConfigDataSection* TempSection;

	if (ConfigDataParser::IsChildSectionNameInput(SectionName))
	{
		if (GetChildBySectionPath(ConfigDataParser::GetSectionNameListFromFunctionArg(SectionName), TempSection))
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


bool ConfigData::AddSection(std::vector<std::string> SectionPathList, ConfigDataSection*& AddedSection)
{
	ConfigDataSection* TempSection;

	if (GetRootSectionByName(SectionPathList[0], TempSection))
	{
		ConfigDataSection* ChildTempSection = TempSection;

		std::vector<int> TempIndicies;

		int RootIndex;
		GetRootSectionIndex(SectionPathList[0], RootIndex);

		TempIndicies.push_back(RootIndex);

		for (int SectionNameListIndex = 1; SectionNameListIndex < SectionPathList.size(); SectionNameListIndex++)
		{
			ConfigDataSection* TempSection = nullptr;
			if (CheckIfChildSectionExist(ChildTempSection, SectionPathList[SectionNameListIndex]))
			{
				GetChildSectionFromParent(ChildTempSection, SectionPathList[SectionNameListIndex], TempSection);
				ChildTempSection = TempSection;

				TempIndicies = ChildTempSection->SectionIndiciePath;
			}
			else
			{
				ConfigDataSection Temp(SectionPathList[SectionNameListIndex]);

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
		configData.RootSections.push_back(ConfigDataSection(SectionPathList[0]));

		ConfigDataSection* ChildTempSection = &configData.RootSections[configData.RootSections.size() - 1];
		std::vector<int> TempIndicies;
		int RootIndex;
		ChildTempSection->SectionPath.push_back(SectionPathList[0]);

		GetRootSectionIndex(SectionPathList[0], RootIndex);

		TempIndicies.push_back(RootIndex);
		for (int SectionNameListIndex = 1; SectionNameListIndex < SectionPathList.size(); SectionNameListIndex++)
		{
			ConfigDataSection* TempSection = nullptr;
			if (CheckIfChildSectionExist(ChildTempSection, SectionPathList[SectionNameListIndex]))
			{
				GetChildSectionFromParent(ChildTempSection, SectionPathList[SectionNameListIndex], TempSection);
				ChildTempSection = TempSection;

				TempIndicies = ChildTempSection->SectionIndiciePath;
			}
			else
			{
				ConfigDataSection Temp(SectionPathList[SectionNameListIndex]);

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


bool ConfigData::GetRootSectionByName(std::string SectionName, ConfigDataSection*& RootSection)
{
	RootSection = nullptr;

	auto it = std::find_if(configData.RootSections.begin(), configData.RootSections.end(), [&SectionName](const ConfigDataSection& obj) {return obj.SectionName == SectionName; });

	if (it != configData.RootSections.end())
	{
		auto index = std::distance(configData.RootSections.begin(), it);
		RootSection = &configData.RootSections[index];

		return true;
	}

	return false;
}


bool ConfigData::GetRootSectionIndex(std::string SectionName, int& RootSectionIndex)
{

	auto it = std::find_if(configData.RootSections.begin(), configData.RootSections.end(), [&SectionName](const ConfigDataSection& obj) {return obj.SectionName == SectionName; });

	if (it != configData.RootSections.end())
	{
		auto index = std::distance(configData.RootSections.begin(), it);
		RootSectionIndex = index;

		return true;
	}

	return false;

	int SectionIndex = -1;
	for (int i = 0; i < configData.RootSections.size(); i++)
	{
		if (configData.RootSections[i].SectionName == SectionName)
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


bool ConfigData::GetChildSectionFromParent(ConfigDataSection* ParentSection, std::string SectionName, ConfigDataSection*& ChildSection)
{
	ChildSection = nullptr;

	auto it = std::find_if(ParentSection->ChildSections.begin(), ParentSection->ChildSections.end(), [&SectionName](const ConfigDataSection& obj) {return obj.SectionName == SectionName; });

	if (it != ParentSection->ChildSections.end())
	{
		auto index = std::distance(ParentSection->ChildSections.begin(), it);
		ChildSection = &ParentSection->ChildSections[index];

		return true;
	}

	return false;
}


void ConfigData::GetChildByIndex(ConfigDataSection* RootSection, int Index, ConfigDataSection*& ChildSection)
{
	ChildSection = &RootSection->ChildSections[Index];
}


bool ConfigData::GetChildBySectionPath(std::vector<std::string> SectionPath, ConfigDataSection*& ChildSection)
{
	ConfigDataSection* RootTempSection;

	if (GetRootSectionByName(SectionPath[0], RootTempSection))
	{
		ConfigDataSection* ChildTempSection = RootTempSection;

		if (SectionPath.size() == 1)
		{
			ChildSection = RootTempSection;
			return true;
		}

		for (int SectionNameListIndex = 1; SectionNameListIndex < SectionPath.size(); SectionNameListIndex++)
		{
			ConfigDataSection* TempSection = nullptr;
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


void ConfigData::GetChildSectionIndicieVector(ConfigDataSection* RootSection, const int ChildIndex, std::vector<int>& Indicies, int& ChildCount)
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


bool ConfigData::GetChildByIndicieVector(const std::vector<int>& Indicies, ConfigDataSection*& ChildSection)
{
	if (Indicies[0] < configData.RootSections.size())
	{
		ConfigDataSection* TempSection = &configData.RootSections[Indicies[0]];

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


void ConfigData::CalculateNumberOfAllChildsFromRoot(ConfigDataSection* RootSection, int& NumberOfChildSections)
{
	for (int i = 0; i < RootSection->ChildSections.size(); i++)
	{
		CalculateNumberOfAllChildsFromRoot(&RootSection->ChildSections[i], NumberOfChildSections);
		NumberOfChildSections++;
	}
	
}


bool  ConfigData::CheckIfRootSectionExist(std::string SectionName)
{

	auto it = std::find_if(configData.RootSections.begin(), configData.RootSections.end(), [&SectionName](const ConfigDataSection& obj) {return obj.SectionName == SectionName; });

	if (it != configData.RootSections.end())
	{
		return true;
	}

	return false;
}


bool ConfigData::CheckIfChildSectionExist(ConfigDataSection* ParentSection, std::string SectionName)
{
	auto it = std::find_if(ParentSection->ChildSections.begin(), ParentSection->ChildSections.end(), [&SectionName](const ConfigDataSection& obj) {return obj.SectionName == SectionName; });

	if (it != ParentSection->ChildSections.end())
	{
		return true;
	}

	return false;
}


bool ConfigData::CheckIfSectionPathExist(std::vector<std::string> SectionPath)
{
	ConfigDataSection* RootTempSection;

	if (GetRootSectionByName(SectionPath[0], RootTempSection))
	{
		ConfigDataSection* ChildTempSection = RootTempSection;

		for (int SectionNameListIndex = 1; SectionNameListIndex < SectionPath.size(); SectionNameListIndex++)
		{
			ConfigDataSection* TempSection = nullptr;
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


void ConfigData::PrintSectionToFile(ConfigDataSection* SectionData, std::ofstream& OutputFile)
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

			OutputFile << ValName << " = " << Val << std::endl;
		}

		for (int i = 0; i < SectionData->FloatVals.size(); i++)
		{
			float Val = SectionData->FloatVals[i].Val;
			std::string ValName = SectionData->FloatVals[i].Name;

			OutputFile << ValName << " = " << Val << std::endl;
		}

		for (int i = 0; i < SectionData->BoolVals.size(); i++)
		{
			bool Val = SectionData->BoolVals[i].Val;
			std::string ValName = SectionData->BoolVals[i].Name;

			OutputFile << ValName << " = " << Val << std::endl;
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
					OutputFile << Val[t];
				}
				else
				{
					OutputFile << Val[t] << ", ";
				}
			}
			OutputFile << " }" << std::endl;

		}

		for (int i = 0; i <  SectionData->FloatVectors.size(); i++)
		{
			std::vector<float> Val = SectionData->FloatVectors[i].Val;
			std::string ValName = SectionData->FloatVectors[i].Name;

			OutputFile << ValName << " = { ";
			for (int t = 0; t < Val.size(); t++)
			{
				if (t == Val.size() - 1)
				{
					OutputFile << Val[t];
				}
				else
				{
					OutputFile << Val[t] << ", ";
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
					OutputFile << Val[t];
				}
				else
				{
					OutputFile << Val[t] << ", ";
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