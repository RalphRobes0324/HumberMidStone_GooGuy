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
}

void PlayerBody::HandleEvents(const SDL_Event& event)
{
    //Maya Added when Keydown
    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.scancode) {
            //A subtract 2 from velocity x until -8.0f velocity is achieved
        case(SDL_SCANCODE_A):
            if (vel.x > -4.0f && !wallTouchLeft)
                vel.x -= 1.0f;
            break;
            //D add 2 from velocity x until 8.0f velocity is achieved
        case(SDL_SCANCODE_D):
            if (vel.x < 4.0f && !wallTouchRight)
                vel.x += 1.0f;
            break;
            //When spacebar is pressed, add 12 to velocity y to simulate a jump is player is grounded
        // Elijah added wall jump
        case(SDL_SCANCODE_SPACE):
            if (isGrounded)
            {
                vel.y += 12.0f;
            }
            else if (!isGrounded && wallTouchLeft)
            {
                vel.y += 15.0f;
                vel.x += 6.0f;
            }
            else if (!isGrounded && wallTouchRight)
            {
                vel.y += 15.0f;
                vel.x -= 6.0f;
            }
            break;
        }
    }
    //if key up A or D make velocity x 0, optimize this later
    if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.scancode) {
        case (SDL_SCANCODE_A):
        case (SDL_SCANCODE_D):
            //vel.x = 0.0f;
            break;
        case (SDL_SCANCODE_LEFT):
            wallTouchLeft = true;
            wallTouchRight = false;
            break;
        case (SDL_SCANCODE_RIGHT):
            wallTouchRight = true;
            wallTouchLeft = false;
            break;
        case (SDL_SCANCODE_R):
            wallTouchLeft = false;
            wallTouchRight = false;
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
    GravForce = Vec3(0.0f, -9.8f * mass, 0.0f);
    if (vel.x < 0)
        frictionForce = Vec3(3.0f, 0, 0.0f);
    else if (vel.x > 0)
        frictionForce = Vec3(-3.0f, 0, 0.0f);
    else
        frictionForce = Vec3();

    totalForce = GravForce + frictionForce; //apply total force, right now total force is just gravity
    ApplyForce(totalForce);
    //Maya Added
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
    wallTouchLeft = false;
    wallTouchRight = false;

    // First, check if there is a general collision
    if (!HasCollidedWith(rect)) {
        return false;
    }

    // Calculate overlaps on all sides
    float overlapRight = (rect.x + rect.w) - (pos.x - radius);  // Overlap on left side
    float overlapLeft = (pos.x + radius) - rect.x;  // Overlap on right side
    float overlapTop = (rect.y + rect.h) - (pos.y - radius);  // Overlap on top side
    float overlapBottom = (pos.y + radius) - rect.y;  // Overlap on bottom side

    std::cout << "overLapRight: " << overlapRight << " overLapLeft: " << overlapLeft << " overLapTop: " << overlapTop << " overLapBottom: " << overlapBottom << std::endl;

    // Determine the smallest overlap (side or top/bottom)
    float minHorizontalOverlap = std::min(overlapLeft, overlapRight);
    float minVerticalOverlap = std::min(overlapTop, overlapBottom);

    // If horizontal overlap is smaller, it's a side collision
    std::cout << "minHorizontalOverlap: " << minHorizontalOverlap << " minVerticalOverlap: " << minVerticalOverlap << std::endl;
    if (minHorizontalOverlap < 0.5f && minVerticalOverlap < 0.90f) {
        if (overlapLeft < overlapRight) {
            std::cout << "Right side\n";
            wallTouchRight = true;
        }
        else {

            std::cout << "Left Side\n";
            wallTouchLeft = true;

        }
        return true;  // Side collision occurred
    }
    return false;
}

bool PlayerBody::HasCollidedTop(SDL_Rect rect)
{
    // First, check if there is a general collision
    if (!HasCollidedWith(rect)) {
        return false;
    }

    // Calculate overlaps on all sides
    float overlapRight = (rect.x + rect.w) - (pos.x - radius);  // Overlap on left side
    float overlapLeft = (pos.x + radius) - rect.x;  // Overlap on right side
    float overlapTop = (rect.y + rect.h) - (pos.y - radius);  // Overlap on top side
    float overlapBottom = (pos.y + radius) - rect.y;  // Overlap on bottom side

    // Determine the smallest overlap (side or top/bottom)
    float minHorizontalOverlap = std::min(overlapLeft, overlapRight);
    float minVerticalOverlap = std::min(overlapTop, overlapBottom);

    // If horizontal overlap is smaller, it's a side collision
    std::cout << "minHorizontalOverlap: " << minHorizontalOverlap << " minVerticalOverlap: " << minVerticalOverlap << std::endl;
    if (minVerticalOverlap > 0.8) {
        
        return true;  // Side collision occurred
    }
    return false;
}

