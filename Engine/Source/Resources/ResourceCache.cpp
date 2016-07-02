#include "ResourceCache.h"
#include "../Bombast/BombastApp.h"
#include <algorithm>
#include <cctype>
#include <string.h>
#include <fstream>

Resource::Resource(const std::string& name)
{
	m_name = name;
	std::transform(m_name.begin(), m_name.end(), m_name.begin(), (int(*)(int)) std::tolower);
}

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
				BE_WARNING("Packages within Directories are ignored");
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

DevelopmentResourceDepot::DevelopmentResourceDepot(const std::wstring resFilename)
{
	m_assetsDir += s2ws(ROOT_ENGINE_PATH) + L"Assets/";
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

bool DevelopmentResourceDepot::VOpen()
{
	//Open assets and read non-hidden stuff
	ReadAssetsDirectory(L"*");

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

ResourceHandle::ResourceHandle(Resource& resource, char* buffer, unsigned int size, ResourceCache* pResCache) : m_resource(resource)
{
	m_buffer = buffer;
	m_size = size;
	m_pResourceCache = pResCache;
	m_pExtra = NULL;
}

ResourceHandle::~ResourceHandle()
{
	if (m_pExtra)
	{
		m_pExtra->Shutdown();
		SAFE_DELETE(m_pExtra);
	}
	SAFE_DELETE_ARRAY(m_buffer);
	m_pResourceCache->MemoryHasBeenFreed(m_size);
}

ResourceCache::ResourceCache(const unsigned int sizeInMb, IResourceDepot* resFile)
{
	m_cacheSize = sizeInMb * 1024 * 1024; //Total Memory Size (need to multiply to get bits) Mb -> kB -> b
	m_allocated = 0;
	m_pDepot = resFile;
}

ResourceCache::~ResourceCache()
{
	while (!m_lru.empty())
	{
		FreeOneResource();
	}
	SAFE_DELETE(m_pDepot);

	while (!m_resourceLoaders.empty())
	{
		IResourceLoader* resource = *(m_resourceLoaders.begin());
		SAFE_DELETE(resource);
		m_resourceLoaders.pop_front();
	}
}

bool ResourceCache::Initialize()
{
	if (m_pDepot->VOpen())
	{
		RegisterLoader((IResourceLoader*)(BE_NEW DefaultResourceLoader()));
		return true;
	}

	return false;
}

//Add Loader to resource Cache
void ResourceCache::RegisterLoader(IResourceLoader* loader)
{
	m_resourceLoaders.push_front(loader);
}

ResourceHandle* ResourceCache::GetHandle(Resource* r)
{
	ResourceHandle* handle = Find(r);

	if (handle == NULL)
	{
		handle = Load(r);
	}
	else
	{
		Update(handle);
	}

	return handle;
}

ResourceHandle* ResourceCache::Load(Resource* r)
{
	//Create new resource and add to lru list and map
	IResourceLoader* loader = NULL;
	ResourceHandle* handle;

	for (ResourceLoaderList::iterator it = m_resourceLoaders.begin(); it != m_resourceLoaders.end(); it++)
	{
		IResourceLoader* testLoader = *it;

		std::vector<std::string>& testPatterns = testLoader->VGetPatterns();
		for (auto it2 = testPatterns.begin(); it2 != testPatterns.end(); it2++)
		{
			if (WildcardMatch(it2->c_str(), r->m_name.c_str()))
			{
				loader = testLoader;
				break;
			}
		}

		if (loader != NULL)
		{
			break;
		}
	}

	if (!loader)
	{
		BE_ERROR("Default Resource Loader not found");
		return NULL;
	}

	int rawSize = m_pDepot->VGetRawResourceSize(*r);
	if (rawSize < 0)
	{
		BE_ERROR("Resource returned: %d - Resource not found or empty: %s", rawSize, r->m_name.c_str());
		return NULL;
	}

	int allocSize = rawSize + ((loader->VAddNullZero()) ? 1 : 0);
	char* rawBuffer = loader->VUseRawFile() ? Allocate(allocSize) : BE_NEW char[allocSize];
	memset(rawBuffer, 0, allocSize);

	if (rawBuffer == NULL || m_pDepot->VGetRawResource(*r, rawBuffer) == 0)
	{
		//Resource Cache Out of memory
		BE_ERROR("Error Loading Resource: %s Resource Cache out of memory", r->m_name);
		return NULL;
	}

	//Add null 0 if Loader  requires it
	if (loader->VAddNullZero()) {
		rawBuffer[allocSize - 1] = '\0';
	}

	char* buffer = NULL;
	unsigned int size = 0;

	if (loader->VUseRawFile())
	{
		buffer = rawBuffer;
		handle = BE_NEW ResourceHandle(*r, buffer, rawSize, this);
	}
	else
	{
		size = loader->VGetLoadedResourceSize(rawBuffer, rawSize);
		buffer = Allocate(size);
		if (rawBuffer == NULL || buffer == NULL)
		{
			//Resource Cache Out of memory
			BE_ERROR("Error Loading Resource: %s Resource Cache out of memory", r->m_name);
			return NULL;
		}

		handle = BE_NEW ResourceHandle(*r, buffer, size, this);
		bool success = loader->VLoadResource(rawBuffer, rawSize, handle);

		if (loader->VDiscardRawBufferAfterLoad())
		{
			SAFE_DELETE_ARRAY(rawBuffer);
		}

		if (!success)
		{
			//Failed To load Resource
			BE_ERROR("Error Loading Resource: %s Failed to load resource", r->m_name);
			SAFE_DELETE(handle);
			return NULL;
		}
	}

	if (handle)
	{
		m_lru.push_front(handle);
		m_resources[r->m_name] = handle;
	}

	BE_ASSERT(loader && _T("Default Resource loader not found"));
	return handle;
}

ResourceHandle* ResourceCache::Find(Resource* r)
{
	ResourceHandleMap::iterator i = m_resources.find(r->m_name);
	if (i == m_resources.end())
	{
		return NULL;
	}

	return i->second;
}

void ResourceCache::Update(ResourceHandle* handle)
{
	m_lru.remove(handle);
	m_lru.push_front(handle);
}

char* ResourceCache::Allocate(unsigned int size)
{
	if (!MakeRoom(size))
	{
		return NULL;
	}

	char* mem = BE_NEW char[size];
	if (mem)
	{
		m_allocated += size;
	}

	return mem;
}

void ResourceCache::FreeOneResource()
{
	ResourceHandleList::iterator target = m_lru.end();
	target--;

	ResourceHandle* handle = *target;

	m_lru.pop_back();
	m_resources.erase(handle->m_resource.m_name);
	SAFE_DELETE(handle);
}

//Frees every handle in cache.
void ResourceCache::Flush()
{
	while (!m_lru.empty())
	{
		ResourceHandle* handle = *(m_lru.begin());
		Free(handle);
		m_lru.pop_front();
	}
}

bool ResourceCache::MakeRoom(unsigned int size)
{
	if (size > m_cacheSize)
	{
		return false;
	}

	//return false if no way to allocate memory
	while (size > (m_cacheSize - m_allocated))
	{
		// Nothing to remove from cache and still not enough room
		if (m_lru.empty())
		{
			return false;
		}

		FreeOneResource();
	}

	return true;
}

void ResourceCache::Free(ResourceHandle* target)
{
	m_lru.remove(target);
	m_resources.erase(target->m_resource.m_name);

	SAFE_DELETE(target);
}

//Called when Resource is destroyed to indicate freed space
void ResourceCache::MemoryHasBeenFreed(unsigned int size)
{
	m_allocated -= size;
}

int ResourceCache::Preload(const std::string pattern, void(*progressCallback)(size_t, bool&))
{
	if (m_pDepot == NULL)
	{
		return 0;
	}

	size_t numPackages = m_pDepot->VGetNumPackages();
	int loaded = 0;
	bool cancel = false;

	for (size_t i = 0; i < numPackages; i++)
	{
		size_t numResources = m_pDepot->VGetNumResources(i);
		for (size_t j = 0; j < numResources; j++)
		{
			Resource resource(m_pDepot->VGetResourceName(i, j));

			if (WildcardMatch(pattern.c_str(), resource.m_name.c_str()))
			{
				ResourceHandle* handle = g_pApp->m_pResourceCache->GetHandle(&resource);
				loaded++;
			}

			if (progressCallback != NULL)
			{
				progressCallback(j * 100 / numResources, cancel);
			}
		}
	}

	return loaded;
}

//Search Resources Cache assets for files matching given pattern
std::vector<std::string> ResourceCache::Match(const std::string pattern)
{
	std::vector<std::string> matchingNames;
	if (m_pDepot == NULL)
		return matchingNames;

	size_t numPackages = m_pDepot->VGetNumPackages();

	for (int i = 0; i < numPackages; i++)
	{
		size_t numFiles = m_pDepot->VGetNumResources(i);
		for (size_t j = 0; j < numFiles; j++)
		{
			std::string name = m_pDepot->VGetResourceName(i, j);
			std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
			if (WildcardMatch(pattern.c_str(), name.c_str()))
			{
				matchingNames.push_back(name);
			}
		}
	}
	return matchingNames;
}