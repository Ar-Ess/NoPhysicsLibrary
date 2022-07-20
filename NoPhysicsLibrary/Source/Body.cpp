#include "Body.h"

Body::Body(BodyClass clas, Rect rect, float mass)
{
	this->clas = clas;
	this->rect = rect;

	if (mass <= 0.0f) mass = 1.0f;
	this->mass = mass;

	this->properties.Set(true, 0);
}