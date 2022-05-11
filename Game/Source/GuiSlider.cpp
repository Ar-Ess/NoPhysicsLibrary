#include "GuiSlider.h"
#include "GuiManager.h"
#include "Collider.h"

GuiSlider::GuiSlider(Rect bounds, SDL_Texture* texture, Point scale, suint id, bool anchored, Input* input, Render* render, GuiManager* gui, Audio* audio, Scene* scene, Textures* tex) :
    GuiControl(
        bounds,
        GuiControlType::SLIDER,
        texture,
        scale,
        id,
        anchored,
        input,
        render,
        gui,
        audio,
        scene,
        tex
    )
{
    SetDimensions(Point{ bounds.w, bounds.h });
}

GuiSlider::~GuiSlider()
{
    tex->UnLoad(texture);
}

bool GuiSlider::Update(float dt, bool DGSO, bool MGS)
{
    if (state == GuiControlState::DISABLED) return true;
    bool ret = true;

    if (DGSO)
        ret = DGSOUpdate(MGS);
    else
        ret = NormalUpdate();

    return ret;
}

bool GuiSlider::Draw(float dt) const
{
    Rect bar = { 0 };
    Rect sldr = { 0 };
    bar.SetDimensions(bounds.GetSize());
    sldr.SetDimensions(slider.GetSize());

    switch (state)
    {
    case GuiControlState::NORMAL:
        bar.SetPosition(normal);
        sldr.SetPosition(normalButton);
        break;
    case GuiControlState::FOCUSED:
        bar.SetPosition(focused);
        sldr.SetPosition(focusedButton);
        break;
    
    case GuiControlState::PRESSED:
        bar.SetPosition(pressed);
        sldr.SetPosition(pressedButton);
        break;

    case GuiControlState::DISABLED:
        bar.SetPosition(disabled);
        sldr.SetPosition(disabledButton);
        break;
    }

    render->DrawTexture(texture, bounds.GetPosition(), scale, anchored, &bar);
    render->DrawTexture(texture, slider.GetPosition(), scale, anchored, &sldr);

    if (text) text->Draw(dt);

    if (gui->debug) DebugDraw(dt);

    return true;
}

bool GuiSlider::DebugDraw(float dt) const
{
    Rect barRect = bounds;
    Rect sRect = slider;

    if (anchored)
    {
        barRect.x += render->camera.x;
        barRect.y += render->camera.y;
        sRect.x += render->camera.x;
        sRect.y += render->camera.y;
    }

    barRect.w *= scale.x;
    barRect.h *= scale.y;
    sRect.w *= scale.x;
    sRect.h *= scale.y;

    switch (state)
    {
    case GuiControlState::DISABLED:
        render->DrawRectangle(barRect, { 100, 100, 100, 80 });
        render->DrawRectangle(slider, { 150, 100, 100, 80 });
        break;

    case GuiControlState::NORMAL:
        render->DrawRectangle(barRect, { 0, 255, 0, 80 });
        render->DrawRectangle(slider, { 0, 200, 0, 80 });
        break;

    case GuiControlState::FOCUSED:
        render->DrawRectangle(barRect, { 255, 255, 0, 80 });
        render->DrawRectangle(slider, { 255, 255, 0, 80 });
        break;

    case GuiControlState::PRESSED:
        render->DrawRectangle(barRect, { 0, 255, 255, 80 });
        render->DrawRectangle(slider, { 0, 200, 200, 80 });
        break;
    }

    return true;
}

void GuiSlider::Delete()
{
    observer = nullptr;
    tex->UnLoad(texture);
    texture = nullptr;
}

void GuiSlider::SetDimensions(Point magnitudes)
{
    bounds.w = magnitudes.x;
    bounds.h = magnitudes.y / 8;

    slider.SetDimensions(bounds.h);
    slider.SetPosition(Point{ bounds.x - (slider.w / 2), bounds.y });

    disabled = { 0.0f, 0 * bounds.h };
    normal =   { 0.0f, 1 * bounds.h };
    focused =  { 0.0f, 2 * bounds.h };
    pressed =  { 0.0f, 3 * bounds.h };
    disabledButton = { 0.0f, 4 * bounds.h };
    normalButton =   { 0.0f, 5 * bounds.h };
    focusedButton  =  { 0.0f, 6 * bounds.h };
    pressedButton =  { 0.0f, 7 * bounds.h };

    if (text) Alignment(text, Point{ bounds.w, bounds.h }, nullptr).AlignTo(text->GetAlignment());
}

