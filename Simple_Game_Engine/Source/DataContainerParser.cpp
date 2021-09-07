#include "DataContainerParser.h"


//Helper Function to parse the Configfile

DataContainerParser::DataContainerParser()
{
}

DataContainerParser::~DataContainerParser()
{
}

void DataContainerParser::DeleteWhitespacesFromLine(std::string & line)
{
	line.erase(std::remove_if(line.begin(), line.end(), ::isspace), line.end());
}

void DataContainerParser::DeleteCharFromLine(std::string & line, char Char)
{
	line.erase(std::remove(line.begin(), line.end(), Char), line.end());
}

void DataContainerParser::DeleteOpenAndCloseingCurlyBracesFromLine(std::string & line)
{
	line.erase(0, 1);
	line.erase(line.size() - 1, 1);
}

void DataContainerParser::DeleteOpenAndCloseingQuotationMarkFromLine(std::string & line)
{
	line.erase(line.begin());
	line.erase(line.size() - 1);
}

bool DataContainerParser::HasOpenAndCloseingCurlyBraces(const std::string & line)
{
	if (line.find_first_of("{") == 0)
	{
		return (line.find_last_of("}") == line.size() - 1);
	}
	else
	{
		return false;
	}
}

bool DataContainerParser::HasOpenAndCloseingQuotationMarks(const std::string & line)
{
	if (line.find_first_of("\"") == 0)
	{
		return (line.find_last_of("\"") == line.size() - 1);
	}
	else
	{
		return false;
	}
}

bool DataContainerParser::HasOpenAndCloseingSquareBrackets(const std::string & line)
{
	if (line.find_first_of("[") == 0)
	{
		return (line.find_last_of("]") == line.size() - 1);
	}
	else
	{
		return false;
	}
}

bool DataContainerParser::IsOnlyWhitespacesLine(const std::string & line)
{
	return (line.find_first_not_of(" ") == line.npos);
}

void DataContainerParser::RemoveCommentFromLine(std::string & line)
{
	if (line.find("#") != line.npos)
	{
		line.erase(line.find("#"));
	}
}

