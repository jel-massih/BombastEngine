#include "ResourceDepot.h"
#include <fstream>
#include <cctype>
#include <string.h>

ZipResourceDepot::~ZipResourceDepot()
{
	SAFE_DELETE_ARRAY(m_packages);
}

bool ZipResourceDepot::VOpen()
{
	bool success = true;

	for (auto it = m_packagePaths.begin(); it != m_packagePaths.end(); it++)
	{
		//Iterates over packages directory and registers each package
		success = success && RegisterPackages(*it);
	}

	if (success)
	{
		success = InitializePackages();
	}

	return success;
}

bool ZipResourceDepot::RegisterPackages(std::wstring basePackagePath)
{
	HANDLE fileHandle;
	WIN32_FIND_DATA findData;

	//Get first file
	std::wstring pathSpec = basePackagePath + L"*";
	fileHandle = FindFirstFile(pathSpec.c_str(), &findData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		//Loop on all remaining entries
		while (FindNextFile(fileHandle, &findData))
		{
			//Skip if file is hidden
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			{
				continue;
			}

			//Do not currently support packages within directories, so throw Warning
			if (findData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				//Do not warn for directory symbol
				if (findData.cFileName != s2ws(".."))
				{
					BE_WARNING("Packages within Directories are ignored");
				}
			}
			else
			{
				std::wstring filename = findData.cFileName;
				std::wstring lowerPackageName = filename;

				std::transform(lowerPackageName.begin(), lowerPackageName.end(), lowerPackageName.begin(), (int(*)(int))std::tolower);
				lowerPackageName = lowerPackageName.substr(0, lowerPackageName.find_last_of('.'));

				pathSpec = pathSpec.substr(0, pathSpec.find_last_of('/'));
				pathSpec = pathSpec + s2ws("/") + filename;

				//Add Package to mapping
				m_packageMap[ws2s(lowerPackageName)] = ZipMappingDetails(m_numPackages, ws2s(pathSpec));
				m_numPackages++;
			}
		}
	}

	FindClose(fileHandle);


	return true;
}

bool ZipResourceDepot::InitializePackages()
{
	m_packages = BE_NEW ZipFile[m_numPackages];

	//init each package
	auto it = m_packageMap.begin();
	while (it != m_packageMap.end())
	{
		bool packageInitializeSuccess = m_packages[(*it).second.index].Init(s2ws((*it).second.packagePath));
		if (!packageInitializeSuccess)
		{
			return false;
		}

		it++;
	}

	return true;
}

ZipFile* const ZipResourceDepot::GetZipFileForResource(const Resource& r)
{
	std::string packageName = r.m_name.substr(0, r.m_name.find('.'));

	auto packageIndexIt = m_packageMap.find(packageName);
	if (packageIndexIt == m_packageMap.end()) {
		return nullptr;
	}

	return &m_packages[(*packageIndexIt).second.index];
}

int ZipResourceDepot::VGetRawResourceSize(const Resource &r)
{
	ZipFile* const zipResource = GetZipFileForResource(r);
	if (zipResource == nullptr)
	{
		return -1;
	}

	size_t resourceNum = zipResource->Find(r.m_name.c_str());
	if (resourceNum == -1)
	{
		return -1;
	}

	return zipResource->GetFileLen(resourceNum);
}

size_t ZipResourceDepot::VGetRawResource(const Resource& r, char* buffer)
{
	ZipFile* const zipResource = GetZipFileForResource(r);
	if (zipResource == nullptr)
	{
		return -1;
	}

	size_t size = 0;
	size_t resourceNum = zipResource->Find(r.m_name.c_str());
	if (resourceNum != -1)
	{
		size = zipResource->GetFileLen(resourceNum);
		zipResource->ReadFile(resourceNum, buffer);
	}

	return size;
}

size_t ZipResourceDepot::VGetNumPackages() const
{
	return m_numPackages;
}

size_t ZipResourceDepot::VGetNumResources(size_t packageIndex) const
{
	return (packageIndex < m_numPackages) ? 0 : m_packages[packageIndex].GetNumFiles();
}

std::string ZipResourceDepot::VGetResourceName(size_t packageIndex, size_t resourceIndex) const
{
	std::string resName = "";
	if (packageIndex < m_numPackages && resourceIndex >= 0 && resourceIndex < m_packages[resourceIndex].GetNumFiles())
	{
		resName = m_packages[packageIndex].GetFilename(resourceIndex);
	}

	return resName;
}

void ZipResourceDepot::VAddPackageDirectory(std::wstring directoryName)
{
	m_packagePaths.push_back(directoryName);
}

DevelopmentResourceDepot::~DevelopmentResourceDepot()
{
	SAFE_DELETE_ARRAY(m_packages);
}

