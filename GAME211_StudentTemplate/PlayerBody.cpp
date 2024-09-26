//
//  PlayerBody.cpp
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#include "PlayerBody.h"

bool PlayerBody::OnCreate()
{
    image = IMG_Load("Pacman.png");
    SDL_Renderer* renderer = game->getRenderer();
    texture = SDL_CreateTextureFromSurface(renderer, image);
    if (image == nullptr) {
        std::cerr << "Can't open the image" << std::endl;
        return false;
    }
    return true;
}

void PlayerBody::Render(float scale)
{
    // This is why we need game in the constructor, to get the renderer, etc.
    SDL_Renderer* renderer = game->getRenderer();
    Matrix4 projectionMatrix = game->getProjectionMatrix();

    // square represents the position and dimensions for where to draw the image
    SDL_Rect square;
    Vec3 screenCoords;
    float    w, h;

    // convert the position from game coords to screen coords.
    screenCoords = projectionMatrix * pos;

    // Scale the image, in case the .png file is too big or small
    w = image->w * scale;
    h = image->h * scale;

    // The square's x and y values represent the top left corner of 
    // where SDL will draw the .png image.
    // The 0.5f * w/h offset is to place the .png so that pos represents the center
    // (Note the y axis for screen coords points downward, hence subtraction!!!!)
    square.x = static_cast<int>(screenCoords.x - 0.5f * w);
    square.y = static_cast<int>(screenCoords.y - 0.5f * h);
    square.w = static_cast<int>(w);
    square.h = static_cast<int>(h);

    // Convert character orientation from radians to degrees.
    float orientationDegrees = orientation * 180.0f / M_PI;

    SDL_RenderCopyEx(renderer, texture, nullptr, &square,
        orientationDegrees, nullptr, SDL_FLIP_NONE);

    // change jump meter colour based on jump power
    if(jumpPower < 25)
        // red bar
        meterColour = { 255, 0, 0 , 255 };
    else if (jumpPower > 25 && jumpPower < 50)
        // yellow bar
        meterColour = { 255, 255, 0 , 255 };
    else if (jumpPower > 50 && jumpPower < 75)
        // green bar
        meterColour = { 0, 255, 0 , 255 };
    else
        // blue bar
        meterColour = { 0, 0, 225 , 255 };

    // render jump meter
    SDL_RenderFillRect(renderer, &jumpMeter);
    SDL_Rect filledMeter = { 10, 250, 20, -jumpPower * 2};
    SDL_SetRenderDrawColor(renderer, meterColour.r, meterColour.g, meterColour.b, meterColour.a);
    SDL_RenderFillRect(renderer, &filledMeter);
}

void PlayerBody::HandleEvents(const SDL_Event& event)
{
    if (event.type == SDL_KEYDOWN)
    {
        // check that playing is pressing space
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            // check if at full jump power
            if (jumpPower == 100.0f)
                // start decreasing jump power
                jumpChange = -1.0f;
            // check if no jump power
            else if (jumpPower == 0.0f)
                // start increaseing jump power
                jumpChange = 1.0f;
            // update jump power
            jumpPower += jumpChange;
            // print jump power to console
            std::cout << jumpPower << std::endl;
        }
    }
    else if (event.type == SDL_KEYUP)
    {
        // check that user released space
        if (event.key.keysym.scancode == SDL_SCANCODE_SPACE)
        {
            // print power of jump to console
            std::cout << "Goo-Guy jumped with " << jumpPower << "% power" << std::endl;
            // reset jump power to 0
            jumpPower = 0.0f;
        }
    }
}

void PlayerBody::Update( float deltaTime )
{
    // Update position, call Update from base class
    // Note that would update velocity too, and rotation motion

    Body::Update( deltaTime );

}

