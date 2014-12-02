#include "../BombastEngineStd.h"
#include "../Bombast/interfaces.h"

class SceneNode;

class PongSampleController : public IMouseHandler, public IKeyboardHandler
{
public:
	PongSampleController(SceneNode* object);
	void OnUpdate(DWORD const elapsedMs);

protected:
	BYTE m_bKey[256];
	SceneNode* m_pObject;

public:
	virtual bool VOnMouseMove(const Point& mousePos, const int radius) { return true; }
	virtual bool VOnMouseDown(const Point& mousePos, const int radius, const std::string& buttonName);
	virtual bool VOnMouseUp(const Point& mousePos, const int radius, const std::string& buttonName) { return (buttonName == "MouseLeft"); }

	bool VOnKeyDown(const BYTE c);
	bool VOnKeyUp(const BYTE c);
};