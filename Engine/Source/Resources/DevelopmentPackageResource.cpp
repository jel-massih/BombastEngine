#include "DevelopmentPackageResource.h"

#include <cctype>
#include <fstream>
#include <string.h>

bool DevelopmentPackageResource::Init(const std::string& packageName, const std::wstring &basePackagePath)
{
	m_packageName = packageName;
	m_basePackagePath = basePackagePath;

	//If no trailing slash, add it
	if (!m_basePackagePath.empty() && *m_basePackagePath.rbegin() != '/')
	{
		m_basePackagePath += '/';
	}

	ReadPackageDirectory(s2ws("*"));

	return true;
}

std::string DevelopmentPackageResource::GetFilename(size_t index) const
{
	return ws2s(m_assetsFileInfo[index].resourceName);
}

int DevelopmentPackageResource::GetFileLen(size_t index) const
{
	return m_assetsFileInfo[index].data.nFileSizeLow;
}

bool DevelopmentPackageResource::ReadFile(size_t index, char *pBuffer)
{
	std::wstring fullFileSpec = m_basePackagePath + m_assetsFileInfo[index].data.cFileName;
	std::ifstream file(fullFileSpec, std::ios::binary);
	if (file.is_open())
	{
		file.read(pBuffer, m_assetsFileInfo[index].data.nFileSizeLow);
		file.close();
	}

	return true;
}

size_t DevelopmentPackageResource::Find(const std::string &path) const
{
	std::string lowerCase = path;
	std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int)) std::tolower);

	DevContentsMap::const_iterator i = m_contentsMap.find(lowerCase);
	if (i == m_contentsMap.end())
		return -1;

	return i->second;
}

void DevelopmentPackageResource::ReadPackageDirectory(std::wstring fileSpec)
{
	HANDLE fileHandle;
	WIN32_FIND_DATA findData;

	std::wstring pathSpec = m_basePackagePath + fileSpec;
	fileHandle = FindFirstFile(pathSpec.c_str(), &findData);

	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		while (FindNextFile(fileHandle, &findData))
		{
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			{
				continue;
			}

			std::wstring fileName = findData.cFileName;
			if (findData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				if (fileName != L".." && fileName != L".")
				{
					fileName = fileSpec.substr(0, fileSpec.length() - 1) + fileName + L"\\*";
					ReadPackageDirectory(fileName);
				}
			}
			else
			{
				fileName = fileSpec.substr(0, fileSpec.length() - 1) + fileName;

				std::wstring lower = fileName;
				std::transform(lower.begin(), lower.end(), lower.begin(), (int(*)(int))std::tolower);

				//Replace all slashes to dots
				std::wstring resourceName = lower;
				std::replace(resourceName.begin(), resourceName.end(), '\\', '.');
				std::replace(resourceName.begin(), resourceName.end(), '/', '.');

				//Add the packagename to start of the name
				resourceName = s2ws(m_packageName + ".") + resourceName;

				wcscpy_s(&findData.cFileName[0], MAX_PATH, lower.c_str());
				AssetFileInfo fileInfo = { findData, resourceName };
				m_contentsMap[ws2s(resourceName)] = m_assetsFileInfo.size();
				m_assetsFileInfo.push_back(fileInfo);
			}
		}
	}

	FindClose(fileHandle);
}