#ifndef __GUICHECKBOX_H__
#define __GUICHECKBOX_H__

#include "GuiControl.h"
#include "Point.h"
#include "Collider.h"

class GuiCheckBox : public GuiControl
{
public:

    GuiCheckBox(Rect bounds, SDL_Texture* texture, Point scale, suint id, bool anchored, Input* input, Render* render, GuiManager* gui, Audio* audio, Scene* scene, Textures* tex);
    virtual ~GuiCheckBox();

    bool Update(float dt, bool DGSO = true, bool MGS = true);
    bool Draw(float dt) const;
    void Delete();

private:

    bool NormalUpdate();
    bool DGSOUpdate(bool MGS);

    void SetDimensions(Point magnitudes);

    void DebugDraw() const;

    bool GetCheckState() const
    {
        return checked;
    }

    void Manipulate() override;

private:
    bool checked = false;

    Point checkNormal = {0, 0};
    Point checkFocused = { 0, 0 };
    Point checkPressed = { 0, 0 };
    Point uncheckNormal = { 0, 0 };
    Point uncheckFocused = { 0, 0 };
    Point uncheckPressed = { 0, 0 };
    Point disabled = { 0, 0 };
};

#endif // __GUICHECKBOX_H__
