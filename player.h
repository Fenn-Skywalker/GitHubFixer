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
	void Moving(Tmpl8::vec2 startPos); //controls player movement
	int currentframe(); // checks which frame to use

	void addCollisions(std::vector <HBox> collisionObject); // checks for collisions with the hitboxes
	bool isOverlapping(const HBox& boxb); // checks if the player is overlapping with the hitbox
	void resolveVerticalCollision(const HBox& box, float yOverlap); // resolves the collision with the hitbox
	void resolveHorizontalCollision(const HBox& box, float xOverlap); // resolves the collision with the hitbox
	void Draw(Tmpl8::Surface* surface, Sprites playersprite, bool show);

private:  
	//initializes player variables  
	HBox playerSize;

	float posX, posY, xvelocity, yvelocity, epsi = 0.1;
	bool onGround = false;
	int currentFrame = 12, filler = 1;

	void Accelerate();
	void Friction();
	void Gravity();
	void Jump();
	void Boundries(Tmpl8::vec2 startPos);
	void ClampVelocity();
};  
}
