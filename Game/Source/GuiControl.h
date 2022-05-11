#ifndef __GUICONTROL_H__
#define __GUICONTROL_H__

#include "Input.h"
#include "Audio.h"
#include "Render.h"
#include "Textures.h"
#include "Scene.h"

#include "Point.h"
#include "Rect.h"
#include "Collider.h"

class Input;
class Audio;
class Render;
class Textures;
class Scene;
class GuiString;
class GuiManager;


enum class GuiControlType
{
    BUTTON,
    TOGGLE,
    CHECKBOX,
    SLIDER,
    SLIDERBAR,
    COMBOBOX,
    DROPDOWNBOX,
    INPUTBOX,
    VALUEBOX,
    SPINNER, 
    TEXT,
    IMAGE
};

enum class GuiControlState
{
    DISABLED,
    NORMAL,
    FOCUSED,
    PRESSED,
    SELECTED
};

class GuiControl
{
public:

    GuiControl(Rect bounds, GuiControlType type, SDL_Texture* texture, Point scale, suint id, bool anchored, Input* input, Render* render, GuiManager* gui, Audio* audio, Scene* scene, Textures* tex) : 
        bounds(bounds), type(type), 
        state(GuiControlState::NORMAL), 
        texture(texture), 
        scale(scale), 
        id(id),
        anchored(anchored), 
        input(input), 
        render(render), 
        gui(gui), 
        audio(audio),
        tex(tex),
        observer(scene) {}

    GuiControl(Rect bounds, GuiControlType type, SDL_Texture* texture, suint id, Point scale, bool anchored, Render* render, GuiManager* gui, Textures* tex) :
        bounds(bounds),
        type(type),
        texture(texture),
        id(id),
        scale(scale),
        anchored(anchored),
        render(render), 
        gui(gui),
        tex(tex){}

    GuiControl(GuiControlType type) // Temp
    {

    }

    virtual ~GuiControl() {}

    virtual bool Update(float dt, bool DGSO = true, bool MGS = true)
    {
        return true;
    }

    virtual bool Draw(float dt) const
    {
        return true;
    }

    virtual void SetDimensions(Point size) {}

    Point GetPosition() const
    {
        return bounds.GetPosition();
    }

    void SetPosition(Point position)
    {
        bounds.SetPosition(position);
    }

    void NotifyObserver(float value = 0.0f, bool check = false)
    {
        observer->OnGuiMouseClickEvent(this, value, check);
    }

    virtual void Manipulate()
    {
        return;
    }

public:
    Input* input = nullptr;
    Render* render = nullptr;
    GuiManager* gui = nullptr;
    Audio* audio = nullptr;
    Scene* observer = nullptr;
    Textures* tex = nullptr;
    GuiString* text = nullptr;

    Collision collisionUtils;
    GuiControlType type;
    GuiControlState state;
    Rect bounds = {};
    suint id = 0;
    SDL_Texture* texture = nullptr;
    bool anchored = false;
    Point scale = { 0, 0 };
};

#endif // __GUICONTROL_H__