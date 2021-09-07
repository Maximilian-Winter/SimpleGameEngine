#include "DataContainerSection.h"

void DataContainerSection::BinarySavingValueInformation::SaveToBinaryOutputStream(std::string Name, std::ofstream& OutputFile)
{
	size_t size = Name.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&Name[0], size);
	OutputFile.write(reinterpret_cast<char*>(&OutputStreamPositionBeforeSaving), sizeof(OutputStreamPositionBeforeSaving));
	OutputFile.write(reinterpret_cast<char*>(&OutputStreamPositionAfterSaving), sizeof(OutputStreamPositionAfterSaving));
	OutputFile.write(reinterpret_cast<char*>(&SizeInBytes), sizeof(SizeInBytes));
	OutputFile.write(reinterpret_cast<char*>(&ValueCount), sizeof(ValueCount));
	OutputFile.write(reinterpret_cast<char*>(&OutputStreamPositionBeforeSaving), sizeof(OutputStreamPositionBeforeSaving));
	OutputFile.write(reinterpret_cast<char*>(&OutputStreamPositionAfterSaving), sizeof(OutputStreamPositionAfterSaving));
	size = PerValueSize.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(reinterpret_cast<char*>(&PerValueSize[0]), size * sizeof(PerValueSize[0]));
}

void DataContainerSection::BinarySavingValueInformation::LoadFromBinaryInputStream(std::ifstream& InputFile)
{
	std::string Name;
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Name.resize(size);
	InputFile.read(&Name[0], size);
	InputFile.read(reinterpret_cast<char*>(&OutputStreamPositionBeforeSaving), sizeof(OutputStreamPositionBeforeSaving));
	InputFile.read(reinterpret_cast<char*>(&OutputStreamPositionAfterSaving), sizeof(OutputStreamPositionAfterSaving));
	InputFile.read(reinterpret_cast<char*>(&SizeInBytes), sizeof(SizeInBytes));
	InputFile.read(reinterpret_cast<char*>(&ValueCount), sizeof(ValueCount));
	InputFile.read(reinterpret_cast<char*>(&OutputStreamPositionBeforeSaving), sizeof(OutputStreamPositionBeforeSaving));
	InputFile.read(reinterpret_cast<char*>(&OutputStreamPositionAfterSaving), sizeof(OutputStreamPositionAfterSaving));
}

DataContainerSection::IntValue::IntValue()
{
}

DataContainerSection::IntValue::IntValue(std::string ValName, int Value)
{
	Name = ValName;
	Val = Value;
}

DataContainerSection::IntValue::IntValue(std::ifstream& InputFile)
{
	LoadFromBinaryInputStream(InputFile);
}

void DataContainerSection::IntValue::SaveToBinaryOutputStream(std::ofstream& OutputFile)
{
	size_t size = Name.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&Name[0], size);
	SavingValueInformation.OutputStreamPositionBeforeSaving = OutputFile.tellp();
	OutputFile.write(reinterpret_cast<char*>(&Val), sizeof(Val));
	SavingValueInformation.OutputStreamPositionAfterSaving = OutputFile.tellp();
	SavingValueInformation.SizeInBytes = SavingValueInformation.OutputStreamPositionAfterSaving - SavingValueInformation.OutputStreamPositionBeforeSaving;
	SavingValueInformation.PerValueSize.push_back(sizeof(Val));
	SavingValueInformation.ValueCount = 1;
}

void DataContainerSection::IntValue::LoadFromBinaryInputStream(std::ifstream& InputFile)
{
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Name.resize(size);
	InputFile.read(&Name[0], size);
	InputFile.read(reinterpret_cast<char*>(&Val), sizeof(Val));
}

DataContainerSection::IntVector::IntVector()
{
}

void DataContainerSection::IntVector::SaveToBinaryOutputStream(std::ofstream& OutputFile)
{
	
	size_t size = Name.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&Name[0], size);
	size = Val.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	SavingValueInformation.OutputStreamPositionBeforeSaving = OutputFile.tellp();
	OutputFile.write(reinterpret_cast<char*>(&Val[0]), size * sizeof(Val[0]));
	SavingValueInformation.OutputStreamPositionAfterSaving = OutputFile.tellp();
	SavingValueInformation.SizeInBytes = SavingValueInformation.OutputStreamPositionAfterSaving - SavingValueInformation.OutputStreamPositionBeforeSaving;
	SavingValueInformation.PerValueSize.push_back(sizeof(Val[0]));
	SavingValueInformation.ValueCount = size;
}

