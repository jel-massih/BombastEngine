#pragma once

#include <map>

typedef std::map<std::string, size_t> DevContentsMap;		// maps path to a development content id

class DevelopmentPackageResource
{
public:
	struct AssetFileInfo
	{
		WIN32_FIND_DATA data;
		std::wstring resourceName;
	};

public:
	bool Init(const std::wstring &basePackagePath);

	size_t GetNumFiles()const { return m_assetsFileInfo.size(); }
	std::string GetFilename(size_t i) const;
	int GetFileLen(size_t i) const;
	bool ReadFile(size_t i, char* pBuffer);

	size_t Find(const std::string &path) const;

protected:
	void ReadPackageDirectory(std::wstring fileSpec);

private:
	std::wstring m_basePackagePath;

	std::vector<AssetFileInfo> m_assetsFileInfo;
	DevContentsMap m_contentsMap;
};