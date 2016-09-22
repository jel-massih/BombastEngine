#pragma once

// --------------------------------------------------------------------------
// File:        ZipFile.h
//
// Purpose:     The declaration of a quick'n dirty ZIP file reader class.
//              Original code from Javier Arevalo.
//              Get zlib from http://www.cdrom.com/pub/infozip/zlib/
// --------------------------------------------------------------------------

#include <stdio.h>
#include <map>

struct ZipMappingDetails
{
	size_t index;
	std::string packagePath;

	ZipMappingDetails() {}

	ZipMappingDetails(size_t index, std::string packagePath)
	{
		this->index = index;
		this->packagePath = packagePath;
	}
};

typedef std::map<std::string, size_t> ZipContentsMap;		// maps path to a zip content id

class ZipFile
{
public:
	ZipFile() { m_nEntries = 0; m_pFile = NULL; m_pDirData = NULL; }
	virtual ~ZipFile() { End(); fclose(m_pFile); }

	bool Init(const std::wstring &resFileName);
	void End();

	size_t GetNumFiles()const { return m_nEntries; }
	std::string GetFilename(size_t i) const;
	int GetFileLen(size_t i) const;
	bool ReadFile(size_t i, void *pBuf);

	// Added to show multi-threaded decompression
	bool ReadLargeFile(int i, void *pBuf, void(*progressCallback)(int, bool &));

	size_t Find(const std::string &path) const;

	ZipContentsMap m_ZipContentsMap;

private:
	struct TZipDirHeader;
	struct TZipDirFileHeader;
	struct TZipLocalHeader;

	FILE *m_pFile;		// Zip file
	char *m_pDirData;	// Raw data buffer.
	size_t  m_nEntries;	// Number of entries.

	// Pointers to the dir entries in pDirData.
	const TZipDirFileHeader **m_papDir;
};