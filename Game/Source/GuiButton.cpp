#include "GuiButton.h"
#include "GuiManager.h"

GuiButton::GuiButton(Rect bounds, SDL_Texture* texture, Point scale, suint id, bool anchored, Input* input, Render* render, GuiManager* gui, Audio* audio, Scene* scene, Textures* tex) :
    GuiControl(
        bounds,
        GuiControlType::BUTTON,
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

GuiButton::~GuiButton()
{
    this->audio = nullptr;
    this->gui = nullptr;
    this->input = nullptr;
    this->observer = nullptr;
    this->render = nullptr;
    this->tex->UnLoad(this->texture);
    this->texture = nullptr;
    this->tex = nullptr;

    if (!this->text) return;
    RELEASE(this->text);
    this->text = nullptr;
}

bool GuiButton::Update(float dt, bool DGSO, bool MGS)
{
    if (state == GuiControlState::DISABLED) return true;
    bool ret = true;

    if (DGSO)
        ret = DGSOUpdate(MGS);
    else
        ret = NormalUpdate();

    return ret;
}

bool GuiButton::Draw(float dt) const
{
    Rect section = { 0, 0, bounds.w, bounds.h };
    switch (state)
    {
    case GuiControlState::DISABLED:
        section.x = disabled.x;
        section.y = disabled.y;
        break;
    case GuiControlState::NORMAL:
        section.x = normal.x;
        section.y = normal.y;
        break;
    case GuiControlState::FOCUSED:
        section.x = focused.x;
        section.y = focused.y;
        break;
    case GuiControlState::PRESSED:
        section.x = pressed.x;
        section.y = pressed.y;
        break;
    }

    render->DrawTexture(texture, bounds.GetPosition(), scale, anchored, &section);

    if (text) text->Draw(dt);

    if (gui->debug) DebugDraw();

    return false;
}

void GuiButton::DebugDraw() const
{
    Rect buttonRect = bounds;

    if (anchored)
    {
        buttonRect.x += (int)render->camera.x;
        buttonRect.y += (int)render->camera.y;
    }

    buttonRect.w *= scale.x;
    buttonRect.h *= scale.y;

    switch (state)
    {
    case GuiControlState::DISABLED: render->DrawRectangle(buttonRect, { 100, 100, 100, 80 });
        break;
    case GuiControlState::NORMAL: render->DrawRectangle(buttonRect, { 0, 255, 0, 80 });
        break;
    case GuiControlState::FOCUSED: render->DrawRectangle(buttonRect, { 255, 255, 0, 80 });
        break;
    case GuiControlState::PRESSED: render->DrawRectangle(buttonRect, { 0, 255, 255, 80 });
        break;
    }
}

void GuiButton::Manipulate()
{
    if (input->GetKey(SDL_SCANCODE_RETURN) == KEY_DOWN) NotifyObserver();
}

bool GuiButton::NormalUpdate()
{
    Point mouse = input->GetMousePosition();
    bool on = collisionUtils.CheckCollision(Rect{ mouse, 1.0f, 1.0f }, { bounds.GetPosition(), bounds.GetDimensions().Multiply(scale) });
    bool click = (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT);
    bool release = (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_UP);

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
        if (!on)
        {
            state = GuiControlState::NORMAL;
            break;
        }
        if (click) break;

        state = GuiControlState::FOCUSED;
        NotifyObserver();
        break;
    }

    return true;
}

bool GuiButton::DGSOUpdate(bool MGS)
{
    Point mouse = input->GetMousePosition();
    bool on = collisionUtils.CheckCollision(Rect{ mouse, 1.0f, 1.0f }, { bounds.GetPosition(), bounds.GetDimensions().Multiply(scale) });
    bool click = (input->GetMouseButtonDown(SDL_BUTTON_LEFT) == KEY_REPEAT);
    GuiControlState prevState = state;


    if (!on && !click)
    {
        if (prevState == GuiControlState::PRESSED) NotifyObserver();
        state = GuiControlState::NORMAL;
        prevState = state;
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
        if (click) break;

        state = GuiControlState::FOCUSED;
        NotifyObserver();
        break;
    }

    prevState = state;
}

void GuiButton::SetDimensions(Point magnitude)
{
    bounds.w = magnitude.x;
    bounds.h = magnitude.y / 4;

    disabled = { 0.0f, 0 * bounds.h};
    normal = { 0.0f, 1 * bounds.h };
    focused = { 0.0f, 2 * bounds.h };
    pressed = { 0.0f, 3 * bounds.h };

    if (text) Alignment(text, Point{ bounds.w, bounds.h }, nullptr).AlignTo(text->GetAlignment());
}
