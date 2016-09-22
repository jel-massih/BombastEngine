#pragma once

#include "ResourceCache.h"

class ZipFile;
class DevelopmentPackageResource;

struct PackageMappingDetails
{
	size_t index;
	std::string packagePath;

	PackageMappingDetails() {}

	PackageMappingDetails(size_t index, std::string packagePath)
	{
		this->index = index;
		this->packagePath = packagePath;
	}
};

typedef std::map<std::string, PackageMappingDetails> PackageMap;// maps package name to a zip details

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
	DevelopmentResourceDepot() :m_numPackages(0), m_packages(nullptr) {}
	virtual ~DevelopmentResourceDepot();

	virtual bool VOpen();
	virtual int VGetRawResourceSize(const Resource &r);
	virtual size_t VGetRawResource(const Resource &r, char *buffer);
	virtual size_t VGetNumPackages() const override;
	virtual size_t VGetNumResources(size_t packageIndex = 0) const override;
	virtual std::string VGetResourceName(size_t packageIndex, size_t resourceIndex) const;
	virtual void VAddPackageDirectory(std::wstring directoryName) override;

private:
	bool RegisterPackages(std::wstring basePackagePath);
	bool InitializePackages();
	DevelopmentPackageResource* const GetDevelopmentPackageFileForResource(const Resource& r);

private:
	PackageMap m_packageMap;
	std::vector<std::wstring> m_packagePaths;

	unsigned short m_numPackages;
	DevelopmentPackageResource* m_packages;
};