void DataContainerSection::IntVector::LoadFromBinaryInputStream(std::ifstream& InputFile)
{
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Name.resize(size);
	InputFile.read(&Name[0], size);
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Val.resize(size);
	InputFile.read(reinterpret_cast<char*> (&Val[0]), size * sizeof(Val[0]));
}

DataContainerSection::FloatValue::FloatValue()
{
}

void DataContainerSection::FloatValue::SaveToBinaryOutputStream(std::ofstream& OutputFile)
{
	size_t size = Name.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&Name[0], size);
	SavingValueInformation.OutputStreamPositionBeforeSaving = OutputFile.tellp();
	OutputFile.write(reinterpret_cast<char*>(&Val), sizeof(Val));
	SavingValueInformation.OutputStreamPositionAfterSaving = OutputFile.tellp();
	SavingValueInformation.SizeInBytes = SavingValueInformation.OutputStreamPositionAfterSaving - SavingValueInformation.OutputStreamPositionBeforeSaving;
	SavingValueInformation.PerValueSize.push_back(sizeof(Val));
	SavingValueInformation.ValueCount = 1;
}

void DataContainerSection::FloatValue::LoadFromBinaryInputStream(std::ifstream& InputFile)
{
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Name.resize(size);
	InputFile.read(&Name[0], size);
	InputFile.read(reinterpret_cast<char*>(&Val), sizeof(Val));
}

DataContainerSection::FloatVector::FloatVector()
{
}

void DataContainerSection::FloatVector::SaveToBinaryOutputStream(std::ofstream& OutputFile)
{
	size_t size = Name.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&Name[0], size);
	size = Val.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	SavingValueInformation.OutputStreamPositionBeforeSaving = OutputFile.tellp();
	OutputFile.write(reinterpret_cast<char*>(&Val[0]), size * sizeof(Val[0]));
	SavingValueInformation.OutputStreamPositionAfterSaving = OutputFile.tellp();
	SavingValueInformation.SizeInBytes = SavingValueInformation.OutputStreamPositionAfterSaving - SavingValueInformation.OutputStreamPositionBeforeSaving;
	SavingValueInformation.PerValueSize.push_back(sizeof(Val[0]));
	SavingValueInformation.ValueCount = size;

}

void DataContainerSection::FloatVector::LoadFromBinaryInputStream(std::ifstream& InputFile)
{
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Name.resize(size);
	InputFile.read(&Name[0], size);
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Val.resize(size);
	InputFile.read(reinterpret_cast<char*> (&Val[0]), size * sizeof(Val[0]));
}

DataContainerSection::BoolValue::BoolValue()
{
}

void DataContainerSection::BoolValue::SaveToBinaryOutputStream(std::ofstream& OutputFile)
{

	size_t size = Name.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&Name[0], size);
	SavingValueInformation.OutputStreamPositionBeforeSaving = OutputFile.tellp();
	OutputFile.write(reinterpret_cast<char*>(&Val), sizeof(Val));
	SavingValueInformation.OutputStreamPositionAfterSaving = OutputFile.tellp();
	SavingValueInformation.SizeInBytes = SavingValueInformation.OutputStreamPositionAfterSaving - SavingValueInformation.OutputStreamPositionBeforeSaving;
	SavingValueInformation.PerValueSize.push_back(sizeof(Val));
	SavingValueInformation.ValueCount = 1;
}

void DataContainerSection::BoolValue::LoadFromBinaryInputStream(std::ifstream& InputFile)
{
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Name.resize(size);
	InputFile.read(&Name[0], size);
	InputFile.read(reinterpret_cast<char*>(&Val), sizeof(Val));
}

DataContainerSection::BoolVector::BoolVector()
{
}

void DataContainerSection::BoolVector::SaveToBinaryOutputStream(std::ofstream& OutputFile)
{
	
	size_t size = Name.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&Name[0], size);
	size = Val.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));

	bool* tempArray = new bool[size];
	for(int i = 0; i < size; i++)
	{
		tempArray[i] = Val[i];
	}
	SavingValueInformation.OutputStreamPositionBeforeSaving = OutputFile.tellp();
	OutputFile.write(reinterpret_cast<char*>(tempArray[0]), size * sizeof(Val[0]));
	SavingValueInformation.OutputStreamPositionAfterSaving = OutputFile.tellp();
	SavingValueInformation.SizeInBytes = SavingValueInformation.OutputStreamPositionAfterSaving - SavingValueInformation.OutputStreamPositionBeforeSaving;
	SavingValueInformation.PerValueSize.push_back(sizeof(Val[0]));
	SavingValueInformation.ValueCount = size;
}

