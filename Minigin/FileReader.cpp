#include "FileReader.h"
#include <algorithm>
#include <sstream>
#include <vector>
#include <iostream>
#include <type_traits>
#include <cassert>

#define _SILENCE_CXX20_IS_POD_DEPRECATION_WARNING
#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS


dae::FileReader::FileReader(const std::string& filePath)
	: m_File{}
	, m_FileName{ filePath }
{
	ReadFile();
}

dae::FileReader::~FileReader()
{
	m_File.close();
}

const Document& dae::FileReader::ReadFile() {
	Open();
	std::ifstream inputFile(m_FileName);
	if (!inputFile) {
		std::cerr << "Failed to open the input file." << std::endl;
	}

	std::string content;
	std::string line;

	while (std::getline(inputFile, line)) {
		content += line;
	}
	inputFile.close();

	m_Document.Parse(content.c_str());

	if (m_Document.HasParseError()) {
		std::cerr << "Parsing failed. Error code: " << m_Document.GetParseError() << std::endl;
	}

	if (m_Document.IsObject()) {
		const Value& contents = m_Document.GetObject();
		StringBuffer buffer;
		PrettyWriter<StringBuffer> writer(buffer);
		contents.Accept(writer);
		return m_Document;
	}
	else {
		m_Document.SetObject();
	}
	return m_Document;
}

void dae::FileReader::WriteData(std::string dataKey, std::string data)
{
	std::ofstream file(m_FileName);

	if (file.is_open()) {
		file << dataKey + ":" + data << std::endl;
		file << "+";
		file.close();
	}
}


void dae::FileReader::Open()
{
	m_File.open(m_FileName, std::ios::in);
}

void dae::FileReader::Close()
{
	m_File.close();
}