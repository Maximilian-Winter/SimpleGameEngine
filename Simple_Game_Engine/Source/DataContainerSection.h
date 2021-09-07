// Filename: DataFileContainer.h
// Date: 07.06.2015
// Author: Maximilian Winter
#ifndef _DATACONTAINERSECTION_H_
#define _DATACONTAINERSECTION_H_

#include <vector>
#include <algorithm>
#include <string>
#include <fstream>

class DataContainerSection
{
public:

	struct BinarySavingValueInformation
	{
		long long SizeInBytes;
		long long OutputStreamPositionBeforeSaving;
		long long OutputStreamPositionAfterSaving;
		size_t ValueCount;
		std::vector<size_t> PerValueSize;

		void SaveToBinaryOutputStream(std::string Name, std::ofstream& OutputFile);

		void LoadFromBinaryInputStream(std::ifstream& InputFile);
	};

	//Data Structures for the Configfile
	struct IntValue
	{
		IntValue();
		IntValue(std::string ValName, int Value);
		IntValue(std::ifstream& InputFile);
		std::string Name;
		int Val;
		BinarySavingValueInformation SavingValueInformation;

		void SaveToBinaryOutputStream(std::ofstream& OutputFile);

		void LoadFromBinaryInputStream(std::ifstream& InputFile);
	};

	struct IntVector
	{
		IntVector();
		IntVector(std::string ValName, std::vector<int> Value);
		IntVector(std::ifstream& InputFile);
		std::string Name;
		std::vector<int> Val;
		BinarySavingValueInformation SavingValueInformation;

		void SaveToBinaryOutputStream(std::ofstream& OutputFile);

		void LoadFromBinaryInputStream(std::ifstream& InputFile);
	};

	struct FloatValue
	{
		FloatValue();
		FloatValue(std::string ValName, float Value);
		FloatValue(std::ifstream& InputFile);
		std::string Name;
		float Val;
		BinarySavingValueInformation SavingValueInformation;

		void SaveToBinaryOutputStream(std::ofstream& OutputFile);

		void LoadFromBinaryInputStream(std::ifstream& InputFile);
	};

	struct FloatVector
	{
		FloatVector();
		FloatVector(std::string ValName, std::vector<float> Value);
		FloatVector(std::ifstream& InputFile);
		std::string Name;
		std::vector<float> Val;
		BinarySavingValueInformation SavingValueInformation;

		void SaveToBinaryOutputStream(std::ofstream& OutputFile);

		void LoadFromBinaryInputStream(std::ifstream& InputFile);
	};

	struct BoolValue
	{
		BoolValue();
		BoolValue(std::string ValName, bool Value);
		BoolValue(std::ifstream& InputFile);
		std::string Name;
		bool Val;
		BinarySavingValueInformation SavingValueInformation;

		void SaveToBinaryOutputStream(std::ofstream& OutputFile);

		void LoadFromBinaryInputStream(std::ifstream& InputFile);
	};

	struct BoolVector
	{
		BoolVector();
		BoolVector(std::string ValName, std::vector<bool> Value);
		BoolVector(std::ifstream& InputFile);
		std::string Name;
		std::vector<bool> Val;
		BinarySavingValueInformation SavingValueInformation;

		void SaveToBinaryOutputStream(std::ofstream& OutputFile);

		void LoadFromBinaryInputStream(std::ifstream& InputFile);
	};

	struct StringValue
	{
		StringValue();
		StringValue(std::string ValName, std::string Value);
		StringValue(std::ifstream& InputFile);
		std::string Name;
		std::string Val;
		BinarySavingValueInformation SavingValueInformation;

		void SaveToBinaryOutputStream(std::ofstream& OutputFile);

		void LoadFromBinaryInputStream(std::ifstream& InputFile);
	};

	struct StringVector
	{
		StringVector();
		StringVector(std::string ValName, std::vector<std::string> Value);
		StringVector(std::ifstream& InputFile);
		std::string Name;
		std::vector<std::string> Val;
		BinarySavingValueInformation SavingValueInformation;

		void SaveToBinaryOutputStream(std::ofstream& OutputFile);

		void LoadFromBinaryInputStream(std::ifstream& InputFile);
	};


	DataContainerSection();

	DataContainerSection(std::ifstream& InputFile);

	DataContainerSection(std::string sectionName);

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
	bool GetIntValSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value);
	bool GetIntValName(int Index, std::string& ValueName);

	bool GetIntVector(std::string ValueName, std::vector<int>& Value);
	bool GetIntVector(int Index, std::vector<int>& Value);
	bool GetIntVectorSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value);
	bool GetIntVectorName(int Index, std::string& ValueName);

	bool GetFloatVal(std::string ValueName, float& Value);
	bool GetFloatVal(int Index, float& Value);
	bool GetFloatValSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value);
	bool GetFloatValName(int Index, std::string& ValueName);

	bool GetFloatVector(std::string ValueName, std::vector<float>& Value);
	bool GetFloatVector(int Index, std::vector<float>& Value);
	bool GetFloatVectorSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value);
	bool GetFloatVectorName(int Index, std::string& ValueName);

	bool GetBoolVal(std::string ValueName, bool& Value);
	bool GetBoolVal(int Index, bool& Value);
	bool GetBoolValSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value);
	bool GetBoolValName(int Index, std::string& ValueName);

	bool GetBoolVector(std::string ValueName, std::vector<bool>& Value);
	bool GetBoolVector(int Index, std::vector<bool>& Value);
	bool GetBoolVectorSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value);
	bool GetBoolVectorName(int Index, std::string& ValueName);

	bool GetStringVal(std::string ValueName, std::string& Value);
	bool GetStringVal(int Index, std::string& Value);
	bool GetStringValSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value);
	bool GetStringValName(int Index, std::string& ValueName);

	bool GetStringVector(std::string ValueName, std::vector<std::string>& Value);
	bool GetStringVector(int Index, std::vector<std::string>& Value);
	bool GetStringVectorSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value);
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

	std::vector<DataContainerSection> ChildSections;
	BinarySavingValueInformation SavingValueInformation;

	void SaveValueSavingInformationToBinaryOutputStream(std::ofstream& OutputFile);

	void LoadValueSavingInformationToBinaryOutputStream(std::ifstream& InputFile);

	void SaveToBinaryOutputStream(std::ofstream& OutputFile);

	void LoadFromBinaryInputStream(std::ifstream& InputFile);
};

#endif