void DataContainerSection::BoolVector::LoadFromBinaryInputStream(std::ifstream& InputFile)
{
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Name.resize(size);
	InputFile.read(&Name[0], size);
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Val.resize(size);

	bool* tempArray = new bool[size];
	InputFile.read(reinterpret_cast<char*>(tempArray[0]), size * sizeof(Val[0]));

	for (int i = 0; i < size; i++)
	{
		Val[i] = tempArray[i];
	}
}

DataContainerSection::StringValue::StringValue()
{
}

void DataContainerSection::StringValue::SaveToBinaryOutputStream(std::ofstream& OutputFile)
{
	
	size_t size = Name.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&Name[0], size);
	size = Val.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	SavingValueInformation.OutputStreamPositionBeforeSaving = OutputFile.tellp();
	OutputFile.write(&Val[0], size);
	SavingValueInformation.OutputStreamPositionAfterSaving = OutputFile.tellp();
	SavingValueInformation.SizeInBytes = SavingValueInformation.OutputStreamPositionAfterSaving - SavingValueInformation.OutputStreamPositionBeforeSaving;
	SavingValueInformation.PerValueSize.push_back(sizeof(Val[0]));
	SavingValueInformation.ValueCount = size;
}

void DataContainerSection::StringValue::LoadFromBinaryInputStream(std::ifstream& InputFile)
{
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Name.resize(size);
	InputFile.read(&Name[0], size);
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Val.resize(size);
	InputFile.read(&Val[0], size);
}

DataContainerSection::StringVector::StringVector()
{
}

void DataContainerSection::StringVector::SaveToBinaryOutputStream(std::ofstream& OutputFile)
{
	size_t size = Name.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&Name[0], size);
	size = Val.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for(int i = 0; i < Val.size(); i++)
	{
		size = Val[i].size();
		OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
		if(i == 0)
		{
			SavingValueInformation.OutputStreamPositionBeforeSaving = OutputFile.tellp();
		}
		OutputFile.write(&Val[i][0], size);
		SavingValueInformation.PerValueSize.push_back(size);
	}
	SavingValueInformation.OutputStreamPositionAfterSaving = OutputFile.tellp();
	SavingValueInformation.SizeInBytes = SavingValueInformation.OutputStreamPositionAfterSaving - SavingValueInformation.OutputStreamPositionBeforeSaving;
	SavingValueInformation.ValueCount = Val.size();
}

void DataContainerSection::StringVector::LoadFromBinaryInputStream(std::ifstream& InputFile)
{
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Name.resize(size);
	InputFile.read(&Name[0], size);
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	Val.resize(size);
	for (int i = 0; i < Val.size(); i++)
	{
		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		InputFile.read(reinterpret_cast<char*> (&Val[i][0]), size * sizeof(Val[i][0]));
	}
			
}

DataContainerSection::IntVector::IntVector(std::string ValName, std::vector<int> Value)
{
	Name = ValName;
	Val = Value;
}

DataContainerSection::IntVector::IntVector(std::ifstream& InputFile)
{
	LoadFromBinaryInputStream(InputFile);
}

DataContainerSection::FloatValue::FloatValue(std::string ValName, float Value)
{
	Name = ValName;
	Val = Value;
}

DataContainerSection::FloatValue::FloatValue(std::ifstream& InputFile)
{
	LoadFromBinaryInputStream(InputFile);
}

DataContainerSection::FloatVector::FloatVector(std::string ValName, std::vector<float> Value)
{
	Name = ValName;
	Val = Value;
}

DataContainerSection::FloatVector::FloatVector(std::ifstream& InputFile)
{
	LoadFromBinaryInputStream(InputFile);
}

DataContainerSection::BoolValue::BoolValue(std::string ValName, bool Value)
{
	Name = ValName;
	Val = Value;
}

DataContainerSection::BoolValue::BoolValue(std::ifstream& InputFile)
{
	LoadFromBinaryInputStream(InputFile);
}

DataContainerSection::BoolVector::BoolVector(std::string ValName, std::vector<bool> Value)
{
	Name = ValName;
	Val = Value;
}

DataContainerSection::BoolVector::BoolVector(std::ifstream& InputFile)
{
	LoadFromBinaryInputStream(InputFile);
}

DataContainerSection::StringValue::StringValue(std::string ValName, std::string Value)
{
	Name = ValName;
	Val = Value;
}

DataContainerSection::StringValue::StringValue(std::ifstream& InputFile)
{
	LoadFromBinaryInputStream(InputFile);
}

DataContainerSection::StringVector::StringVector(std::string ValName, std::vector<std::string> Value)
{
	Name = ValName;
	Val = Value;
}

