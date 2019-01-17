#ifndef __ACTORCONTROLLER_H__
#define __ACTORCONTROLLER_H__

#include "Engine/Interfaces.h"
#include "Engine/SharedDefines.h"

class SceneNode;
class ActorController : public IKeyboardHandler, public IPointerHandler, public IJoystickHandler
{
public:
    ActorController(shared_ptr<SceneNode> controlledObject, float speed = 0.36f);

    void SetControlledObject(shared_ptr<SceneNode> controlledObject) { m_pControlledObject = controlledObject; }
    void OnUpdate(uint32 msDiff);
    void HandleAction(ActionType actionType);

    // Interface
    bool VOnKeyDown(SDL_Keycode key);
    bool VOnKeyUp(SDL_Keycode key);

    bool VOnPointerMove(SDL_MouseMotionEvent& mouseEvent);
    bool VOnPointerButtonDown(SDL_MouseButtonEvent& mouseEvent);
    bool VOnPointerButtonUp(SDL_MouseButtonEvent& mouseEvent);

    bool VOnJoystickButtonDown(Uint8 button);
    bool VOnJoystickButtonUp(Uint8 button);
    bool VOnJoystickAxisMotion(Uint8 axis, Sint16 value);

private:
    shared_ptr<SceneNode> m_pControlledObject;
    float m_Speed;

    std::map<int, bool> m_InputKeys;

    std::map<int, bool> m_ControllerKeys;
    std::map<int, int> m_ControllerAxis;

    // SDL_Scancode array
    const uint8* m_pKeyStates;

    bool m_MouseLeftButtonDown;
    bool m_MouseRightButtonDown;
};

#endif //__ACTORCONTROLLER_H__