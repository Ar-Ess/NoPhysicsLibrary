#ifndef __GUI_MANAGER_H__
#define __GUI_MANAGER_H__

#include "Scene.h"
#include "GuiControl.h"
#include "GuiString.h"
#include "GuiSlider.h"
#include "SDL/include/SDL_pixels.h"
#include "AlignEnum.h"

#include <vector>
#include <assert.h>

class ControlSettings;
struct SDL_Texture;
struct _TTF_Font;
enum class GuiControlType;

struct Texture
{
	Texture(SDL_Texture* texture, Point size, GuiControlType guiType)
	{
		this->texture = texture;
		this->size = size;
		this->type = guiType;
	}

	SDL_Texture* texture = nullptr;
	Point size = {};
	GuiControlType type;
};

class Alignment
{
public:
	Alignment(GuiControl* control, Point size, ControlSettings* ret)
	{
		this->control = control;
		this->size = size;
		this->ret = ret;
	}

	ControlSettings* AlignTo(Align align = TOP_LEFT, Point offset = {0, 0})
	{
		GuiString* text = control->text;
		switch (control->type)
		{
		case GuiControlType::BUTTON: ButtonAlignment(text, align); break;
		case GuiControlType::SLIDER: SliderAlignment(text, align); break;
		case GuiControlType::CHECKBOX: CheckboxAlignment(text, align); break;
		}

		text->alignment = align;

		text->offset += offset;
		text->offset *= text->scale;

		return ret;
	}

private:

	void ButtonAlignment(GuiString* text, Align align)
	{

		switch (align)
		{
		case CENTER:
			text->offset = { (size.x / 2) - (text->bounds.w / 2), (size.y / 2) - (text->bounds.h / 2) };
			break;

		case BOTTOM_LEFT:
			text->offset = { 0.0f, size.y - text->bounds.h };
			break;

		case TOP_LEFT:
			text->offset = { 0.0f, 0.0f };
			break;

		case CENTER_LEFT:
			text->offset = { 0.0f, (size.y / 2) - (text->bounds.h / 2) };
			break;

		case BOTTOM_RIGHT:
			text->offset = { size.x - text->bounds.w, size.y - text->bounds.h };
			break;

		case TOP_RIGHT:
			text->offset = { size.x - text->bounds.w, 0.0f };
			break;

		case CENTER_RIGHT:
			text->offset = { size.x - text->bounds.w, (size.y / 2) - (text->bounds.h / 2) };
			break;

		case CENTER_TOP:
			text->offset = { (size.x / 2) - (text->bounds.w / 2), 0.0f };
			break;

		case CENTER_BOTTOM:
			text->offset = { (size.x / 2) - (text->bounds.w / 2), size.y - text->bounds.h };
			break;
		}
	}

	void SliderAlignment(GuiString* text, Align align)
	{
		switch (align)
		{
		case CENTER:
			text->offset = { (size.x / 2) - (text->bounds.w / 2), (size.y / 2) - (text->bounds.h / 2) };
			break;

		case BOTTOM_LEFT:
			text->offset = { 0.0f, size.y };
			break;

		case TOP_LEFT:
			text->offset = { 0.0f, -text->bounds.h };
			break;

		case CENTER_LEFT:
			text->offset = { -text->bounds.w, (size.y / 2) - (text->bounds.h / 2) };
			break;

		case BOTTOM_RIGHT:
			text->offset = { size.x - text->bounds.w, size.y };
			break;

		case TOP_RIGHT:
			text->offset = { size.x - text->bounds.w, -text->bounds.h };
			break;

		case CENTER_RIGHT:
			text->offset = { size.x, (size.y / 2) - (text->bounds.h / 2) };
			break;

		case CENTER_TOP:
			text->offset = { (size.x / 2) - (text->bounds.w / 2), -text->bounds.h };
			break;

		case CENTER_BOTTOM:
			text->offset = { (size.x / 2) - (text->bounds.w / 2), size.y };
			break;
		}
	}

