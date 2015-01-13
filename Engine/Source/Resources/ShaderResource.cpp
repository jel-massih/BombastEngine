#include "ShaderResource.h"

IResourceLoader* CreateShaderResourceLoader()
{
	return BE_NEW ShaderResourceLoader();
}