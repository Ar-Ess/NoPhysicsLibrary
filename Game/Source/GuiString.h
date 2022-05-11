#ifndef _GUISTRING_H_
#define _GUISTRING_H_

#include "GuiControl.h"
#include "AlignEnum.h"

struct _TTF_Font;
struct SDL_Texture;

class GuiString : public GuiControl
{
public:
	GuiString(Rect bounds, const char* string, suint fontIndex, suint id, Point scale, Render* render, GuiManager* gui, Textures* tex, bool anchored = false, SDL_Color color = {0, 0, 0, 255});

	virtual ~GuiString();

public:

	bool Draw(float dt) const;
	bool DebugDraw(float dt) const;
	void Delete();
	void SetString(const char* text, SDL_Color color);

	Align GetAlignment() const
	{
		return alignment;
	}

private:

	friend class Alignment;
	friend class FontSwitcher;
	suint fontId = 0;
	Align alignment = Align::CENTER;
	Point offset = { 0, 0 };
	SDL_Color color;
	const char* string = nullptr;

};

#endif // !_GUISTRING_H_
