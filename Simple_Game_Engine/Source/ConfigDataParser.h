// Filename: ConfigDataParser.h
// Date: 07.06.2015
// Author: Maximilian Winter
#ifndef _CONFIGDATAPARSER_H_
#define _CONFIGDATAPARSER_H_


//Includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <istream>
#include <vector>
#include <algorithm>

class ConfigDataParser
{
public:
	ConfigDataParser();
	~ConfigDataParser();
	//Helper Function to parse the Configfile
	static void DeleteWhitespacesFromLine(std::string &line);
	static void DeleteCharFromLine(std::string &line, char Char);
	static void DeleteOpenAndCloseingCurlyBracesFromLine(std::string &line);
	static void DeleteOpenAndCloseingQuotationMarkFromLine(std::string &line);
	static bool HasOpenAndCloseingCurlyBraces(const std::string &line);
	static bool HasOpenAndCloseingQuotationMarks(const std::string &line);
	static bool HasOpenAndCloseingSquareBrackets(const std::string &line);
	static bool IsOnlyWhitespacesLine(const std::string &line);
	static void RemoveCommentFromLine(std::string &line);
	static bool IsSectionNameLine(const std::string &line);
	static bool IsSectionNameInput(const std::string &input);
	static bool IsChildSectionNameLine(const std::string &line);
	static bool IsChildSectionNameInput(const std::string &input);
	static bool IsValueLine(const std::string &line);
	static bool IsIntValueLine(const std::string &line);
	static bool IsFloatValueLine(const std::string &line);
	static bool IsBoolValueLine(const std::string &line);
	static bool IsStringValueLine(const std::string &line);
	static bool IsIntVectorLine(const std::string &line);
	static bool IsFloatVectorLine(const std::string &line);
	static bool IsBoolVectorLine(const std::string &line);
	static bool IsStringVectorLine(const std::string &line);
	static bool IsNumericValue(const std::string &str);

	static std::string GetSectionNameFromLine(const std::string &line);
	static std::vector<std::string> GetChildSectionNameListFromLine(const std::string &line);
	static std::vector<std::string> GetSectionNameListFromFunctionArg(const std::string &funcarg);
	static std::string GetValueNameFromLine(const std::string &line);
	static int GetIntValueFromLine(const std::string &line);
	static float GetFloatValueFromLine(const std::string &line);
	static bool GetBoolValueFromLine(const std::string &line);
	static std::string GetStringValueFromLine(const std::string &line);
	static std::vector<int> GetIntVectorFromLine(const std::string &line);
	static std::vector<float> GetFloatVectorFromLine(const std::string &line);
	static std::vector<bool> GetBoolVectorFromLine(const std::string &line);
	static std::vector<std::string> GetStringVectorFromLine(const std::string &line);
};

#endif