#include "ResourceDepot.h"
#include <fstream>
#include <cctype>
#include <string.h>

#include "ZipFile.h"
#include "DevelopmentPackageResource.h"

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
				m_packageMap[ws2s(lowerPackageName)] = PackageMappingDetails(m_numPackages, ws2s(pathSpec));
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

/*==================================================================================================================*/
/*======================================== DevelopmentResourceDepot ================================================*/
/*==================================================================================================================*/

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

			//Only Check for Directories
			if (findData.dwFileAttributes &FILE_ATTRIBUTE_DIRECTORY)
			{
				//Do not warn for directory symbol
				if (findData.cFileName == s2ws(".."))
				{
					continue;
				}

				std::wstring directoryName = findData.cFileName;
				std::wstring lowerPackageName = directoryName;

				std::transform(lowerPackageName.begin(), lowerPackageName.end(), lowerPackageName.begin(), (int(*)(int))std::tolower);
				lowerPackageName = lowerPackageName.substr(0, lowerPackageName.find_last_of('.'));

				pathSpec = pathSpec.substr(0, pathSpec.find_last_of('/'));
				pathSpec = pathSpec + s2ws("/") + directoryName;

				//Add Package to mapping
				m_packageMap[ws2s(lowerPackageName)] = PackageMappingDetails(m_numPackages, ws2s(pathSpec));
				m_numPackages++;
			}
		}
	}

	FindClose(fileHandle);

	return true;
}

bool DevelopmentResourceDepot::InitializePackages()
{
	m_packages = BE_NEW DevelopmentPackageResource[m_numPackages];

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

DevelopmentPackageResource* const DevelopmentResourceDepot::GetDevelopmentPackageFileForResource(const Resource& r)
{
	std::string packageName = r.m_name.substr(0, r.m_name.find('.'));

	auto packageIndexIt = m_packageMap.find(packageName);
	if (packageIndexIt == m_packageMap.end()) {
		return nullptr;
	}

	return &m_packages[(*packageIndexIt).second.index];
}

int DevelopmentResourceDepot::VGetRawResourceSize(const Resource &r)
{
	DevelopmentPackageResource* const developmentResource = GetDevelopmentPackageFileForResource(r);
	if (developmentResource == nullptr)
	{
		return -1;
	}

	size_t resourceNum = developmentResource->Find(r.m_name.c_str());
	if (resourceNum == -1)
	{
		return -1;
	}

	return developmentResource->GetFileLen(resourceNum);
}

size_t DevelopmentResourceDepot::VGetRawResource(const Resource &r, char* buffer)
{
	DevelopmentPackageResource* const packageResource = GetDevelopmentPackageFileForResource(r);
	if (packageResource == nullptr)
	{
		return -1;
	}

	size_t size = 0;
	size_t resourceNum = packageResource->Find(r.m_name.c_str());
	if (resourceNum != -1)
	{
		size = packageResource->GetFileLen(resourceNum);
		packageResource->ReadFile(resourceNum, buffer);
	}

	return size;
}

size_t DevelopmentResourceDepot::VGetNumPackages() const
{
	return m_numPackages;
}

size_t DevelopmentResourceDepot::VGetNumResources(size_t packageIndex) const
{
	return (packageIndex < m_numPackages) ? 0 : m_packages[packageIndex].GetNumFiles();
}

std::string	DevelopmentResourceDepot::VGetResourceName(size_t packageIndex, size_t resourceIndex) const
{
	std::string resName = "";
	if (packageIndex < m_numPackages && resourceIndex >= 0 && resourceIndex < m_packages[resourceIndex].GetNumFiles())
	{
		resName = m_packages[packageIndex].GetFilename(resourceIndex);
	}

	return resName;
}

void DevelopmentResourceDepot::VAddPackageDirectory(std::wstring directoryName)
{
	m_packagePaths.push_back(directoryName);
}