void GuiSlider::Manipulate()
{
    if (input->GetKey(lessKey) == KEY_DOWN)
    {
        suint substract = 5;
        if (input->GetKey(slowKey) == KEY_REPEAT) substract = 1;

        if (value - substract < 0) value = 0;
        else
            value -= substract;
        slider.x = (bounds.w * value / 100) + bounds.x - (slider.w / 2);
        NotifyObserver(((range.y - range.x) * value / 100) + range.x);

        return;
    }

    if (input->GetKey(moreKey) == KEY_DOWN)
    {
        suint add = 5;
        if (input->GetKey(slowKey) == KEY_REPEAT) add = 1;

        if (value + add > 100) value = 100;
        else
            value += add;
        slider.x = (bounds.w * value / 100) + bounds.x - (slider.w / 2);
        NotifyObserver(((range.y - range.x) * value / 100) + range.x);

        return;
    }
}

bool GuiSlider::NormalUpdate()
{
    Point mouse = input->GetMousePosition();
    bool on = collisionUtils.CheckCollision(Rect{ mouse, 1.0f, 1.0f }, slider);
    bool onTrail = collisionUtils.CheckCollision(Rect{ mouse, 1.0f, 1.0f }, bounds);
    bool click = (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT);
    bool release = (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP);
    float res = 0;

    switch (state)
    {
    case GuiControlState::NORMAL:
        state = GuiControlState::NORMAL;
        if (!on) break;
        audio->PlaySfx(Sfx::BUTTON_FOCUSSED);
        state = GuiControlState::FOCUSED;

    case GuiControlState::FOCUSED:
        if (!on)
        {
            state = GuiControlState::NORMAL;
            break;
        }
        if (!click) break;
        state = GuiControlState::PRESSED;
        audio->PlaySfx(Sfx::BUTTON_RELEASED);

    case GuiControlState::PRESSED:
        if (!click && on)
        {
            state = GuiControlState::FOCUSED;
            break;
        }
        if (!on && !onTrail)
        {
            state = GuiControlState::NORMAL;
            break;
        }
        res = (mouse.x - bounds.x) * 100 / bounds.w;
        if (res < 0) res = 0;
        if (res > 100) res = 100;
        value = res;
        slider.x = (bounds.w * res / 100) + bounds.x - (slider.w / 2);
        if (rips) NotifyObserver(((range.y - range.x) * value / 100) + range.x);
        break;
    }

    if (!rips && release) NotifyObserver(((range.y - range.x) * value / 100) + range.x);

    return true;
}

bool GuiSlider::DGSOUpdate(bool MGS)
{
    Point mouse = input->GetMousePosition();
    bool on = collisionUtils.CheckCollision(Rect{ mouse, 1.0f, 1.0f }, slider);
    bool click = (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT);
    float res = 0;
    GuiControlState prevState = state;

    if (!on && !click)
    {
        state = GuiControlState::NORMAL;
        if (!rips && prevState == GuiControlState::PRESSED && state != GuiControlState::PRESSED) NotifyObserver(((range.y - range.x) * value / 100) + range.x);
        return true;
    }

    switch (state)
    {
    case GuiControlState::NORMAL:
        state = GuiControlState::NORMAL;
        if (!on || MGS) break;
        audio->PlaySfx(Sfx::BUTTON_FOCUSSED);
        state = GuiControlState::FOCUSED;

    case GuiControlState::FOCUSED:
        if (!click) break;
        state = GuiControlState::PRESSED;
        audio->PlaySfx(Sfx::BUTTON_RELEASED);
        break;

    case GuiControlState::PRESSED:
        if (MGS)
        {
            state = GuiControlState::NORMAL;
            break;
        }
        if (!click)
        {
            state = GuiControlState::FOCUSED;
            break;
        }
        res = (mouse.x - bounds.x) * 100 / bounds.w;
        if (res < 0) res = 0;
        if (res > 100) res = 100;
        value = res;
        slider.x = (bounds.w * res / 100) + bounds.x - (slider.w / 2);
        if (rips) NotifyObserver(((range.y - range.x) * value / 100) + range.x);
        break;
    }

    if (!rips && prevState == GuiControlState::PRESSED && state != GuiControlState::PRESSED) NotifyObserver(((range.y - range.x) * value / 100) + range.x);

}
