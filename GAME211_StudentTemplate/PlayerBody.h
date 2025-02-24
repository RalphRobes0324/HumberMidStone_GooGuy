//
//  PlayerBody.h
//  DemoAI
//
//  Created by Gail Harris on 2021-Dec-23.
//

#ifndef PLAYERBODY_H
#define PLAYERBODY_H

#include <stdio.h>
#include "Body.h"
#include "GameManager.h"

class PlayerBody : public Body
{
protected:
    class GameManager* game;

private:
    float playerW, playerH;
    Uint32 sprite, counter, numFrames, numSprites;
    char animationName;

public:
    bool isGrounded = false; // Added isGrounded check for Jump (Maya)
    bool wallTouch = false;
    bool wallTouchLeft = false;
    bool wallTouchRight = false;
    bool facingRight = true;
    Vec3 GravForce, totalForce, frictionForce; // added force Vec3's for force checking (Maya)
    PlayerBody() : Body{}
    {
        game = nullptr;
    }

    // Note the last parameter in this constructor!
    // Look in GameManager.cpp to see how this is called.
    // Look in .cpp file, at Render(), to see why game_ is a parameter.
    PlayerBody(
        Vec3 pos_, Vec3 vel_, Vec3 accel_,
        float mass_,
        float radius_,
        float orientation_,
        float rotation_,
        float angular_,
        GameManager *game_
    ) : Body{
          pos_
        , vel_
        , accel_
        , mass_
        , radius_
        , orientation_
        , rotation_
        , angular_
    }
        , game{ game_ }
    {}
    
    // use the base class versions of getters

    bool OnCreate();
    void Render( float scale = 1.0f );
    void HandleEvents( const SDL_Event& event );
    void Update( float deltaTime );
    void setTexture( SDL_Texture* texture_ ) { texture = texture_; }
    bool HasCollidedWith(SDL_FRect rect); // added for calculating collision (Maya)
    bool HasCollidedSide(SDL_FRect rect);
    bool HasCollidedTop(SDL_FRect rect);
    bool HasCollidedBottom(SDL_FRect rect);
    void animationSwitch(char _anim);
    
};

#endif /* PLAYERBODY_H */
