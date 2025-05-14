#include "surface.h"
#include "template.h"
#include "player.h"
#include "sprites.h"
#include "game.h"
#include "layers.h"

#include <iostream>
#include <LDtkLoader/Project.hpp>
#include <vector>
#include <memory>
#include <Windows.h>
#include <SDL.h>
#include <algorithm>
#include <cstdio> //printf


namespace Tmpl8
{

	//Player Constructor
	Player::Player(const HBox& playerSize, float xvelocity, float yvelocity) // constructor
		: playerSize(playerSize), xvelocity(xvelocity), yvelocity(yvelocity)
	{
		posX = static_cast<float>(playerSize.x);
		posY = static_cast<float>(playerSize.y);
	}
	
	//handles the acceleration/increasing of velocity
	void Player::Accelerate(float deltatime)
	{
		//compute deltas
		float dx = Clamp(xvelocity, -5.0f, 5.0f);
		posX += dx;
	}

	//reduces velocity when in contact with ground at a set rate for more organic movement
	void Player::Friction(float deltatime, bool moving)
	{
		if (onGround == true) // if the player is on the ground
		{
			if (xvelocity > 0)
			{
				xvelocity -= fricspeed * deltatime;
			}
			if (xvelocity < 0)
			{
				xvelocity += fricspeed * deltatime;
			}
		}
		else // if the player is in the air
		{
			if (xvelocity > 0)
			{
				xvelocity -= airresist * deltatime;
			}
			if (xvelocity < 0)
			{
				xvelocity += airresist * deltatime;
			}
		}

		if (xvelocity != 0 && xvelocity < 0.05f && xvelocity > -0.05f && moving == false) { xvelocity = 0; }
	}

	//controls vertical acceleration and velocity/checks floor collision
	void Player::Gravity(float deltatime)
	{
		
			yvelocity += gravspeed * deltatime ;
			posY += yvelocity;
	}

	//ensures player stays within the screen bounds
	void Player::Boundries(Tmpl8::vec2 startPos)
	{
		if (posY > ScreenHeight - 16)
		{
			posY = startPos.y;
			posX = startPos.x;
			yvelocity = 0.0f;
			xvelocity = 0.0f;
		}
		if (posX <= 0) { posX = 0 , xvelocity = 0; }
		if (posX >= ScreenWidth - 16) { posX = ScreenWidth - 16, xvelocity = 0; }
		if (posY < 0) { posY = 0, yvelocity = 0; }
	}
	void Player::ClampVelocity() //sets certain velocity values to a set range
	{
		if (xvelocity > 5.0f) { xvelocity = 5.0f; }
		if (xvelocity < -5.0f) { xvelocity = -5.0f; }
		if (yvelocity > 5.0f) { yvelocity = 5.0f; }
		if (yvelocity < -5.0f) { yvelocity = -5.0f; }
	}

	//sets y velocity to a negative value to simulate jumping
	void Player::Jump(float deltatime)
	{
		onGround = false;
		yvelocity -= jumpSpeed * deltatime;
		posY += yvelocity;
	}


	// Helper to check AABB overlap
	// Check AABB overlap between player and a box
	bool Player::isOverlapping(const HBox& box) 
	{
		return (posX + playerSize.w >= box.x && posX <= box.x + box.w &&
			posY + playerSize.h >= box.y && posY <= box.y + box.h);
	}

	// Resolve vertical collisions, returns true if landed
	void Player::resolveVerticalCollision(const HBox& box, float yOverlap) 
	{
		if (posY < box.y)
		{
			posY -= yOverlap;
			onGround = true; // Player landed on top
		}
		else
		{
			posY += yOverlap; // Player hit the ceiling
		}

		yvelocity = 0.0f;
	}

	// Resolve horizontal collisions
	void Player::resolveHorizontalCollision(const HBox& box, float xOverlap) 
	{
		if (posX < box.x)
		{
			posX -= xOverlap; // Player hit the left side
		}
		else
		{
			posX += xOverlap; // Player hit the right side
		}
		xvelocity = 0.0f;
	}

	// Main collision method, method was originally self taught/made based on the https://github.com/jobtalle/PlatformerPhysics 
	// and when collision issues occured due to modifying code based around ldtk and I couldn't figure out what the issue was it was further resolved through AI
	// fine tuning corner collisions for vertical collisions when the player lands must still be implemented
	void Player::addCollisions(std::vector<HBox> collisionObject)
	{
		onGround = false; // Reset at start of frame
		for (const auto& box : collisionObject)
		{
			if (isOverlapping(box))
			{
				float xOverlap = Tmpl8::Min(posX + playerSize.w, static_cast<float>(box.x + box.w)) -
					Tmpl8::Max(posX, static_cast<float>(box.x));
				float yOverlap = Tmpl8::Min(posY + playerSize.h, static_cast<float>(box.y + box.h)) -
					Tmpl8::Max(posY, static_cast<float>(box.y));

				if (xOverlap < yOverlap)
				{
					//something has to be introduced to handle falling collisions better so that when the player lands on a corner it doesnt get thrown to the side
					resolveHorizontalCollision(box, xOverlap);
				}
				else
				{
					resolveVerticalCollision(box, yOverlap);
				}
			}
		}
	}

	int Player::currentframe(float time) // checking which frame to use
	{
		if (onGround)
		{
			if (xvelocity > 0)
			{
				currentFrame = 12;
			}
			else if (xvelocity < 0) 
			{
				currentFrame = 8;
			}
			else
			{
				currentFrame = 0;
			}
		}
		else
		{
			currentFrame = 31;
		}
		
		return currentFrame;
	}

	void Player::Moving(Tmpl8::vec2 startPos, float deltatime)
	{
		bool moving;
		Gravity(deltatime);
		Boundries(startPos);
		if (GetAsyncKeyState(VK_LEFT)) { moving = true, xvelocity -= movementSpeed * deltatime; }
		else if (GetAsyncKeyState(VK_RIGHT)) { moving = true, xvelocity += movementSpeed * deltatime; }
		else { moving = false; }
		if (GetAsyncKeyState(VK_UP) && onGround == true) { Jump(deltatime); }
		ClampVelocity();
		Accelerate(deltatime);
		Friction(deltatime, moving);
	}

	void Player::Draw(Tmpl8::Surface* surface, Sprites playersprite, bool show) // drawing the player
	{
		if (show)
		{
			surface->Box(static_cast<int>(posX), static_cast<int>(posY), playerSize.w + static_cast<int>(posX), playerSize.h + static_cast<int>(posY), 0x000000);// draw the hitbox
		}
		playersprite.Draw(surface, Tmpl8::vec2(posX, posY)); // draw the player sprite
	}
};