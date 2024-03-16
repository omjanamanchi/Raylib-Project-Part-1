#include "raylib.h"
#include "Player.h"
#include <iostream>
#include <map>
#include <raymath.h>
using namespace std;

//Integer Preliminary Data
const int numColors = 10;
const int numCircles = 1;
const int circleRadius = 8;
const int width = 1600;
const int height = 1000;
const int targetFPS = 60;
const int margin = 10;

//Float Preliminary Data
const float scaleFactor = 0.16f;
const float max_velocity = 3.0f;
const float slow_rad = 115.0f;
const float max_speed = 3.5f;
const float max_force = 4.5f;
const float max_steer = PI/5.5f;
const float stop_rad = 1.0f;
const float time_lag = 6.0;

//Colors
const Color colorOptions[numColors] = {RED,ORANGE,YELLOW,GREEN,BLUE,PURPLE,PINK,DARKGRAY,SKYBLUE,LIME};

struct Circle 
{
   int x;
   int y;
   int radius;
   Color color;
};

int Xplay = 0;
int Yplay = 100;
float Xplay_vel = 0.0f;
float Yplay_vel = 0.0f;

Vector2 boost(Vector2 curr_vel, Vector2 target_pos, Vector2 curr_pos, float max_speed, float max_force, float max_steer, float slow_rad, float stop_rad, float deltaTime) {

    Vector2 expect = Vector2Subtract(target_pos, curr_pos);
    float distance = Vector2Length(expect);
    if (distance < stop_rad) 
    { 
        return { 0, 0 }; 
    }

    expect = Vector2Normalize(expect);
    float curr_angle = atan2(curr_vel.y, curr_vel.x);
    float expectAngle = atan2(expect.y, expect.x);
    float difference = expectAngle - curr_angle;

    if (difference > PI)
    {
        difference -= 2 * PI;
    }
    if (difference < -PI)
    {
        difference += 2 * PI;
    }

    difference = fmax(fmin(difference, max_steer * deltaTime), -max_steer * deltaTime);
    curr_angle += difference;

    expect.x = cos(curr_angle);
    expect.y = sin(curr_angle);

    if (distance < slow_rad) {
        float mappedSpeed = (distance/slow_rad) * max_speed;
        expect = Vector2Scale(expect, mappedSpeed);
    }
    else {
        expect = Vector2Scale(expect, max_speed);
    }

    Vector2 steering = Vector2Subtract(expect, curr_vel);
    steering = Vector2Scale(Vector2Normalize(steering), max_force);
    Vector2 newVelocity = Vector2Add(curr_vel, Vector2Scale(steering, deltaTime));

    if (Vector2Length(newVelocity) > max_speed) 
    { 
        newVelocity = Vector2Scale(Vector2Normalize(newVelocity), max_speed); 
    }
    return newVelocity;
}

void CreateCircles(Circle circles[sizeof(colorOptions)][numCircles]) 
{
  for (int i = 0; i < 10; i++) 
  {
    for (int j = 0; j < 5; j++) 
    {
      DrawCircle(circles[i][j].x, circles[i][j].y, circles[i][j].radius, circles[i][j].color);
    }
  }
}

int main(void) {
    InitWindow(width, height, "raylib [core] example - basic window");
    SetTargetFPS(targetFPS);
    Player myPlayer("C:\\Users\jshri\OneDrive\Desktop\Raylib Project\raylib-game-template-main\src\resources\ship.jpg", scaleFactor);
    int Xtarget = width/2;
    int Ytarget = height/2;
    float stopwatch = 0.0;
    Circle circles[numColors][numCircles];

    for (int i = 0; i < numColors; i++) 
    {
        for (int j = 0; j < numCircles; j++) 
        {
            circles[i][j].x = GetRandomValue(0 + margin, width - margin);
            circles[i][j].y = GetRandomValue(0 + margin, height - margin);
            circles[i][j].radius = circleRadius;
            circles[i][j].color = colorOptions[i];
        }
    }

    while (!WindowShouldClose()) 
    {  
        BeginDrawing();
        ClearBackground(BLACK);
        stopwatch += GetFrameTime();

        Vector2 curr_vel = { Xplay_vel, Yplay_vel };
        Vector2 curr_pos = { static_cast<float>(Xplay), static_cast<float>(Yplay) };
        Vector2 target_pos = { static_cast<float>(Xtarget), static_cast<float>(Ytarget) };
        Vector2 newVelocity = boost(curr_vel, target_pos, curr_pos, max_speed, max_force, max_steer, slow_rad, stop_rad, GetFrameTime());
        Xplay += newVelocity.x;
        Yplay += newVelocity.y;
        Xplay_vel = newVelocity.x;
        Yplay_vel = newVelocity.y;

        DrawRectangle(Xtarget - 10, Ytarget - 10, 20, 20, Color{ 255, 0, 0, 128 });
        if (stopwatch >= time_lag) 
        {
            stopwatch = 0.0;
            int random_col = GetRandomValue(0, numColors - 1);
            int random_cir = GetRandomValue(0, numCircles - 1);
            Xtarget = circles[random_col][random_cir].x;
            Ytarget = circles[random_col][random_cir].y;
        }

        Rectangle sourceRect = { 0.0f, 0.0f, (float)myPlayer.getTexture().width, (float)myPlayer.getTexture().height };
        Rectangle destRect = { (float)Xplay, (float)Yplay, (float)myPlayer.getTexture().width, (float)myPlayer.getTexture().height };
        Vector2 origin = { (float)myPlayer.getTexture().width/2, (float)myPlayer.getTexture().height / 2 };
        float rot = atan2(newVelocity.y, newVelocity.x) * (180.0/PI) + 90.0;
        myPlayer.setRotation(rot);
        DrawTexturePro(myPlayer.getTexture(), sourceRect, destRect, origin, myPlayer.getRotation(), WHITE);

        CreateCircles(circles);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}