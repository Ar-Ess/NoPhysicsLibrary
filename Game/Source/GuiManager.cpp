#include "GuiManager.h"

#include "GuiButton.h"
#include "GuiCheckBox.h"
#include "GuiSlider.h"
#include "GuiImage.h"
#include "GuiString.h"

#include "SDL_ttf/include/SDL_ttf.h"
#pragma comment( lib, "../Game/Source/External/SDL_ttf/libx86/SDL2_ttf.lib" )

#include "Log.h"

GuiManager::GuiManager(Input* input, Render* render, Audio* audio, Textures* texture)
{
	this->texture = texture;
	this->input = input;
	this->render = render;
	this->audio = audio;
}

GuiManager::~GuiManager()
{
}

bool GuiManager::Start(Scene* scene)
{
	InitializeFonts();

	CreateTexture("Assets/Textures/UI/button_default_set.png", GuiControlType::BUTTON);
	CreateTexture("Assets/Textures/UI/slider_default_set.png", GuiControlType::SLIDER);
	CreateTexture("Assets/Textures/UI/checkbox_default_set.png", GuiControlType::CHECKBOX);
	CreateTexture("Assets/Textures/UI/image_default_set.png", GuiControlType::IMAGE);
	CreateFont("Fonts/manaspace.regular.ttf", 18);
	CreateFont("Fonts/manaspace.regular.ttf", 32);

	debug = false;
	this->scene = scene;
	idSelection = -1;
	return true;
}

bool GuiManager::Update(float dt)
{
	suint size = controls.size();
	bool MGS = false;
	suint index = 0;

	if (!allowMGS)
	{
		for (suint i = 0; i < size; ++i)
		{
			if (controls[i]->state == GuiControlState::PRESSED)
			{
				MGS = true;
				index = i;
				break;
			}
		}
	}

	for (suint i = 0; i < size; ++i)
	{
		bool active = MGS;
		if (i == index) active = false;
		controls[i]->Update(dt, allowDGSO, active);
	}

	if (allowGuiNav) SelectButtonsLogic();

	return true;
}

bool GuiManager::Draw(float dt)
{
	suint size = controls.size();
	for (suint i = 0; i < size; ++i)
	{
		controls[i]->Draw(dt);
	}

	return true;
}

ControlSettings GuiManager::CreateGuiControl(GuiControlType type, Point position, suint texIndex, bool anchored, Point scale)
{
	GuiControl* control = nullptr;

	if (textures.size() <= texIndex) texIndex = 0;
	Texture* tex = textures.at(texIndex);
	Rect bounds = { position.x, position.y, tex->size.x, tex->size.y };

	switch (type)
	{
	case GuiControlType::BUTTON:
		control = new GuiButton(bounds, tex->texture, scale, controls.size(), anchored, input, render, this, audio, scene, texture); 
		break;
	case GuiControlType::IMAGE:
		control = new GuiImage(bounds, tex->texture, scale, controls.size(), anchored, input, render, this, audio, scene, texture);
		break;
	case GuiControlType::CHECKBOX: control = new GuiCheckBox(bounds, tex->texture, scale, controls.size(), anchored, input, render, this, audio, scene, texture); break;
	case GuiControlType::SLIDER:
		control = new GuiSlider(bounds, tex->texture, scale, controls.size(), anchored, input, render, this, audio, scene, texture);
		break;
	case GuiControlType::TEXT: 
		bounds.SetDimensions({0, 0});
		control = new GuiString(bounds, "", texIndex, controls.size(), scale, render, this, texture, anchored);
		break;
	}

	if (control != nullptr) controls.push_back(control);

	return ControlSettings(control);
}

void GuiManager::DestroyGuiControl(suint index)
{
	RELEASE(controls.at(index));
	controls.erase(controls.begin() + index);
}

int GuiManager::CreateTexture(const char* path, GuiControlType type)
{
	Point size = {};
	SDL_Texture* tex = texture->Load(path, &size);
	textures.push_back(new Texture(tex, size, type));

	return textures.size() - 1;
}

void GuiManager::DestroyTexture(suint index)
{
	texture->UnLoad(textures.at(index)->texture);
	textures.erase(textures.begin() + index);
}

void GuiManager::CreateFont(const char* path, suint fontSize)
{
	_TTF_Font* font = TTF_OpenFont(path, fontSize);

	if (font == NULL)
	{
		LOG("Could not load TTF font with path: %s. TTF_OpenFont: %s", path, TTF_GetError());
		return;
	}

	fonts.push_back(font);
}

SDL_Texture* GuiManager::PrintFont(const char* text, SDL_Color color, suint fontIndex, int endLine)
{
	SDL_Texture* ret = nullptr;
	Point result = {0, 0};
	_TTF_Font* font = fonts.at(fontIndex);

	if (endLine == -1)
	{
		CalculateSize(text, fontIndex, &result);
		endLine = result.x;
	}
	SDL_Surface* surface = TTF_RenderText_Blended_Wrapped(font, text, color, endLine);

	if (surface == nullptr) return nullptr;

	ret = texture->LoadSurface(surface, nullptr);
	SDL_FreeSurface(surface);

	return ret;
}

FontSwitcher GuiManager::ChangeFont(suint controlIndex)
{
	assert(controlIndex >= 0 && controlIndex < controls.size());

	return FontSwitcher(controls.at(controlIndex), &fonts);
}

TextureSwitcher GuiManager::ChangeTexture(suint controlIndex)
{
	return TextureSwitcher(controls.at(controlIndex), &textures);
}

bool GuiManager::CalculateSize(const char* text, suint fontIndex, Point* result) const
{
	int w, h;

	if (TTF_SizeText(fonts.at(fontIndex), text, &w, &h) != 0) return false;

	result->x = w;
	result->y = h;

	return true;
}

bool GuiManager::InitializeFonts()
{
	LOG("Init True Type Font library");
	bool ret = true;

	if (TTF_Init() == -1)
	{
		LOG("SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError());
		ret = false;
	}

	return ret;
}

void GuiManager::SelectButtonsLogic()
{
	if (input->GetKey(navKey) == KEY_DOWN)
	{
		++idSelection;
		if (idSelection == controls.size())
		{
			idSelection = -1;
			ActivateControls(true);
			return;
		}

		ActivateControls(false);
		controls[idSelection]->state = GuiControlState::NORMAL;
	}

	if (idSelection != -1) controls[idSelection]->Manipulate();

	return;
}

void GuiManager::ActivateControls(bool active)
{
	suint size = controls.size();
	for (suint i = 0; i < size; ++i) !active ? controls[i]->state = GuiControlState::DISABLED : controls[i]->state = GuiControlState::NORMAL;
}

bool GuiManager::CleanUp()
{
	LOG("Freeing True Type fonts and library");

	suint size = fonts.size();
	for (suint i = 0; i < size; ++i) TTF_CloseFont(fonts.at(i));
	fonts.shrink_to_fit();
	fonts.clear();
	TTF_Quit();

	return true;
}
