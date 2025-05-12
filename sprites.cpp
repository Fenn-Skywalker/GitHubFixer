#include "Sprites.h"

#include <cassert>
#include <string>

Sprites::Sprites(std::shared_ptr<Tmpl8::Surface> image, const HBox& hbox) //constructor
	: image(image), hbox(hbox) //initialize the variables
{
}

void Sprites::Draw(Tmpl8::Surface* surface, const Tmpl8::vec2& pos) //draw the sprite
{
	for (int y = 0; y < hbox.h; y++) //loop through the height of the sprite
	{
		for (int x = 0; x < hbox.w; x++) //loop through the width of the sprite
		{
			Tmpl8::Pixel color = image->GetBuffer()[(x + hbox.x) + (y + hbox.y) * image->GetPitch()]; //get the color of the pixel
			if (color >> 24 > 0) // shifting 24 bits to the right to get to the alfa channel 
				surface->Plot(x + static_cast<int>(pos.x), y + static_cast<int>(pos.y), image->GetBuffer()[(x + hbox.x) + (y + hbox.y) * image->GetPitch()]); //draw the sprite
		}
	}
}

SpriteSheet::SpriteSheet(const std::string& path, int tilesize) //constructor
{
	image = std::make_shared<Tmpl8::Surface>(path.c_str()); //load the image
	columns = image->GetWidth() / tilesize; //number of tiles in x 
	rows = image->GetHeight() / tilesize; //number of tiles in y
	for (int y = 0; y < rows; y++) //loop through the tiles in y
	{
		for (int x = 0; x < columns; x++) //loop through the tiles in x
		{
			HBox hbox; //create a rectangle
			hbox.x = x * tilesize; //set the x coordinate
			hbox.y = y * tilesize; //set the y coordinate
			hbox.w = tilesize; //set the width
			hbox.h = tilesize; //set the height
			sprites.emplace_back(image, hbox); //add the sprite to the vector
		}
	}
}

SpriteSheet::SpriteSheet(const std::string& path, int VTileSize, int HTileSize) //constructor
{
	image = std::make_shared<Tmpl8::Surface>(path.c_str()); //load the image
	columns = image->GetWidth() / HTileSize; //number of tiles in x 
	rows = image->GetHeight() / VTileSize; //number of tiles in y
	for (int y = 0; y < rows; y++) //loop through the tiles in y
	{
		for (int x = 0; x < columns; x++) //loop through the tiles in x
		{
			HBox hbox; //create a rectangle
			hbox.x = x; //set the x coordinate
			hbox.y = y; //set the y coordinate
			hbox.w = HTileSize; //set the width
			hbox.h = VTileSize; //set the height
			sprites.emplace_back(image, hbox); //add the sprite to the vector
		}
	}
}



const Sprites& SpriteSheet::GetSprite(int TileNumber) //get the sprite
{
	assert(TileNumber < sprites.size()); //assert that the tile number is within the bounds
	return sprites[TileNumber]; //return the sprite
}