DataContainerSection::StringVector::StringVector(std::ifstream& InputFile)
{
	LoadFromBinaryInputStream(InputFile);
}

DataContainerSection::DataContainerSection()
{
	SectionName = "";
	SectionPath.clear();
	SectionIndiciePath.clear();

	IntVals.clear();
	IntVectors.clear();
	FloatVals.clear();
	FloatVectors.clear();
	BoolVals.clear();
	BoolVectors.clear();
	StringVals.clear();
	StringVectors.clear();

	ChildSections.clear();

}

DataContainerSection::DataContainerSection(std::ifstream& InputFile)
{
	LoadFromBinaryInputStream(InputFile);
}

DataContainerSection::DataContainerSection(std::string sectionName)
{
	SectionName = sectionName;
	SectionPath.clear();
	SectionIndiciePath.clear();

	IntVals.clear();
	IntVectors.clear();
	FloatVals.clear();
	FloatVectors.clear();
	BoolVals.clear();
	BoolVectors.clear();
	StringVals.clear();
	StringVectors.clear();

	ChildSections.clear();

}

bool DataContainerSection::AddIntVal(std::string ValueName, int Val)
{
	if (!CheckIfIntValueExist(ValueName))
	{
		IntVals.push_back(IntValue(ValueName, Val));
	}
	else
	{
		return false;
	}

	return true;

}

bool DataContainerSection::AddFloatVal(std::string ValueName, float Val)
{
	if (!CheckIfFloatValueExist(ValueName))
	{
		FloatVals.push_back(FloatValue(ValueName, Val));
	}
	else
	{
		return false;
	}

	return true;
}

bool DataContainerSection::AddBoolVal(std::string ValueName, bool Val)
{
	if (!CheckIfBoolValueExist(ValueName))
	{
		BoolVals.push_back(BoolValue(ValueName, Val));
	}
	else
	{
		return false;
	}

	return true;
}

bool DataContainerSection::AddStringVal(std::string ValueName, std::string Val)
{
	if (!CheckIfStringValueExist(ValueName))
	{
		StringVals.push_back(StringValue(ValueName, Val));
	}
	else
	{
		return false;
	}

	return true;
}

bool DataContainerSection::AddIntVector(std::string ValueName, std::vector<int> Val)
{
	if (!CheckIfIntVectorExist(ValueName))
	{
		IntVectors.push_back(IntVector(ValueName, Val));
	}
	else
	{
		return false;
	}

	return true;
}

bool DataContainerSection::AddFloatVector(std::string ValueName, std::vector<float> Val)
{
	if (!CheckIfFloatVectorExist(ValueName))
	{
		FloatVectors.push_back(FloatVector(ValueName, Val));
	}
	else
	{
		return false;
	}

	return true;
}

bool DataContainerSection::AddBoolVector(std::string ValueName, std::vector<bool> Val)
{
	if (!CheckIfBoolVectorExist(ValueName))
	{
		BoolVectors.push_back(BoolVector(ValueName, Val));
	}
	else
	{
		return false;
	}

	return true;
}

bool DataContainerSection::AddStringVector(std::string ValueName, std::vector<std::string> Val)
{
	if (!CheckIfStringVectorExist(ValueName))
	{
		StringVectors.push_back(StringVector(ValueName, Val));
	}
	else
	{
		return false;
	}

	return true;
}

