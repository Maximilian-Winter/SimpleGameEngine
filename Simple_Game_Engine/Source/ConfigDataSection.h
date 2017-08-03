// Filename: ConfigData.h
// Date: 07.06.2015
// Author: Maximilian Winter
#ifndef _CONFIGDATASECTION_H_
#define _CONFIGDATASECTION_H_

#include <vector>
#include <algorithm>

class ConfigDataSection
{
public:
	//Data Structures for the Configfile
	struct IntValue
	{
		IntValue(std::string ValName, int Value);
		std::string Name;
		int Val;
	};

	struct IntVector
	{
		IntVector(std::string ValName, std::vector<int> Value);
		std::string Name;
		std::vector<int> Val;
	};

	struct FloatValue
	{
		FloatValue(std::string ValName, float Value);
		std::string Name;
		float Val;
	};

	struct FloatVector
	{
		FloatVector(std::string ValName, std::vector<float> Value);
		std::string Name;
		std::vector<float> Val;
	};

	struct BoolValue
	{
		BoolValue(std::string ValName, bool Value);
		std::string Name;
		bool Val;
	};

	struct BoolVector
	{
		BoolVector(std::string ValName, std::vector<bool> Value);
		std::string Name;
		std::vector<bool> Val;
	};

	struct StringValue
	{
		StringValue(std::string ValName, std::string Value);
		std::string Name;
		std::string Val;
	};

	struct StringVector
	{
		StringVector(std::string ValName, std::vector<std::string> Value);
		std::string Name;
		std::vector<std::string> Val;
	};


	ConfigDataSection();;

	ConfigDataSection(std::string sectionName);

	bool AddIntVal(std::string ValueName, int Val);

	bool AddFloatVal(std::string ValueName, float Val);

	bool AddBoolVal(std::string ValueName, bool Val);

	bool AddStringVal(std::string ValueName, std::string Val);

	bool AddIntVector(std::string ValueName, std::vector<int> Val);

	bool AddFloatVector(std::string ValueName, std::vector<float> Val);

	bool AddBoolVector(std::string ValueName, std::vector<bool> Val);

	bool AddStringVector(std::string ValueName, std::vector< std::string> Val);

	bool GetIntVal(std::string ValueName, int& Value);
	bool GetIntVal(int Index, int& Value);
	bool GetIntValName(int Index, std::string& ValueName);

	bool GetIntVector(std::string ValueName, std::vector<int>& Value);
	bool GetIntVector(int Index, std::vector<int>& Value);
	bool GetIntVectorName(int Index, std::string& ValueName);

	bool GetFloatVal(std::string ValueName, float& Value);
	bool GetFloatVal(int Index, float& Value);
	bool GetFloatValName(int Index, std::string& ValueName);

	bool GetFloatVector(std::string ValueName, std::vector<float>& Value);
	bool GetFloatVector(int Index, std::vector<float>& Value);
	bool GetFloatVectorName(int Index, std::string& ValueName);

	bool GetBoolVal(std::string ValueName, bool& Value);
	bool GetBoolVal(int Index, bool& Value);
	bool GetBoolValName(int Index, std::string& ValueName);

	bool GetBoolVector(std::string ValueName, std::vector<bool>& Value);
	bool GetBoolVector(int Index, std::vector<bool>& Value);
	bool GetBoolVectorName(int Index, std::string& ValueName);

	bool GetStringVal(std::string ValueName, std::string& Value);
	bool GetStringVal(int Index, std::string& Value);
	bool GetStringValName(int Index, std::string& ValueName);

	bool GetStringVector(std::string ValueName, std::vector<std::string>& Value);
	bool GetStringVector(int Index, std::vector<std::string>& Value);
	bool GetStringVectorName(int Index, std::string& ValueName);

	int GetNumberOfIntVals();
	int GetNumberOfIntVectors();

	int GetNumberOfFloatVals();
	int GetNumberOfFloatVectors();

	int GetNumberOfBoolVals();
	int GetNumberOfBoolVectors();

	int GetNumberOfStringVals();
	int GetNumberOfStringVectors();

	bool CheckIfValueExist(std::string ValueName);
	bool CheckIfIntValueExist(std::string ValueName);
	bool CheckIfIntVectorExist(std::string ValueName);
	bool CheckIfFloatValueExist(std::string ValueName);
	bool CheckIfFloatVectorExist(std::string ValueName);
	bool CheckIfBoolValueExist(std::string ValueName);
	bool CheckIfBoolVectorExist(std::string ValueName);
	bool CheckIfStringValueExist(std::string ValueName);
	bool CheckIfStringVectorExist(std::string ValueName);

	std::string SectionName;
	std::vector<std::string> SectionPath;
	std::vector<int> SectionIndiciePath;

	std::vector<IntValue> IntVals;
	std::vector<IntVector> IntVectors;
	std::vector<FloatValue> FloatVals;
	std::vector<FloatVector> FloatVectors;
	std::vector<BoolValue> BoolVals;
	std::vector<BoolVector> BoolVectors;
	std::vector<StringValue> StringVals;
	std::vector<StringVector> StringVectors;

	std::vector<ConfigDataSection> ChildSections;
};

#endif