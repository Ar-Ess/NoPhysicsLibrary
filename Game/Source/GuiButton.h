#ifndef __GUIBUTTON_H__
#define __GUIBUTTON_H__

#include "GuiControl.h"
#include "Point.h"
#include "Collider.h"

class GuiButton : public GuiControl
{
public:

    GuiButton(Rect bounds, SDL_Texture* texture, Point scale, suint id, bool anchored, Input* input, Render* render, GuiManager* gui, Audio* audio, Scene* scene, Textures* tex);
    virtual ~GuiButton();

    bool Update(float dt, bool DGSO = true, bool MGS = true);
    bool Draw(float dt) const;

private:

    bool NormalUpdate();
    bool DGSOUpdate(bool MGS);

    void SetDimensions(Point magnitudes);

    void DebugDraw() const;

    void Manipulate() override;

private:

    //Texture sections
    Point disabled = { 0, 0};
    Point normal = { 0, 0};
    Point focused = { 0, 0};
    Point pressed = { 0, 0};
};

#endif // __GUIBUTTON_H__