	void CheckboxAlignment(GuiString* text, Align align)
	{
		switch (align)
		{
		case CENTER:
			text->offset = { (size.x / 2) - (text->bounds.w / 2), (size.y / 2) - (text->bounds.h / 2) };
			break;

		case CENTER_LEFT:
			text->offset = { -size.x, (size.y / 2) - (text->bounds.h / 2) };
			break;

		case BOTTOM_LEFT:
			text->offset = { -size.x, size.y };
			break;

		case TOP_LEFT:
			text->offset = { -size.x, -text->bounds.h };
			break;

		case BOTTOM_RIGHT:
			text->offset = { size.x,  size.y };
			break;

		case TOP_RIGHT:
			text->offset = { size.x, -text->bounds.h };
			break;

		case CENTER_RIGHT:
			text->offset = { size.x, (size.y / 2) - (text->bounds.h / 2) };
			break;

		case CENTER_TOP:
			text->offset = { (size.x / 2) - (text->bounds.w / 2), -text->bounds.h };
			break;

		case CENTER_BOTTOM:
			text->offset = { (size.x / 2) - (text->bounds.w / 2),  size.y };
			break;
		}
	}

private:

	GuiControl* control = nullptr;
	ControlSettings* ret = nullptr;
	Point size = {};
};

class TextureSwitcher
{
public:
	TextureSwitcher(GuiControl* control, std::vector<Texture*>* textures)
	{
		this->control = control;
		this->textures = textures;
	}

	void To(suint newTextureId)
	{
		assert(newTextureId > 0 && newTextureId < textures->size());
		Texture* newTexture = textures->at(newTextureId);
		control->texture = newTexture->texture;
		control->SetDimensions(newTexture->size);
	}

	bool Next()
	{
		suint size = textures->size();
		int index = -1;
		for (suint i = 0; i < size; ++i)
		{
			if (textures->at(i)->texture == control->texture)
			{
				index = i;
				break;
			}
		}

		if (index == -1) return false;

		Texture* newTexture = textures->at(index);

		for (suint i = index + 1; i != index; ++i)
		{
			if (i == (size - 1)) i = 0;

			if (textures->at(i)->type == control->type)
			{
				newTexture = textures->at(i);
				break;
			}
		}

		control->texture = newTexture->texture;
		control->SetDimensions(newTexture->size);

		return true;
	}

	bool Prev()
	{
		suint size = textures->size();
		int index = -1;
		for (suint i = 0; i < size; ++i)
		{
			if (textures->at(i)->texture == control->texture)
			{
				index = i;
				break;
			}
		}

		if (index == -1) return false;

		Texture* newTexture = textures->at(index);

		for (int i = index - 1; i != index; --i)
		{
			if (i == -1) i = size - 1;

			if (textures->at(i)->type == control->type)
			{
				newTexture = textures->at(i);
				break;
			}
		}

		control->texture = newTexture->texture;
		control->SetDimensions(newTexture->size);

		return true;
	}

private:

	GuiControl* control = nullptr;
	std::vector<Texture*>* textures = {};
};

class FontSwitcher
{
public:
	FontSwitcher(GuiControl* control, std::vector<_TTF_Font*>* fonts)
	{
		this->control = control;
		this->fonts = fonts;
	}

	void To(suint newFontId)
	{
		assert(newFontId >= 0 && newFontId < fonts->size());

		GuiString* string = control->text;
		Point position = control->GetPosition();
		Align alignment = string->alignment;
		GuiString* newString = new GuiString(string->bounds, string->string, newFontId, string->id, string->scale, string->render, string->gui, string->tex, string->anchored, string->color);
		
		RELEASE(string);
		newString->SetPosition(position);
		newString->alignment = alignment;
		if (control->type != GuiControlType::TEXT) control->text = newString;
		else control = newString;

		Alignment align(control, control->bounds.GetSize(), nullptr);
		align.AlignTo(alignment);
	}

	bool Next()
	{
		GuiString* str = nullptr;
		if (control->type != GuiControlType::TEXT) str = control->text;
		else str = (GuiString*)control;
		short int newIndex = str->fontId;

		if (str->fontId == fonts->size() - 1) newIndex = -1;

		newIndex++;

		To(newIndex);

		return true;
	}

	bool Prev()
	{
		GuiString* str = nullptr;
		if (control->type != GuiControlType::TEXT) str = control->text;
		else str = (GuiString*)control;
		short int newIndex = str->fontId;

		if (str->fontId == 0) newIndex = fonts->size();

		newIndex--;

		To(newIndex);

		return true;
	}

private:

	GuiControl* control = nullptr;
	std::vector<_TTF_Font*>* fonts = {};
};

class ControlSettings
{
public:
	ControlSettings(GuiControl* control)
	{
		this->control = control;
	}

	Alignment AddGuiString(const char* text, suint fontIndex = 0, SDL_Color color = { 0, 0, 0, 255 })
	{
		// You already created a text for this Gui Control, no more that 1 text is allowed
		assert(control->type != GuiControlType::TEXT && control->text == nullptr);

		control->text = new GuiString(control->bounds, text, fontIndex, control->id, control->scale, control->render, control->gui, control->tex, control->anchored, color);

		return Alignment(control, Point{control->bounds.w, control->bounds.h}, this);
	}

