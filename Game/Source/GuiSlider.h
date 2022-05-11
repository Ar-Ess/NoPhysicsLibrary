#ifndef __GUISLIDER_H__
#define __GUISLIDER_H__

#include "GuiControl.h"
#include "Point.h"
#include "Collider.h"

class GuiSlider : public GuiControl
{
public:

    GuiSlider(Rect bounds, SDL_Texture* texture, Point scale, suint id, bool anchored, Input* input, Render* render, GuiManager* gui, Audio* audio, Scene* scene, Textures* tex);
    virtual ~GuiSlider();

    bool Update(float dt, bool DGSO = true, bool MGS = true);
    bool Draw(float dt) const;
    bool DebugDraw(float dt) const;
    void Delete();

    void SetRIPS(bool allowRIPS)
    {
        rips = allowRIPS;
    }
    void SetInitialValue(float initialValue)
    {
        value = initialValue;
        slider.x = (bounds.w * scale.x * value / 100) + (bounds.x * scale.x) - (slider.w * scale.x / 2);
        NotifyObserver(initialValue);
    }
    void SetRange(Point range)
    {
        this->range = range;
    }
    void SetKeys(suint lessKey = 80, suint moreKey = 79, suint slowKey = 225)
    {
        this->lessKey = lessKey;
        this->moreKey = moreKey;
        this->slowKey = slowKey;
    }

private:

    void SetDimensions(Point magnitudes);

    void Manipulate() override;

    bool NormalUpdate();
    bool DGSOUpdate(bool MGS);

private:

    Rect slider;
    Point range = {0, 100};
    float value = 0;

    bool rips = false;

    Point disabled = { 0, 0};
    Point normal = { 0, 0 };
    Point focused = { 0, 0 };
    Point pressed = { 0, 0 };

    Point disabledButton = { 0, 0 };
    Point normalButton = { 0, 0 };
    Point focusedButton = { 0, 0 };
    Point pressedButton = { 0, 0 };

    suint lessKey = 80;
    suint moreKey = 79;
    suint slowKey = 225;
};

#endif // __GUISLIDER_H__
