#pragma once  
#include "surface.h"  
#include "template.h"  
#include "windows.h"  
#include "sprites.h"
#include <LDtkLoader/Project.hpp>
#include <memory>

namespace Tmpl8 {  
class Surface;  

class Collision;

class Player  
{  
public:  

	Player() = default;
	Player(const HBox& playerSize, float xvelocity, float yvelocity);
	~Player() = default;
	Tmpl8::vec2 GetPosition() { return Tmpl8::vec2(posX, posY); } // returns the position of the player
	void Moving(Tmpl8::vec2 startPos, float deltatime); //controls player movement
	int currentframe(float time); // checks which frame to use

	void addCollisions(std::vector <HBox> collisionObject); // checks for collisions with the hitboxes
	bool isOverlapping(const HBox& boxb); // checks if the player is overlapping with the hitbox
	void resolveVerticalCollision(const HBox& box, float yOverlap); // resolves the collision with the hitbox
	void resolveHorizontalCollision(const HBox& box, float xOverlap); // resolves the collision with the hitbox
	void Draw(Tmpl8::Surface* surface, Sprites playersprite, bool show);

private:  
	//initializes player variables  
	HBox playerSize;

	float posX = 0, posY = 0, xvelocity = 0, yvelocity = 0, epsi = 0.1, 
	movementSpeed = 8.5f, fricspeed = 7.0f, airresist = 4.0f, jumpSpeed = 400.0f, gravspeed = 10.0f;
	bool onGround = false;
	int currentFrame = 12, filler = 1;

	void Accelerate(float deltatime);
	void Friction(float deltatime, bool moving);
	void Gravity(float deltatime);
	void Jump(float deltatime);
	void Boundries(Tmpl8::vec2 startPos);
	void ClampVelocity();
};  
}
