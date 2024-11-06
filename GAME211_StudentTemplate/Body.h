#ifndef BODY_H
#define BODY_H

#include <MMath.h>
#include <VMath.h>
#include <SDL.h>
#include <SDL_image.h>

using namespace MATH;

class Body {
protected:
	// inherited classes can access this
	Vec3 pos;
	Vec3 vel;
	Vec3 accel;
	float mass;
    float orientation;		// facing this direction
    float rotation;			// rotating at this speed (radians per second?)
    float angular;          // angular acceleration
    float radius;           // for getting near walls

	Vec3 imageSizeWorldCoords;
    SDL_Surface* image;
    SDL_Texture* texture;

public:
    Body();
	Body(
        Vec3 pos_, Vec3 vel_, Vec3 accel_,
        float mass_,
        float radius_,
        float orientation_,
        float rotation_,
        float angular_
    );
	virtual ~Body();
    virtual void Update(float deltaTime);
    virtual void ApplyForce(Vec3 force_);
    virtual Vec3 getPos() { return pos; }
    virtual Vec3 getVel() { return vel; }
    virtual float getRadius() { return radius; }
    virtual Vec3 getAccel() { return accel; }
    virtual float getMass() { return mass; }
    virtual float getOrientation() { return orientation; }
    virtual float getRotation() { return rotation; }
    virtual float getAngular() { return angular; }

    virtual void setImage(SDL_Surface* image_) { image = image_; }
    virtual SDL_Surface* getImage() { return image; }

    virtual void setImageSizeWorldCoords(Vec3 imageSizeWorldCoords_)
    {
        imageSizeWorldCoords = imageSizeWorldCoords_;
	}

    virtual void setTexture( SDL_Texture* texture_ ) { texture = texture_; }
    virtual SDL_Texture* getTexture() { return texture; }

    virtual void HandleEvents( const SDL_Event& event );

	// Added this as public to deal with my demo
	// so that mouse position can be copied into a Body.
    virtual void setPos( Vec3 pos );
    virtual void setAccel(Vec3 accel); //added setter for acceleration (Maya)
    virtual void setVel(Vec3 vel); //added setter for velocity (Maya)
};

#endif /* BODY_H */