bool DevelopmentResourceDepot::VOpen()
{
	//Open assets and read non-hidden stuff
	bool success = true;

	for (auto it = m_packagePaths.begin(); it != m_packagePaths.end(); it++)
	{
		success = success && RegisterPackages(*it);
	}

	if (success)
	{
		success = InitializePackages();
	}

	return success;
}

bool DevelopmentResourceDepot::RegisterPackages(std::wstring basePackagePath)
{
	HANDLE fileHandle;
	WIN32_FIND_DATA findData;

	//Get first file
	std::wstring pathSpec = basePackagePath + L"*";
	fileHandle = FindFirstFile(pathSpec.c_str(), &findData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		//Loop on all remaining entries
		while (FindNextFile(fileHandle, &findData))
		{
			//Skip if file is hidden
			if (findData.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
			{
				continue;
			}

			//Do not currently support packages within directories, so throw Warning
			if (findData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				//Do not warn for directory symbol
				if (findData.cFileName != s2ws(".."))
				{
					BE_WARNING("Packages within Directories are ignored");
				}
			}
			else
			{
				std::wstring filename = findData.cFileName;
				std::wstring lowerPackageName = filename;

				std::transform(lowerPackageName.begin(), lowerPackageName.end(), lowerPackageName.begin(), (int(*)(int))std::tolower);
				lowerPackageName = lowerPackageName.substr(0, lowerPackageName.find_last_of('.'));

				pathSpec = pathSpec.substr(0, pathSpec.find_last_of('/'));
				pathSpec = pathSpec + s2ws("/") + filename;

				//Add Package to mapping
				m_packageMap[ws2s(lowerPackageName)] = ZipMappingDetails(m_numPackages, ws2s(pathSpec));
				m_numPackages++;
			}
		}
	}

	FindClose(fileHandle);

	return true;
}

bool DevelopmentResourceDepot::InitializePackages()
{
	m_packages = BE_NEW ZipFile[m_numPackages];

	//init each package
	auto it = m_packageMap.begin();
	while (it != m_packageMap.end())
	{
		bool packageInitializeSuccess = m_packages[(*it).second.index].Init(s2ws((*it).second.packagePath));
		if (!packageInitializeSuccess)
		{
			return false;
		}

		it++;
	}

	return true;
}

int DevelopmentResourceDepot::VGetRawResourceSize(const Resource &r)
{
	size_t num = Find(r.m_name.c_str());
	if (num == -1)
	{
		return -1;
	}

	return m_assetsFileInfo[num].data.nFileSizeLow;
}

size_t DevelopmentResourceDepot::VGetRawResource(const Resource &r, char* buffer)
{
	size_t num = Find(r.m_name.c_str());
	if (num == -1)
	{
		return -1;
	}

	int bytes = -1;
	std::wstring fullFileSpec = m_assetsDir + m_assetsFileInfo[num].data.cFileName;
	std::ifstream file(fullFileSpec, std::ios::binary);
	if (file.is_open())
	{
		file.read(buffer, m_assetsFileInfo[num].data.nFileSizeLow);
		bytes = (int)file.gcount();
		file.close();
	}
	return bytes;
}

size_t DevelopmentResourceDepot::VGetNumPackages() const
{
	return 1;
}

size_t DevelopmentResourceDepot::VGetNumResources(size_t packageIndex) const
{
	return m_assetsFileInfo.size();
}

std::string	DevelopmentResourceDepot::VGetResourceName(size_t packageIndex, size_t resourceIndex) const
{
	std::wstring wideName = m_assetsFileInfo[resourceIndex].data.cFileName;
	return ws2s(wideName);
}

void DevelopmentResourceDepot::ReadAssetsDirectory(std::wstring fileSpec)
{
	HANDLE fileHandle;
	WIN32_FIND_DATA findData;

	//Get first file
	std::wstring pathSpec = m_assetsDir + fileSpec;
	fileHandle = FindFirstFile(pathSpec.c_str(), &findData);
	if (fileHandle != INVALID_HANDLE_VALUE)
	{
		//Loop on all remaining entries
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
					ReadAssetsDirectory(fileName);
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

				wcscpy_s(&findData.cFileName[0], MAX_PATH, lower.c_str());
				AssetFileInfo fileInfo = { findData, resourceName };
				m_directoryContentsMap[ws2s(resourceName)] = m_assetsFileInfo.size();
				m_assetsFileInfo.push_back(fileInfo);
			}
		}
	}

	FindClose(fileHandle);
}

size_t DevelopmentResourceDepot::Find(const std::string &name)
{
	std::string lowerCase = name;
	std::transform(lowerCase.begin(), lowerCase.end(), lowerCase.begin(), (int(*)(int))std::tolower);
	ZipContentsMap::const_iterator i = m_directoryContentsMap.find(lowerCase);
	if (i == m_directoryContentsMap.end())
	{
		return -1;
	}

	return i->second;
}
