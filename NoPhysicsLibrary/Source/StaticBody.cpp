#include "StaticBody.h"

StaticBody::StaticBody(Rect rect, float mass) : Body(BodyClass::STATIC_BODY, rect, mass)
{
}
