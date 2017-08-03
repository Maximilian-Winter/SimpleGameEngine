#include "ConfigDataSection.h"

ConfigDataSection::IntValue::IntValue(std::string ValName, int Value)
{
	Name = ValName;
	Val = Value;
}

ConfigDataSection::IntVector::IntVector(std::string ValName, std::vector<int> Value)
{
	Name = ValName;
	Val = Value;
}

ConfigDataSection::FloatValue::FloatValue(std::string ValName, float Value)
{
	Name = ValName;
	Val = Value;
}

ConfigDataSection::FloatVector::FloatVector(std::string ValName, std::vector<float> Value)
{
	Name = ValName;
	Val = Value;
}

ConfigDataSection::BoolValue::BoolValue(std::string ValName, bool Value)
{
	Name = ValName;
	Val = Value;
}

ConfigDataSection::BoolVector::BoolVector(std::string ValName, std::vector<bool> Value)
{
	Name = ValName;
	Val = Value;
}

ConfigDataSection::StringValue::StringValue(std::string ValName, std::string Value)
{
	Name = ValName;
	Val = Value;
}

ConfigDataSection::StringVector::StringVector(std::string ValName, std::vector<std::string> Value)
{
	Name = ValName;
	Val = Value;
}

ConfigDataSection::ConfigDataSection()
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

ConfigDataSection::ConfigDataSection(std::string sectionName)
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

bool ConfigDataSection::AddIntVal(std::string ValueName, int Val)
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

bool ConfigDataSection::AddFloatVal(std::string ValueName, float Val)
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

bool ConfigDataSection::AddBoolVal(std::string ValueName, bool Val)
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

bool ConfigDataSection::AddStringVal(std::string ValueName, std::string Val)
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

bool ConfigDataSection::AddIntVector(std::string ValueName, std::vector<int> Val)
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

bool ConfigDataSection::AddFloatVector(std::string ValueName, std::vector<float> Val)
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

bool ConfigDataSection::AddBoolVector(std::string ValueName, std::vector<bool> Val)
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

bool ConfigDataSection::AddStringVector(std::string ValueName, std::vector<std::string> Val)
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

bool ConfigDataSection::GetIntVal(std::string ValueName, int & Value)
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