bool DataContainerParser::IsSectionNameLine(const std::string & line)
{
	std::string lineContent = line;
	DeleteWhitespacesFromLine(lineContent);

	if (lineContent.find_first_of("[") == 0)
	{
		if (lineContent.find_first_of("[", lineContent.find_first_of("[") + 1) == std::string::npos)
		{
			if (lineContent.find_first_of("]") == lineContent.size() - 1)
			{
				if (lineContent.find_first_of("]", lineContent.find_first_of("]") + 1) == std::string::npos)
				{
					if (lineContent.find_first_of("=") == std::string::npos)
					{
						return true;
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
	else
	{
		return false;
	}
}

bool DataContainerParser::IsSectionNameInput(const std::string & input)
{
	std::string lineContent = input;
	DeleteWhitespacesFromLine(lineContent);

	if (lineContent.find_first_of(".") == std::string::npos)
	{
		if (lineContent.find_first_of("=") == std::string::npos)
		{
			return true;
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

bool DataContainerParser::IsChildSectionNameLine(const std::string & line)
{
	std::string lineContent = line;
	DeleteWhitespacesFromLine(lineContent);

	if (lineContent.find_first_of("[") == 0)
	{
		if (lineContent.find_first_of("]") != std::string::npos)
		{
			if (lineContent.find_first_of(".") != std::string::npos)
			{
				if (lineContent.find_first_of("=") == std::string::npos)
				{
					size_t n = std::count(lineContent.begin(), lineContent.end(), '.');

					for (int i = 0; i <= n; i++)
					{
						int PointPos = lineContent.find_first_of(".");
						std::string TempValue = lineContent.substr(0, PointPos);

						if (!HasOpenAndCloseingSquareBrackets(TempValue))
						{
							return false;
						}
						lineContent.erase(0, PointPos + 1);
					}
					return true;
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

bool DataContainerParser::IsChildSectionNameInput(const std::string & input)
{
	std::string lineContent = input;
	DeleteWhitespacesFromLine(lineContent);
	if (lineContent.size() > 0)
	{
		if (lineContent.find_first_of("=") == std::string::npos)
		{
			size_t n = std::count(lineContent.begin(), lineContent.end(), '.');
			for (int i = 0; i <= n; i++)
			{
				int PointPos = lineContent.find_first_of(".");
				std::string TempValue = lineContent.substr(0, PointPos);

				if (IsOnlyWhitespacesLine(TempValue))
				{
					return false;
				}
				lineContent.erase(0, PointPos + 1);
			}
			return true;
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

bool DataContainerParser::IsValueLine(const std::string & line)
{
	if (line.find_first_of("=") != std::string::npos)
	{
		std::string value = line;

		DeleteWhitespacesFromLine(value);

		if (value.find_first_of("=") != 0)
		{
			int pos = line.find_first_of("=") + 1;

			if (!(pos >= line.size()))
			{
				return (line.find_first_not_of(" ", pos) != std::string::npos);
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
	else
	{
		return false;
	}
}

bool DataContainerParser::IsIntValueLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);

	if (Value.find_first_of(".") == std::string::npos)
	{
		DeleteWhitespacesFromLine(Value);
		DeleteCharFromLine(Value, '-');
		return std::all_of(Value.begin(), Value.end(), ::isdigit);
	}
	else
	{
		return false;
	}
}

bool DataContainerParser::IsFloatValueLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);


	if (Value.find_first_of(".") != std::string::npos)
	{
		DeleteWhitespacesFromLine(Value);
		DeleteCharFromLine(Value, '.');
		DeleteCharFromLine(Value, '-');

		return std::all_of(Value.begin(), Value.end(), ::isdigit);
	}
	else
	{
		return false;
	}
}

bool DataContainerParser::IsBoolValueLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);

	if (Value == "true" || Value == "True" || Value == "1" || Value == "False" || Value == "false" || Value == "0")
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool DataContainerParser::IsStringValueLine(const std::string & line)
{
	int ValStartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(ValStartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);

	return HasOpenAndCloseingQuotationMarks(Value);
}

bool DataContainerParser::IsIntVectorLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);

	if (Value.find_first_of(".") == std::string::npos)
	{
		DeleteWhitespacesFromLine(Value);
		if (HasOpenAndCloseingCurlyBraces(Value))
		{

			DeleteOpenAndCloseingCurlyBracesFromLine(Value);
			DeleteCharFromLine(Value, ',');
			DeleteCharFromLine(Value, '-');

			return std::all_of(Value.begin(), Value.end(), ::isdigit);
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

bool DataContainerParser::IsFloatVectorLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);

	if (HasOpenAndCloseingCurlyBraces(Value))
	{
		if (Value.find_first_of(".") != std::string::npos)
		{
			DeleteOpenAndCloseingCurlyBracesFromLine(Value);
			DeleteCharFromLine(Value, ',');
			DeleteCharFromLine(Value, '.');
			DeleteCharFromLine(Value, '-');
			DeleteCharFromLine(Value, 'e');
			DeleteCharFromLine(Value, 'E');
			return std::all_of(Value.begin(), Value.end(), ::isdigit);

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

bool DataContainerParser::IsBoolVectorLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);

	if (HasOpenAndCloseingCurlyBraces(Value))
	{
		DeleteOpenAndCloseingCurlyBracesFromLine(Value);

		size_t n = std::count(Value.begin(), Value.end(), ',');

		for (int i = 0; i <= n; i++)
		{
			int CommaPos = Value.find_first_of(",");
			std::string TempValue = Value.substr(0, CommaPos);

			if ((TempValue != "True" && TempValue != "true" && TempValue != "1" && TempValue != "False" && TempValue != "false" && TempValue != "0"))
			{
				return false;
			}
			Value.erase(0, CommaPos + 1);
		}

		return true;
	}
	else
	{
		return false;
	}
}

bool DataContainerParser::IsStringVectorLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);

	if (HasOpenAndCloseingCurlyBraces(Value))
	{
		DeleteOpenAndCloseingCurlyBracesFromLine(Value);

		size_t n = std::count(Value.begin(), Value.end(), ',');

		for (int i = 0; i <= n; i++)
		{
			int CommaPos = Value.find_first_of(",");
			std::string TempValue = Value.substr(0, CommaPos);

			if (!HasOpenAndCloseingQuotationMarks(TempValue))
			{
				return false;
			}
			Value.erase(0, CommaPos + 1);
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool DataContainerParser::IsNumericValue(const std::string & str)
{
	if (str.find_first_of(".") != std::string::npos)
	{
		std::string Temp1 = str.substr(0, str.find_first_of(".") - 1);
		std::string Temp2 = str.substr(str.find_first_of(".") + 1, std::string::npos);

		bool res = std::all_of(Temp1.begin(), Temp1.end(), ::isdigit);
		if (!res)
		{
			return false;
		}

		return std::all_of(Temp2.begin(), Temp2.end(), ::isdigit);
	}

	return std::all_of(str.begin(), str.end(), ::isdigit);
}

std::string DataContainerParser::GetSectionNameFromLine(const std::string & line)
{
	int StartPos = line.find_first_of("[") + 1;
	int Length = line.find_first_of("]") - StartPos;
	std::string SectionName = line.substr(StartPos, Length);
	DeleteWhitespacesFromLine(SectionName);

	return SectionName;
}

std::vector<std::string> DataContainerParser::GetChildSectionNameListFromLine(const std::string & line)
{

	std::vector<std::string> TempSectionNameVector;
	std::string TempLine = line;
	size_t n = std::count(TempLine.begin(), TempLine.end(), '.');

	for (int i = 0; i <= n; i++)
	{
		int CommaPos = TempLine.find_first_of(".");
		std::string TempValue = TempLine.substr(0, CommaPos);

		unsigned first = TempValue.find_first_of("[") + 1;
		unsigned last = TempValue.find_last_of("]");

		TempValue = TempValue.substr(first, last - first);

		TempSectionNameVector.push_back(TempValue);

		TempLine.erase(0, CommaPos + 1);
	}

	return TempSectionNameVector;
}

std::vector<std::string> DataContainerParser::GetSectionNameListFromFunctionArg(const std::string & funcarg)
{

	std::vector<std::string> TempSectionNameVector;
	std::string TempLine = funcarg;
	size_t count = std::count(TempLine.begin(), TempLine.end(), '.');

	for (int i = 0; i <= count; i++)
	{
		int PointPos = TempLine.find_first_of(".");
		std::string TempValue = TempLine.substr(0, PointPos);

		TempSectionNameVector.push_back(TempValue);

		TempLine.erase(0, PointPos + 1);
	}

	return TempSectionNameVector;
}

std::string DataContainerParser::GetValueNameFromLine(const std::string & line)
{
	int length = line.find_first_of("=");
	std::string ValueName = line.substr(0, length);

	DeleteWhitespacesFromLine(ValueName);

	return ValueName;
}

int DataContainerParser::GetIntValueFromLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);

	return std::stoi(Value);
}

float DataContainerParser::GetFloatValueFromLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);

	return std::stof(Value);
}

bool DataContainerParser::GetBoolValueFromLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);

	if (Value == "true")
	{
		return true;
	}
	if (Value == "True")
	{
		return true;
	}
	if (Value == "1")
	{
		return true;
	}
	if (Value == "false")
	{
		return false;
	}
	if (Value == "False")
	{
		return false;
	}
	if (Value == "0")
	{
		return false;
	}

	return false;
}

std::string DataContainerParser::GetStringValueFromLine(const std::string & line)
{
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);

	unsigned first = line.find_first_of("\"") + 1;
	unsigned last = line.find_last_of("\"");

	Value = line.substr(first, last - first);

	//DeleteOpenAndCloseingQuotationMarkFromLine(Value);

	return Value;
}

std::vector<int> DataContainerParser::GetIntVectorFromLine(const std::string & line)
{
	std::vector<int> TempVector;
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);
	DeleteOpenAndCloseingCurlyBracesFromLine(Value);

	std::stringstream ss(Value);

	int i;

	while (ss >> i)
	{
		TempVector.push_back(i);
		if (ss.peek() == ',')
			ss.ignore();
	}

	return TempVector;
}

std::vector<float> DataContainerParser::GetFloatVectorFromLine(const std::string & line)
{
	std::vector<float> TempVector;
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);
	DeleteOpenAndCloseingCurlyBracesFromLine(Value);

	std::stringstream ss(Value);

	float i;

	while (ss >> i)
	{
		TempVector.push_back(i);
		if (ss.peek() == ',')
			ss.ignore();
	}

	return TempVector;
}

std::vector<bool> DataContainerParser::GetBoolVectorFromLine(const std::string & line)
{
	std::vector<bool> TempVector;
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteWhitespacesFromLine(Value);
	DeleteOpenAndCloseingCurlyBracesFromLine(Value);

	size_t n = std::count(Value.begin(), Value.end(), ',');

	for (int i = 0; i <= n; i++)
	{
		int CommaPos = Value.find_first_of(",");
		std::string TempValue = Value.substr(0, CommaPos);

		if (TempValue == "true")
		{
			TempVector.push_back(true);
		}
		if (TempValue == "True")
		{
			TempVector.push_back(true);
		}
		if (TempValue == "1")
		{
			TempVector.push_back(true);
		}
		if (TempValue == "false")
		{
			TempVector.push_back(false);
		}
		if (TempValue == "False")
		{
			TempVector.push_back(false);
		}
		if (TempValue == "0")
		{
			TempVector.push_back(false);
		}

		Value.erase(0, CommaPos + 1);
	}

	return TempVector;
}

std::vector<std::string> DataContainerParser::GetStringVectorFromLine(const std::string & line)
{
	std::vector<std::string> TempVector;
	int StartPos = line.find_first_of("=") + 1;
	std::string Value = line.substr(StartPos, std::string::npos);
	DeleteOpenAndCloseingCurlyBracesFromLine(Value);

	size_t n = std::count(Value.begin(), Value.end(), ',');

	for (int i = 0; i <= n; i++)
	{
		int CommaPos = Value.find_first_of(",");
		std::string TempValue = Value.substr(0, CommaPos);

		unsigned first = TempValue.find_first_of("\"") + 1;
		unsigned last = TempValue.find_last_of("\"");

		TempValue = TempValue.substr(first, last - first);

		TempVector.push_back(TempValue);

		Value.erase(0, CommaPos + 1);
	}

	return TempVector;
}
