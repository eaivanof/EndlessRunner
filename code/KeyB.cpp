#include "..\head\KeyB.h"

KeyB::KeyB()
{
    for (int i = 0; i < 256; ++i)
    {
        key[i] = false;
    }
}

void KeyB::keyDown(unsigned int _key)
{
    key[_key] = true;
}

void KeyB::keyUp(unsigned int _key)
{
    key[_key] = false;
}

bool KeyB::isKeyDown(unsigned int _key)
{
    return key[_key];
}