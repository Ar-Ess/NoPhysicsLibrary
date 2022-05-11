#ifndef __GUIIMAGE_H__
#define __GUIIMAGE_H__

#include "GuiControl.h"
#include "Point.h"

class GuiImage : public GuiControl
{
public:

    GuiImage(Rect bounds, SDL_Texture* texture, Point scale, suint id, bool anchored, Input* input, Render* render, GuiManager* gui, Audio* audio, Scene* scene, Textures* tex);
    virtual ~GuiImage();

    bool Draw(float dt) const;
    void Delete();

private:
    void SetDimensions(Point magnitudes);

    void Manipulate() override;

private:

};

#endif // __GUIIMAGE_H__
