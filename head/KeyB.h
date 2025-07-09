#pragma once

class KeyB
{
public:
    KeyB();
    void keyDown(unsigned int _key);
    void keyUp(unsigned int _key);
    bool isKeyDown(unsigned int _key);

private:
    bool key[256];
};