bool DataContainerSection::GetIntVal(std::string ValueName, int & Value)
{
	bool FoundValue = false;

	for (IntValue it : IntVals)
	{
		if (it.Name == ValueName)
		{
			Value = it.Val;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetIntVal(int Index, int & Value)
{
	bool FoundValue = false;

	if (Index < IntVals.size())
	{
		Value = IntVals[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetIntValSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value)
{
	bool FoundValue = false;

	for (IntValue it : IntVals)
	{
		if (it.Name == ValueName)
		{
			Value = it.SavingValueInformation;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetIntValName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < IntVals.size())
	{
		ValueName = IntVals[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetIntVector(std::string ValueName, std::vector<int>& Value)
{
	bool FoundValue = false;

	for (IntVector it : IntVectors)
	{
		if (it.Name == ValueName)
		{
			Value = it.Val;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetIntVector(int Index, std::vector<int>& Value)
{
	bool FoundValue = false;

	if (Index < IntVectors.size())
	{
		Value = IntVectors[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetIntVectorSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value)
{
	bool FoundValue = false;

	for (IntVector it : IntVectors)
	{
		if (it.Name == ValueName)
		{
			Value = it.SavingValueInformation;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetIntVectorName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < IntVectors.size())
	{
		ValueName = IntVectors[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetFloatVal(std::string ValueName, float & Value)
{
	bool FoundValue = false;

	for (FloatValue it : FloatVals)
	{
		if (it.Name == ValueName)
		{
			Value = it.Val;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetFloatVal(int Index, float & Value)
{
	bool FoundValue = false;

	if (Index < FloatVals.size())
	{
		Value = FloatVals[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetFloatValSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value)
{
	bool FoundValue = false;

	for (FloatValue it : FloatVals)
	{
		if (it.Name == ValueName)
		{
			Value = it.SavingValueInformation;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetFloatValName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < FloatVals.size())
	{
		ValueName = FloatVals[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetFloatVector(std::string ValueName, std::vector<float>& Value)
{
	bool FoundValue = false;

	for (FloatVector it : FloatVectors)
	{
		if (it.Name == ValueName)
		{
			Value = it.Val;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetFloatVector(int Index, std::vector<float>& Value)

{
	bool FoundValue = false;

	if (Index < FloatVectors.size())
	{
		Value = FloatVectors[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetFloatVectorSavingValueInformation(std::string ValueName,
	BinarySavingValueInformation& Value)
{
	bool FoundValue = false;

	for (FloatVector it : FloatVectors)
	{
		if (it.Name == ValueName)
		{
			Value = it.SavingValueInformation;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetFloatVectorName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < FloatVectors.size())
	{
		ValueName = FloatVectors[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetBoolVal(std::string ValueName, bool & Value)
{
	bool FoundValue = false;

	for (BoolValue it : BoolVals)
	{
		if (it.Name == ValueName)
		{
			Value = it.Val;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetBoolVal(int Index, bool & Value)
{
	bool FoundValue = false;

	if (Index < BoolVals.size())
	{
		Value = BoolVals[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetBoolValSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value)
{
	bool FoundValue = false;

	for (BoolValue it : BoolVals)
	{
		if (it.Name == ValueName)
		{
			Value = it.SavingValueInformation;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetBoolValName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < BoolVals.size())
	{
		ValueName = BoolVals[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetBoolVector(std::string ValueName, std::vector<bool>& Value)
{
	bool FoundValue = false;

	for (BoolVector it : BoolVectors)
	{
		if (it.Name == ValueName)
		{
			Value = it.Val;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetBoolVector(int Index, std::vector<bool>& Value)
{
	bool FoundValue = false;

	if (Index < BoolVectors.size())
	{
		Value = BoolVectors[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetBoolVectorSavingValueInformation(std::string ValueName,
	BinarySavingValueInformation& Value)
{
	bool FoundValue = false;

	for (BoolVector it : BoolVectors)
	{
		if (it.Name == ValueName)
		{
			Value = it.SavingValueInformation;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetBoolVectorName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < BoolVectors.size())
	{
		ValueName = BoolVectors[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetStringVal(std::string ValueName, std::string & Value)
{
	bool FoundValue = false;

	for (StringValue it : StringVals)
	{
		if (it.Name == ValueName)
		{
			Value = it.Val;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetStringVal(int Index, std::string & Value)
{
	bool FoundValue = false;

	if (Index < StringVals.size())
	{
		Value = StringVals[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetStringValSavingValueInformation(std::string ValueName, BinarySavingValueInformation& Value)
{
	bool FoundValue = false;

	for (StringValue it : StringVals)
	{
		if (it.Name == ValueName)
		{
			Value = it.SavingValueInformation;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetStringValName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < StringVals.size())
	{
		ValueName = StringVals[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetStringVector(std::string ValueName, std::vector<std::string>& Value)
{
	bool FoundValue = false;

	for (StringVector it : StringVectors)
	{
		if (it.Name == ValueName)
		{
			Value = it.Val;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetStringVector(int Index, std::vector<std::string>& Value)
{
	bool FoundValue = false;

	if (Index < StringVectors.size())
	{
		Value = StringVectors[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool DataContainerSection::GetStringVectorSavingValueInformation(std::string ValueName,
	BinarySavingValueInformation& Value)
{
	bool FoundValue = false;

	for (StringVector it : StringVectors)
	{
		if (it.Name == ValueName)
		{
			Value = it.SavingValueInformation;
			FoundValue = true;
		}
	}

	return FoundValue;
}

bool DataContainerSection::GetStringVectorName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < StringVectors.size())
	{
		ValueName = StringVectors[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

int DataContainerSection::GetNumberOfIntVals()
{
	return IntVals.size();
}

int DataContainerSection::GetNumberOfIntVectors()
{
	return IntVectors.size();
}

int DataContainerSection::GetNumberOfFloatVals()
{
	return FloatVals.size();
}

int DataContainerSection::GetNumberOfFloatVectors()
{
	return FloatVectors.size();
}

int DataContainerSection::GetNumberOfBoolVals()
{
	return BoolVals.size();
}

int DataContainerSection::GetNumberOfBoolVectors()
{
	return BoolVectors.size();
}

int DataContainerSection::GetNumberOfStringVals()
{
	return StringVals.size();
}

int DataContainerSection::GetNumberOfStringVectors()
{
	return StringVectors.size();
}

bool DataContainerSection::CheckIfValueExist(std::string ValueName)
{

	auto itBoolVal = std::find_if(BoolVals.begin(), BoolVals.end(), [&ValueName](const BoolValue& obj) {return obj.Name == ValueName; });

	if (itBoolVal != BoolVals.end())
	{
		return true;
	}

	auto itBoolVector = std::find_if(BoolVectors.begin(), BoolVectors.end(), [&ValueName](const BoolVector& obj) {return obj.Name == ValueName; });

	if (itBoolVector != BoolVectors.end())
	{
		return true;
	}

	auto itFloatValue = std::find_if(FloatVals.begin(), FloatVals.end(), [&ValueName](const FloatValue& obj) {return obj.Name == ValueName; });

	if (itFloatValue != FloatVals.end())
	{
		return true;
	}

	auto itFloatVector = std::find_if(FloatVectors.begin(), FloatVectors.end(), [&ValueName](const FloatVector& obj) {return obj.Name == ValueName; });

	if (itFloatVector != FloatVectors.end())
	{
		return true;
	}

	auto itIntValue = std::find_if(IntVals.begin(), IntVals.end(), [&ValueName](const IntValue& obj) {return obj.Name == ValueName; });

	if (itIntValue != IntVals.end())
	{
		return true;
	}

	auto itIntVector = std::find_if(IntVectors.begin(), IntVectors.end(), [&ValueName](const IntVector& obj) {return obj.Name == ValueName; });

	if (itIntVector != IntVectors.end())
	{
		return true;
	}

	auto itStringValue = std::find_if(StringVals.begin(), StringVals.end(), [&ValueName](const StringValue& obj) {return obj.Name == ValueName; });

	if (itStringValue != StringVals.end())
	{
		return true;
	}

	auto itStringVector = std::find_if(StringVectors.begin(), StringVectors.end(), [&ValueName](const StringVector& obj) {return obj.Name == ValueName; });

	if (itStringVector != StringVectors.end())
	{
		return true;
	}

	return false;
}

bool DataContainerSection::CheckIfIntValueExist(std::string ValueName)
{
	auto itIntValue = std::find_if(IntVals.begin(), IntVals.end(), [&ValueName](const IntValue& obj) {return obj.Name == ValueName; });

	if (itIntValue != IntVals.end())
	{
		return true;
	}

	return false;
}

bool DataContainerSection::CheckIfIntVectorExist(std::string ValueName)
{
	auto itIntVector = std::find_if(IntVectors.begin(), IntVectors.end(), [&ValueName](const IntVector& obj) {return obj.Name == ValueName; });

	if (itIntVector != IntVectors.end())
	{
		return true;
	}

	return false;
}

bool DataContainerSection::CheckIfFloatValueExist(std::string ValueName)
{
	auto itFloatValue = std::find_if(FloatVals.begin(), FloatVals.end(), [&ValueName](const FloatValue& obj) {return obj.Name == ValueName; });

	if (itFloatValue != FloatVals.end())
	{
		return true;
	}

	return false;
}

bool DataContainerSection::CheckIfFloatVectorExist(std::string ValueName)
{
	auto itFloatVector = std::find_if(FloatVectors.begin(), FloatVectors.end(), [&ValueName](const FloatVector& obj) {return obj.Name == ValueName; });

	if (itFloatVector != FloatVectors.end())
	{
		return true;
	}

	return false;
}

bool DataContainerSection::CheckIfBoolValueExist(std::string ValueName)
{
	auto itBoolVal = std::find_if(BoolVals.begin(), BoolVals.end(), [&ValueName](const BoolValue& obj) {return obj.Name == ValueName; });

	if (itBoolVal != BoolVals.end())
	{
		return true;
	}

	return false;
}

bool DataContainerSection::CheckIfBoolVectorExist(std::string ValueName)
{

	auto itBoolVector = std::find_if(BoolVectors.begin(), BoolVectors.end(), [&ValueName](const BoolVector& obj) {return obj.Name == ValueName; });

	if (itBoolVector != BoolVectors.end())
	{
		return true;
	}

	return false;
}

bool DataContainerSection::CheckIfStringValueExist(std::string ValueName)
{
	auto itStringValue = std::find_if(StringVals.begin(), StringVals.end(), [&ValueName](const StringValue& obj) {return obj.Name == ValueName; });

	if (itStringValue != StringVals.end())
	{
		return true;
	}

	return false;
}

bool DataContainerSection::CheckIfStringVectorExist(std::string ValueName)
{

	auto itStringVector = std::find_if(StringVectors.begin(), StringVectors.end(), [&ValueName](const StringVector& obj) {return obj.Name == ValueName; });

	if (itStringVector != StringVectors.end())
	{
		return true;
	}

	return false;
}

void DataContainerSection::SaveValueSavingInformationToBinaryOutputStream(std::ofstream& OutputFile)
{
	size_t size = SectionName.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&SectionName[0], size);

	size = SectionPath.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < SectionPath.size(); i++)
	{
		size = SectionPath[i].size();
		OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
		OutputFile.write(&SectionPath[i][0], size);
	}

	size = SectionIndiciePath.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(reinterpret_cast<char*>(&SectionIndiciePath[0]), size * sizeof(SectionIndiciePath[0]));

	size = IntVals.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < IntVals.size(); i++)
	{
		IntVals[i].SavingValueInformation.SaveToBinaryOutputStream(IntVals[i].Name, OutputFile);
	}

	size = IntVectors.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < IntVectors.size(); i++)
	{
		IntVectors[i].SavingValueInformation.SaveToBinaryOutputStream(IntVectors[i].Name, OutputFile);
	}

	size = FloatVals.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < FloatVals.size(); i++)
	{
		FloatVals[i].SavingValueInformation.SaveToBinaryOutputStream(FloatVals[i].Name, OutputFile);
	}

	size = FloatVectors.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < FloatVectors.size(); i++)
	{
		FloatVectors[i].SavingValueInformation.SaveToBinaryOutputStream(FloatVectors[i].Name, OutputFile);
	}

	size = BoolVals.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < BoolVals.size(); i++)
	{
		BoolVals[i].SavingValueInformation.SaveToBinaryOutputStream(BoolVals[i].Name, OutputFile);
	}

	size = BoolVectors.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < BoolVectors.size(); i++)
	{
		BoolVectors[i].SavingValueInformation.SaveToBinaryOutputStream(BoolVectors[i].Name, OutputFile);
	}

	size = StringVals.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < StringVals.size(); i++)
	{
		StringVals[i].SavingValueInformation.SaveToBinaryOutputStream(StringVals[i].Name, OutputFile);
	}

	size = StringVectors.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < StringVectors.size(); i++)
	{
		StringVectors[i].SavingValueInformation.SaveToBinaryOutputStream(StringVectors[i].Name, OutputFile);
	}

	size = ChildSections.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < ChildSections.size(); i++)
	{
		ChildSections[i].SaveValueSavingInformationToBinaryOutputStream(OutputFile);
	}

}

void DataContainerSection::LoadValueSavingInformationToBinaryOutputStream(std::ifstream& InputFile)
{
	std::string sectionName;
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	sectionName.resize(size);
	InputFile.read(&sectionName[0], size);
	if(sectionName == SectionName)
	{
		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (int i = 0; i < SectionPath.size(); i++)
		{
			InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
			SectionPath[i].resize(size);
			InputFile.read(&sectionName[0], size);
		}
		std::vector<int> temp;
		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		temp.resize(size);
		InputFile.read(reinterpret_cast<char*>(&temp[0]), size * sizeof(SectionIndiciePath[0]));

		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			IntVals[i].SavingValueInformation.LoadFromBinaryInputStream(InputFile);
		}

		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			IntVectors[i].SavingValueInformation.LoadFromBinaryInputStream(InputFile);
		}

		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			FloatVals[i].SavingValueInformation.LoadFromBinaryInputStream(InputFile);
		}

		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			FloatVectors[i].SavingValueInformation.LoadFromBinaryInputStream(InputFile);
		}

		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			BoolVals[i].SavingValueInformation.LoadFromBinaryInputStream(InputFile);
		}

		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			BoolVectors[i].SavingValueInformation.LoadFromBinaryInputStream(InputFile);
		}

		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (int i = 0; i < size; i++)
		{
			StringVals[i].SavingValueInformation.LoadFromBinaryInputStream(InputFile);
		}

		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		for (int i = 0; i < StringVectors.size(); i++)
		{
			StringVectors[i].SavingValueInformation.LoadFromBinaryInputStream(InputFile);
		}

		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		ChildSections.resize(size);
		for (int i = 0; i < size; i++)
		{
			ChildSections[i].SavingValueInformation.LoadFromBinaryInputStream(InputFile);
		}
	}
}

void DataContainerSection::SaveToBinaryOutputStream(std::ofstream& OutputFile)
{
	size_t size = SectionName.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(&SectionName[0], size);

	size = SectionPath.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < SectionPath.size(); i++)
	{
		size = SectionPath[i].size();
		OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
		OutputFile.write(&SectionPath[i][0], size);
	}

	size = SectionIndiciePath.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	OutputFile.write(reinterpret_cast<char*>(&SectionIndiciePath[0]), size * sizeof(SectionIndiciePath[0]));

	SavingValueInformation.OutputStreamPositionBeforeSaving = OutputFile.tellp();

	size = IntVals.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < IntVals.size(); i++)
	{
		IntVals[i].SaveToBinaryOutputStream(OutputFile);
	}

	size = IntVectors.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < IntVectors.size(); i++)
	{
		IntVectors[i].SaveToBinaryOutputStream(OutputFile);
	}

	size = FloatVals.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < FloatVals.size(); i++)
	{
		FloatVals[i].SaveToBinaryOutputStream(OutputFile);
	}

	size = FloatVectors.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < FloatVectors.size(); i++)
	{
		FloatVectors[i].SaveToBinaryOutputStream(OutputFile);
	}

	size = BoolVals.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < BoolVals.size(); i++)
	{
		BoolVals[i].SaveToBinaryOutputStream(OutputFile);
	}

	size = BoolVectors.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < BoolVectors.size(); i++)
	{
		BoolVectors[i].SaveToBinaryOutputStream(OutputFile);
	}

	size = StringVals.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < StringVals.size(); i++)
	{
		StringVals[i].SaveToBinaryOutputStream(OutputFile);
	}

	size = StringVectors.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < StringVectors.size(); i++)
	{
		StringVectors[i].SaveToBinaryOutputStream(OutputFile);
	}

	SavingValueInformation.OutputStreamPositionAfterSaving = OutputFile.tellp();
	SavingValueInformation.SizeInBytes = SavingValueInformation.OutputStreamPositionAfterSaving - SavingValueInformation.OutputStreamPositionBeforeSaving;

	size = ChildSections.size();
	OutputFile.write(reinterpret_cast<char*>(&size), sizeof(size));
	for (int i = 0; i < ChildSections.size(); i++)
	{
		ChildSections[i].SaveToBinaryOutputStream(OutputFile);
	}

	
}

void DataContainerSection::LoadFromBinaryInputStream(std::ifstream& InputFile)
{
	size_t size;
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	SectionName.resize(size);
	InputFile.read(&SectionName[0], size);
	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	SectionPath.resize(size);
	for (int i = 0; i < SectionPath.size(); i++)
	{
		InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
		SectionPath[i].resize(size);
		InputFile.read(&SectionPath[i][0], size);
	}

	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	SectionIndiciePath.resize(size);
	InputFile.read(reinterpret_cast<char*>(&SectionIndiciePath[0]), size * sizeof(SectionIndiciePath[0]));

	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	IntVals.resize(size);
	for (int i = 0; i < size; i++)
	{
		IntVals[i] = IntValue(InputFile);
	}

	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	IntVectors.resize(size);
	for (int i = 0; i < size; i++)
	{
		IntVectors[i] = IntVector(InputFile);
	}

	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	FloatVals.resize(size);
	for (int i = 0; i < size; i++)
	{
		FloatVals[i] = FloatValue(InputFile);
	}

	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	FloatVectors.resize(size);
	for (int i = 0; i < size; i++)
	{
		FloatVectors[i] = FloatVector(InputFile);
	}

	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	BoolVals.resize(size);
	for (int i = 0; i < size; i++)
	{
		BoolVals[i] = BoolValue(InputFile);
	}

	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	BoolVectors.resize(size);
	for (int i = 0; i < size; i++)
	{
		BoolVectors[i] = BoolVector(InputFile);
	}

	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	StringVals.resize(size);
	for (int i = 0; i < size; i++)
	{
		StringVals[i] = StringValue(InputFile);
	}

	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	StringVectors.resize(size);
	for (int i = 0; i < size; i++)
	{
		StringVectors[i] = StringVector(InputFile);
	}

	InputFile.read(reinterpret_cast<char*>(&size), sizeof(size));
	ChildSections.resize(size);
	for (int i = 0; i < size; i++)
	{
		ChildSections[i] = DataContainerSection(InputFile);
	}
}