	// SliderSettings function allows to modify some parts of the functionality of a slider:
	//   - Initial Value: Set an initial value in which the slider will start
	//   - Allow RIPS: this system stands for "Retrieve Information Pressed State". If this option is false, 
	//                    the slider will retieve the value when the the click button is released. If the option
	//                    it true, it will retrieve the value each frame that the slider is pressed.
	//   - Range: this variable allows to set the value given at the minimum position and the value given at the
	//            maximum position of the slider. Point{minimum, maximum}
	//   - Keys: 
	ControlSettings* SliderSettings(float initValue = 0.0f, bool allowRIPS = false, Point range = {0, 100}, suint subsKey = 80, suint addKey = 79, suint slowKey = 225)
	{
		// You tried to modify slider setting in another gui control. SliderSettings is only for Sliders
		assert(control->type == GuiControlType::SLIDER);

		GuiSlider* slider = (GuiSlider*)control;
		slider->SetRIPS(allowRIPS);
		slider->SetInitialValue(initValue);
		slider->SetRange(range);
		slider->SetKeys(subsKey, addKey, slowKey);

		return this;
	}

	ControlSettings* TextSettings(const char* text = "", SDL_Color color = {255, 255, 255, 255})
	{
		// You tried to modify slider setting in another gui control. SliderSettings is only for Sliders
		assert(control->type == GuiControlType::TEXT);

		GuiString* string = (GuiString*)control;
		string->SetString(text, color);

		return this;
	}


private:

	GuiControl* control = nullptr;
};

class GuiManager
{
public:

	GuiManager(Input* input, Render* Render, Audio* audio, Textures* texture);

	virtual ~GuiManager();

	bool Start(Scene* scene);

	bool Update(float dt = 0.0f);

	bool Draw(float dt = 0.0f);

	bool CleanUp();

	ControlSettings CreateGuiControl(GuiControlType type, Point position = { 0, 0 }, suint texIndex = 0, bool anchored = false, Point scale = {1, 1});

	void DestroyGuiControl(suint index);

	int CreateTexture(const char* path, GuiControlType type);

	TextureSwitcher ChangeTexture(suint controlIndex);
	
	void DestroyTexture(suint index);

	void CreateFont(const char* path, suint fontSize);

	SDL_Texture* PrintFont(const char* text, SDL_Color color, suint fontIndex, int endLine = -1);

	FontSwitcher ChangeFont(suint controlIndex);

	/*
	GuiSettings function allows to modify some parts of the functionality of the gui :
	   - Allow Gui Navigation (allowGuiNav) allows user to navigate through controls using keyboard.
	   - Navigation Key (navKey) sets the key to navigate through controls. Default key = Tab Button. Using SDL_SCANCODE enum. "allowGuiNav" required to be allowed.
	   - Allow Drag Gui State Out (allowDGSO) allows user to mantain press state of a control when mouse is outside bounding box. Allows to do click action outside the control collision.
	   - Allow Multiple Gui Selection (allowMGS) allows user to select various gui controls to modify them at the same time. Mantaining one control pressed, user can move mouse towards other controls to modify them toghether. It is required that "allowDGSO" is allowed.
	*/
	void GuiSettings(bool allowGuiNav = true, suint navKey = 43, bool allowDGSO = false, bool allowMGS = false)
	{
		this->allowGuiNav = allowGuiNav;
		this->navKey = navKey;
		this->allowDGSO = allowDGSO;
		this->allowDGSO ? this->allowMGS = allowMGS : this->allowMGS = false;
	}

private:
	
	friend class GuiString;
	bool CalculateSize(const char* text, suint fontIndex, Point* result) const;

	bool InitializeFonts();

	void SelectButtonsLogic();

	void ActivateControls(bool active);

public:

	bool debug = false;

private:

	Input* input = nullptr;
	Render* render = nullptr;
	Audio* audio = nullptr;
	Textures* texture = nullptr;
	Scene* scene = nullptr;

	std::vector<GuiControl*> controls;
	std::vector<Texture*> textures;
	std::vector<_TTF_Font*> fonts;
	
	suint navKey = 43;
	int idSelection = -1;

	bool allowGuiNav = true;
	bool allowDGSO = false;
	bool allowMGS = false;

};

#endif // __GUI_MANAGER_H__