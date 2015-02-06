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

ResourceZipFile::~ResourceZipFile()
{
	SAFE_DELETE(m_pZipFile);
}

bool ResourceZipFile::VOpen()
{
	m_pZipFile = BE_NEW ZipFile;
	if (m_pZipFile)
	{
		return m_pZipFile->Init(m_resFileName.c_str());
	}
	return false;
}

int ResourceZipFile::VGetRawResourceSize(const Resource &r)
{
	size_t resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum == -1)
	{
		return -1;
	}

	return m_pZipFile->GetFileLen(resourceNum);
}

size_t ResourceZipFile::VGetRawResource(const Resource& r, char* buffer)
{
	size_t size = 0;
	size_t resourceNum = m_pZipFile->Find(r.m_name.c_str());
	if (resourceNum != -1)
	{
		size = m_pZipFile->GetFileLen(resourceNum);
		m_pZipFile->ReadFile(resourceNum, buffer);
	}

	return size;
}

size_t ResourceZipFile::VGetNumResources() const
{
	return (m_pZipFile == NULL) ? 0 : m_pZipFile->GetNumFiles();
}

std::string ResourceZipFile::VGetResourceName(size_t num) const
{
	std::string resName = "";
	if (m_pZipFile != NULL && num >= 0 && num < m_pZipFile->GetNumFiles())
	{
		resName = m_pZipFile->GetFilename(num);
	}

	return resName;
}

DevelopmentResourceZipFile::DevelopmentResourceZipFile(const std::wstring resFilename)
	: ResourceZipFile(resFilename)
{
	TCHAR dir[MAX_PATH];
	GetCurrentDirectory(MAX_PATH, dir);

	m_assetsDir = dir;
	m_assetsDir += L"\\..\\Assets\\";
}

size_t DevelopmentResourceZipFile::Find(const std::string &name)
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

bool DevelopmentResourceZipFile::VOpen()
{
	//Open assets and read non-hidden stuff
	ReadAssetsDirectory(L"*");

	return true;
}

int DevelopmentResourceZipFile::VGetRawResourceSize(const Resource &r)
{
	size_t num = Find(r.m_name.c_str());
	if (num == -1)
	{
		return -1;
	}

	return m_assetsFileInfo[num].nFileSizeLow;
}

size_t DevelopmentResourceZipFile::VGetRawResource(const Resource &r, char* buffer)
{
	size_t num = Find(r.m_name.c_str());
	if (num == -1)
	{
		return -1;
	}

	int bytes = -1;
	
	std::string fullFileSpec = ws2s(m_assetsDir) + r.m_name.c_str();
	std::ifstream file(fullFileSpec, std::ios::binary);
	if (file.is_open())
	{
		file.read(buffer, m_assetsFileInfo[num].nFileSizeLow);
		bytes = (int)file.gcount();
		file.close();
	}
	return bytes;
}

size_t DevelopmentResourceZipFile::VGetNumResources() const
{
	return m_assetsFileInfo.size();
}

std::string	DevelopmentResourceZipFile::VGetResourceName(size_t num) const
{
	std::wstring wideName = m_assetsFileInfo[num].cFileName;
	return ws2s(wideName);
}

void DevelopmentResourceZipFile::ReadAssetsDirectory(std::wstring fileSpec)
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
				wcscpy_s(&findData.cFileName[0], MAX_PATH, lower.c_str());
				m_directoryContentsMap[ws2s(lower)] = m_assetsFileInfo.size();
				m_assetsFileInfo.push_back(findData);
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

ResourceCache::ResourceCache(const unsigned int sizeInMb, IResourceFile* resFile)
{
	m_cacheSize = sizeInMb * 1024 * 1024; //Total Memory Size (need to multiply to get bits) Mb -> kB -> b
	m_allocated = 0;
	m_pFile = resFile;
}

ResourceCache::~ResourceCache()
{
	while (!m_lru.empty())
	{
		FreeOneResource();
	}
	SAFE_DELETE(m_pFile);

	while (!m_resourceLoaders.empty())
	{
		IResourceLoader* resource = *(m_resourceLoaders.begin());
		SAFE_DELETE(resource);
		m_resourceLoaders.pop_front();
	}
}

bool ResourceCache::Initialize()
{
	if (m_pFile->VOpen())
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

	int rawSize = m_pFile->VGetRawResourceSize(*r);
	if (rawSize < 0)
	{
		BE_ERROR("Resource returned: %d - Resource not found or empty: %s", rawSize, r->m_name);
		return NULL;
	}

	int allocSize = rawSize + ((loader->VAddNullZero()) ? 1 : 0);
	char* rawBuffer = loader->VUseRawFile() ? Allocate(allocSize) : BE_NEW char[allocSize];
	memset(rawBuffer, 0, allocSize);

	if (rawBuffer == NULL || m_pFile->VGetRawResource(*r, rawBuffer) == 0)
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
	if (m_pFile == NULL)
	{
		return 0;
	}

	size_t numFiles = m_pFile->VGetNumResources();
	int loaded = 0;
	bool cancel = false;

	for (size_t i = 0; i < numFiles; i++)
	{
		Resource resource(m_pFile->VGetResourceName(i));

		if (WildcardMatch(pattern.c_str(), resource.m_name.c_str()))
		{
			ResourceHandle* handle = g_pApp->m_pResourceCache->GetHandle(&resource);
			loaded++;
		}

		if (progressCallback != NULL)
		{
			progressCallback(i * 100 / numFiles, cancel);
		}
	}

	return loaded;
}

//Search Resources Cache assets for files matching given pattern
std::vector<std::string> ResourceCache::Match(const std::string pattern)
{
	std::vector<std::string> matchingNames;
	if (m_pFile == NULL)
		return matchingNames;

	size_t numFiles = m_pFile->VGetNumResources();
	for (size_t i = 0; i<numFiles; ++i)
	{
		std::string name = m_pFile->VGetResourceName(i);
		std::transform(name.begin(), name.end(), name.begin(), (int(*)(int)) std::tolower);
		if (WildcardMatch(pattern.c_str(), name.c_str()))
		{
			matchingNames.push_back(name);
		}
	}
	return matchingNames;
}