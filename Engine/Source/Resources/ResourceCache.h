#pragma once

#include "../Bombast/interfaces.h"
#include "ZipFile.h"
#include <list>
#include <map>
#include <vector>

class ResourceCache;
class ResourceHandle;

class IResourceExtraData
{
public:
	virtual void Shutdown(){};
	virtual std::string VToString()=0;
};

class Resource
{
public:
	std::string m_name;
	Resource(const std::string &name);
};

class ZipResourceDepot : public IResourceDepot
{
public:
	ZipResourceDepot() :m_numPackages(0), m_packages(nullptr) {}
	virtual ~ZipResourceDepot();

	virtual bool VOpen() override;
	virtual int VGetRawResourceSize(const Resource &r) override;
	virtual size_t VGetRawResource(const Resource &r, char *buffer) override;
	virtual size_t VGetNumPackages() const override;
	virtual size_t VGetNumResources(size_t packageIndex = 0) const override;
	virtual std::string VGetResourceName(size_t packageIndex, size_t resourceIndex) const override;
	virtual void VAddPackageDirectory(std::wstring directoryName) override;

private:
	bool RegisterPackages();
	ZipFile* const GetZipFileForResource(const Resource& r);

private:
	ZipContentsMap m_packageMap;
	std::vector<std::wstring> m_packagePaths;
	
	unsigned short m_numPackages;
	ZipFile* m_packages;
};

//No packaging, just in regular folders
class DevelopmentResourceDepot : public IResourceDepot
{
public:
	struct AssetFileInfo
	{
		WIN32_FIND_DATA data;
		std::wstring resourceName;
	};

public:
	std::wstring m_assetsDir;
	std::vector<AssetFileInfo> m_assetsFileInfo;
	ZipContentsMap m_directoryContentsMap;

	DevelopmentResourceDepot(const std::wstring resFileName);

	virtual bool VOpen();
	virtual int VGetRawResourceSize(const Resource &r);
	virtual size_t VGetRawResource(const Resource &r, char *buffer);
	virtual size_t VGetNumPackages() const override;
	virtual size_t VGetNumResources(size_t packageIndex = 0) const override;
	virtual std::string VGetResourceName(size_t packageIndex, size_t resourceIndex) const;
	virtual bool VIsUsingDevelopmentDirectories(void) const { return true; }

	size_t Find(const std::string &path);

protected:
	void ReadAssetsDirectory(std::wstring fileSpec);
};

class ResourceHandle
{
	friend class ResourceCache;

public:
	ResourceHandle(Resource &handle, char* buffer, unsigned int size, ResourceCache* pResourceCache);

	virtual ~ResourceHandle();

	const std::string  GetName() const { return m_resource.m_name; }
	unsigned int Size() const { return m_size; }
	char* Buffer() const { return m_buffer; }
	char* WritableBuffer() { return m_buffer; }

	IResourceExtraData* GetExtra() { return m_pExtra; }
	void SetExtra(IResourceExtraData* extra) { m_pExtra = extra; }

protected:
	Resource m_resource;
	char* m_buffer;
	unsigned int m_size;
	ResourceCache* m_pResourceCache;
	IResourceExtraData* m_pExtra;
};

class DefaultResourceLoader : public IResourceLoader
{
public:
	virtual bool VUseRawFile() { return true; }
	virtual bool VDiscardRawBufferAfterLoad() { return true; }
	virtual unsigned int VGetLoadedResourceSize(char* rawBuffer, unsigned int rawSize) { return rawSize; }
	virtual bool VLoadResource(char* rawBuffer, unsigned int rawSize, ResourceHandle* handle) { return true; }
	virtual std::vector<std::string> VGetPatterns() { return std::vector<std::string>({ "*" }); }
};

typedef std::list<ResourceHandle*> ResourceHandleList;
typedef std::map<std::string, ResourceHandle*> ResourceHandleMap;
typedef std::list<IResourceLoader* > ResourceLoaderList;

class ResourceCache
{
	friend class ResourceHandle;

	ResourceHandleList m_lru;
	ResourceHandleMap m_resources;
	ResourceLoaderList m_resourceLoaders;

	IResourceDepot* m_pDepot;

	unsigned int m_cacheSize;
	unsigned int m_allocated;

public:
	ResourceCache(const unsigned int sizeInMb, IResourceDepot* depot);
	virtual ~ResourceCache();

	bool Initialize();

	void RegisterLoader(IResourceLoader* Loader);

	ResourceHandle* GetHandle(Resource* r);

	int Preload(const std::string pattern, void(*progressCallback)(size_t, bool &));

	void Flush();

	std::vector<std::string> Match(const std::string pattern);

protected:
	bool MakeRoom(unsigned int size);
	char* Allocate(unsigned int size);
	void Free(ResourceHandle* target);

	ResourceHandle* Load(Resource* r);
	ResourceHandle* Find(Resource* r);
	void Update(ResourceHandle* handle);

	void FreeOneResource();
	void MemoryHasBeenFreed(unsigned int size);
};