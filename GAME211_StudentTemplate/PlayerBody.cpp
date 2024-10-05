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
    float w, h;

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

    //Convert player WxH to phyics space
    playerW = static_cast<int>(w) * game->getSceneWidth() / 1000;
    playerH = (600 - static_cast<int>(h))* game->getSceneHeight() / 600;

    // Convert character orientation from radians to degrees.
    float orientationDegrees = orientation * 180.0f / M_PI;

    SDL_RenderCopyEx(renderer, texture, nullptr, &square,
        orientationDegrees, nullptr, SDL_FLIP_NONE);

    meterBackgroundColour = { 50, 50, 50, 255 };
    jumpMeterBackground = { 10, 260, 28, -220 };
    SDL_SetRenderDrawColor(renderer, meterBackgroundColour.r, meterBackgroundColour.g, meterBackgroundColour.b, meterBackgroundColour.a);
    SDL_RenderFillRect(renderer, &jumpMeterBackground);

    jumpMeter = { 14, 250, 20, (int)-jumpPower * 2 };
    SDL_SetRenderDrawColor(renderer, meterColour.r, meterColour.g, meterColour.b, meterColour.a);
    SDL_RenderFillRect(renderer, &jumpMeter);
}

void PlayerBody::HandleEvents(const SDL_Event& event)
{
    //Maya Added when Keydown
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.scancode) {
            //A subtract 2 from velocity x until -8.0f velocity is achieved
        case(SDL_SCANCODE_A):
            if (vel.x > -8.0f)
                vel.x -= 1.0f;
            break;
            //D add 2 from velocity x until 8.0f velocity is achieved
        case(SDL_SCANCODE_D):
            if (vel.x < 8.0f)
                vel.x += 1.0f;
            break;
            //When spacebar is pressed, add 12 to velocity y to simulate a jump is player is grounded
        // Elijah added Jump Power/Change Updater
        case(SDL_SCANCODE_SPACE):
            if (isGrounded)
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
            break;
        }
    }
    //if key up A or D make velocity x 0, optimize this later
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.scancode) {
        case (SDL_SCANCODE_A):
        case (SDL_SCANCODE_D):
            vel.x = 0.0f;
            break;
        case(SDL_SCANCODE_SPACE):
            vel.y += 12.0f * (jumpPower * 0.01f);
            // print power of jump to console
            std::cout << "Goo-Guy jumped with " << jumpPower << "% power" << std::endl;
            // reset jump power to 0
            jumpPower = 0.0f;
            break;
        }
    }
}

void PlayerBody::Update(float deltaTime)
{
    // Update position, call Update from base class
    // Note that would update velocity too, and rotation motion

    Body::Update(deltaTime);

    //Maya Added
    //if the player isGrounded make the grav force 0 so you don't have any gravity,
    // if the character isn't grounded grav force is -9.8
    if (!isGrounded)
    {
        GravForce = Vec3(0.0f, -9.8f * mass, 0.0f);
    }
    else
    {
        GravForce = Vec3();
    }

    totalForce = GravForce; //apply total force, right now total force is just gravity
    ApplyForce(totalForce);
    //Maya Added

    // Elijah Added
    // change jump meter colour based on jump power
    if (jumpPower < 20)
        // red bar
        meterColour = { 255, 0, 0 , 255 };
    else if (jumpPower > 20 && jumpPower < 50)
        // yellow bar
        meterColour = { 255, 255, 0 , 255 };
    else if (jumpPower > 50 && jumpPower < 75)
        // green bar
        meterColour = { 0, 255, 0 , 255 };
    else
        // blue bar
        meterColour = { 0, 0, 225 , 255 };
}
// apply force function from physics 1 added by Maya
void PlayerBody::ApplyForce(Vec3 force)
{
    accel.y = force.y / mass; //apply force divided by mass to acceleration
    accel.x = force.x / mass;
}
/// <summary>
/// Maya added Has Collided With function
/// Function to check if the player has collided with one of the platforms
/// </summary>
/// <param name="rect"></param>
/// <returns></returns>
bool PlayerBody::HasCollidedWith(SDL_Rect rect)
{
    if ((pos.x - radius) > (rect.x + rect.w) || ((pos.x + radius) < rect.x) // x positions
        ||
        ((pos.y + radius) < (rect.y - rect.h)) || ((pos.y - radius) > rect.y)) 
    {
        return false;
    }
    
    return true; //collision has occured
}


/// <summary>
/// Checks the collision side of the build 
/// </summary>
/// <param name="rect"></param>
/// <returns></returns>
bool PlayerBody::HasCollidedSide(SDL_Rect rect)
{
    // First, check if there's any collision at all
    if (pos.x + playerW <= rect.x ||    // Player is to the left of rectB
        pos.x >= rect.x + rect.w ||     // Player is to the right of rectB
        pos.y + playerH <= rect.y ||    // Player is above rectB
        pos.y >= rect.y + rect.h) {     // Player is below rectB
        return false;  // No collision
    }


    // Check if the collision is happening on the left or right sides
    if ((pos.x + playerW > rect.x && pos.x < rect.x) ||  // Hitting rectB's left side
        (pos.x < rect.x + rect.w && pos.x + playerW > rect.x + rect.w)) {  // Hitting rectB's right side
        return true;
    }




    return false;
}