bool ConfigDataSection::GetIntVal(int Index, int & Value)
{
	bool FoundValue = false;

	if (Index < IntVals.size())
	{
		Value = IntVals[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetIntValName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < IntVals.size())
	{
		ValueName = IntVals[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetIntVector(std::string ValueName, std::vector<int>& Value)
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

bool ConfigDataSection::GetIntVector(int Index, std::vector<int>& Value)
{
	bool FoundValue = false;

	if (Index < IntVectors.size())
	{
		Value = IntVectors[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetIntVectorName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < IntVectors.size())
	{
		ValueName = IntVectors[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetFloatVal(std::string ValueName, float & Value)
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

bool ConfigDataSection::GetFloatVal(int Index, float & Value)
{
	bool FoundValue = false;

	if (Index < FloatVals.size())
	{
		Value = FloatVals[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetFloatValName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < FloatVals.size())
	{
		ValueName = FloatVals[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetFloatVector(std::string ValueName, std::vector<float>& Value)
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

bool ConfigDataSection::GetFloatVector(int Index, std::vector<float>& Value)

{
	bool FoundValue = false;

	if (Index < FloatVectors.size())
	{
		Value = FloatVectors[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetFloatVectorName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < FloatVectors.size())
	{
		ValueName = FloatVectors[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetBoolVal(std::string ValueName, bool & Value)
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

bool ConfigDataSection::GetBoolVal(int Index, bool & Value)
{
	bool FoundValue = false;

	if (Index < BoolVals.size())
	{
		Value = BoolVals[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetBoolValName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < BoolVals.size())
	{
		ValueName = BoolVals[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetBoolVector(std::string ValueName, std::vector<bool>& Value)
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

bool ConfigDataSection::GetBoolVector(int Index, std::vector<bool>& Value)
{
	bool FoundValue = false;

	if (Index < BoolVectors.size())
	{
		Value = BoolVectors[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetBoolVectorName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < BoolVectors.size())
	{
		ValueName = BoolVectors[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetStringVal(std::string ValueName, std::string & Value)
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

bool ConfigDataSection::GetStringVal(int Index, std::string & Value)
{
	bool FoundValue = false;

	if (Index < StringVals.size())
	{
		Value = StringVals[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetStringValName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < StringVals.size())
	{
		ValueName = StringVals[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetStringVector(std::string ValueName, std::vector<std::string>& Value)
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

bool ConfigDataSection::GetStringVector(int Index, std::vector<std::string>& Value)
{
	bool FoundValue = false;

	if (Index < StringVectors.size())
	{
		Value = StringVectors[Index].Val;
		FoundValue = true;
	}

	return FoundValue;
}

bool ConfigDataSection::GetStringVectorName(int Index, std::string & ValueName)
{
	bool FoundValue = false;

	if (Index < StringVectors.size())
	{
		ValueName = StringVectors[Index].Name;
		FoundValue = true;
	}

	return FoundValue;
}

int ConfigDataSection::GetNumberOfIntVals()
{
	return IntVals.size();
}

int ConfigDataSection::GetNumberOfIntVectors()
{
	return IntVectors.size();
}

int ConfigDataSection::GetNumberOfFloatVals()
{
	return FloatVals.size();
}

int ConfigDataSection::GetNumberOfFloatVectors()
{
	return FloatVectors.size();
}

int ConfigDataSection::GetNumberOfBoolVals()
{
	return BoolVals.size();
}

int ConfigDataSection::GetNumberOfBoolVectors()
{
	return BoolVectors.size();
}

int ConfigDataSection::GetNumberOfStringVals()
{
	return StringVals.size();
}

int ConfigDataSection::GetNumberOfStringVectors()
{
	return StringVectors.size();
}

bool ConfigDataSection::CheckIfValueExist(std::string ValueName)
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

bool ConfigDataSection::CheckIfIntValueExist(std::string ValueName)
{
	auto itIntValue = std::find_if(IntVals.begin(), IntVals.end(), [&ValueName](const IntValue& obj) {return obj.Name == ValueName; });

	if (itIntValue != IntVals.end())
	{
		return true;
	}

	return false;
}

bool ConfigDataSection::CheckIfIntVectorExist(std::string ValueName)
{
	auto itIntVector = std::find_if(IntVectors.begin(), IntVectors.end(), [&ValueName](const IntVector& obj) {return obj.Name == ValueName; });

	if (itIntVector != IntVectors.end())
	{
		return true;
	}

	return false;
}

bool ConfigDataSection::CheckIfFloatValueExist(std::string ValueName)
{
	auto itFloatValue = std::find_if(FloatVals.begin(), FloatVals.end(), [&ValueName](const FloatValue& obj) {return obj.Name == ValueName; });

	if (itFloatValue != FloatVals.end())
	{
		return true;
	}

	return false;
}

bool ConfigDataSection::CheckIfFloatVectorExist(std::string ValueName)
{
	auto itFloatVector = std::find_if(FloatVectors.begin(), FloatVectors.end(), [&ValueName](const FloatVector& obj) {return obj.Name == ValueName; });

	if (itFloatVector != FloatVectors.end())
	{
		return true;
	}

	return false;
}

bool ConfigDataSection::CheckIfBoolValueExist(std::string ValueName)
{
	auto itBoolVal = std::find_if(BoolVals.begin(), BoolVals.end(), [&ValueName](const BoolValue& obj) {return obj.Name == ValueName; });

	if (itBoolVal != BoolVals.end())
	{
		return true;
	}

	return false;
}

bool ConfigDataSection::CheckIfBoolVectorExist(std::string ValueName)
{

	auto itBoolVector = std::find_if(BoolVectors.begin(), BoolVectors.end(), [&ValueName](const BoolVector& obj) {return obj.Name == ValueName; });

	if (itBoolVector != BoolVectors.end())
	{
		return true;
	}

	return false;
}

bool ConfigDataSection::CheckIfStringValueExist(std::string ValueName)
{
	auto itStringValue = std::find_if(StringVals.begin(), StringVals.end(), [&ValueName](const StringValue& obj) {return obj.Name == ValueName; });

	if (itStringValue != StringVals.end())
	{
		return true;
	}

	return false;
}

bool ConfigDataSection::CheckIfStringVectorExist(std::string ValueName)
{

	auto itStringVector = std::find_if(StringVectors.begin(), StringVectors.end(), [&ValueName](const StringVector& obj) {return obj.Name == ValueName; });

	if (itStringVector != StringVectors.end())
	{
		return true;
	}

	return false;
}

