#ifndef PLAYER_H
#define PLAYER_H
#include <string>
#include "raylib.h"

class Player 
{
    public:
        Player(const std::string& imagePath, float scaleFactor);
        Texture2D& getTexture();
        float getRotation();
        float setRotation(float angle);

    private:
        std::string imagePath;
        float scaleFactor;
        Texture2D texture;
        float rotation;
};

#endif#pragma once
