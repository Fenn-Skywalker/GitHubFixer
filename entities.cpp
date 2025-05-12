#include "entities.h"
#include "sprites.h"
#include "game.h"
#include <LDtkLoader/Project.hpp>
#include <vector>
#include <memory>

Hitbox::Hitbox(const HBox& hitbox)
	: hitbox(hitbox)
{

}

void Hitbox::Draw(Tmpl8::Surface* surface, const Tmpl8::vec2& pos) // drawing the hitbox
{
	surface->Box(hitbox.x + static_cast<int>(pos.x), hitbox.y + static_cast<int>(pos.y), hitbox.x + hitbox.w + static_cast<int>(pos.x), hitbox.y + hitbox.h + static_cast<int>(pos.y), 0xff0000);

}

std::vector <bool> check(1000); // for checking if the key is collected
std::vector <bool> check2(1000); // for checking if the key is in its original position
std::vector <float> Ycurrent(1000); // for checking the Y position of the key

void Key::DrawKeys(Tmpl8::Surface* surface, std::vector<HBox> collisionObject, Tmpl8::vec2 playerPosition, HBox playersize, Sprites sprite)
{  
   int i = 0; // index for the key  
   totalkeys = collisionObject.size(); // setting the amount of keys to the amount of keys in the level
   for (auto& box : collisionObject) // repeat for the amount of keys  
   {  
       if (check2[i] == false)  
       {  
           Ycurrent[i] = static_cast<float>(box.y); // setting Ycurrent to the start pos of the key/egg  
           check2[i] = true;  
       }  

       if (playerPosition.y + playersize.h - 0.2f > box.y && // checking if the player is in the hitbox  
           playerPosition.y < box.y + box.h &&  
           playerPosition.x + playersize.w > box.x &&  
           playerPosition.x < box.x + box.w  
           && check[i] == false)  
       {  
           keys++; // adding a key  
           check[i] = true; // setting the key to collected  
       }  
       else if (check[i] == false)  
       {  
           sprite.Draw(surface, Tmpl8::vec2(static_cast<float>(box.x), static_cast<float>(Ycurrent[i]) + 2.0f)); // drawing the coin/egg
       }  
       i++;  
   }  
}

void Key::resetCheck() // resetting the check variables for a new level
{
	for (int i = 0; i < check.size(); i++)
	{
		check[i] = false;
	}
	for (int i = 0; i < check2.size(); i++)
	{
		check2[i] = false;
	}

}

bool Door::touchDoor(Tmpl8::Surface* surface, std::vector <HBox> doorBox, HBox playerSize, float x, float y) // drawing the door
{
	//surface->Box(door.x, door.y, door.x + door.w, door.y + door.h, 0xff00ff);
	HBox door = doorBox[0]; // getting the door from the vector

	if (x + playerSize.w > door.x && x < door.x + door.w && y < door.y + door.h && y + playerSize.h > door.y) // checking if player is inside door
	{
		return true; // setting the door to open
	}
	else
	{
		return false; // setting the door to closed
	}

}

void Door::DrawDoor(Tmpl8::Surface* surface, Sprites doorsprite, std::vector <HBox> doorBox)
{
	HBox door = doorBox[0];
    doorsprite.Draw(surface, Tmpl8::vec2(door.x, door.y));
}

void Text::DrawCurrentText(Tmpl8::Surface* surface, Sprites textsprite, std::vector <HBox> textBox)
{
	HBox door = textBox[0];
	textsprite.Draw(surface, Tmpl8::vec2(door.x, door.y));
}