#include "Player.h"
#include <iostream>

Player::Player(const std::string& imagePath, float scaleFactor) : imagePath(imagePath), scaleFactor(scaleFactor), rotation(0.0f) 
{
    texture = LoadTexture(imagePath.c_str());
    texture.width *= scaleFactor;
    texture.height *= scaleFactor;
}

Texture2D& Player::getTexture() 
{
    return texture;
}

float Player::setRotation(float angle) 
{
    rotation = angle;
    return rotation;
}

float Player::getRotation() 
{
    return rotation;
}