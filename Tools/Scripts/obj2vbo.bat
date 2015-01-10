set /p objName=Enter Path to OBJ File:%=%

set /p outputName=Enter Output VBO File Name:%=%

START ../Bin/MeshFormatConverter.exe %objName% %outputName%