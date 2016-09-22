#pragma once

#include "ResourceCache.h"

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
	bool RegisterPackages(std::wstring basePackagePath);
	bool InitializePackages();
	ZipFile* const GetZipFileForResource(const Resource& r);

private:
	PackageMap m_packageMap;
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

	struct DevelopmentResourcePackage
	{
		std::string packageName;
		std::vector<AssetFileInfo> packageAssets;
	};

public:
	std::vector<AssetFileInfo> m_assetsFileInfo;
	ZipContentsMap m_directoryContentsMap;

	DevelopmentResourceDepot() :m_numPackages(0), m_packages(nullptr) {}
	virtual ~DevelopmentResourceDepot();

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

private:
	bool RegisterPackages(std::wstring basePackagePath);
	bool InitializePackages();

private:
	PackageMap m_packageMap;
	std::vector<std::wstring> m_packagePaths;

	unsigned short m_numPackages;
	ZipFile* m_packages;
};