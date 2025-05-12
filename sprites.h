#pragma once

#include "template.h"
#include "surface.h"

#include <memory>
#include <vector>
#include <string>


struct HBox // Rectangle structure for the hitboxes
{
	int x, y, w, h; // x and y coordinates, width and height
};

class Sprites
{
public:
	Sprites() = default;
	~Sprites() = default;
	Sprites(std::shared_ptr<Tmpl8::Surface> image, const HBox& hbox); // Constructor.

	//Draw the sprite
	void Draw(Tmpl8::Surface* surface, const Tmpl8::vec2& pos);
	int GetWidth() const { return hbox.w; } // Get the width of the sprite.
	int GetHeight() const { return hbox.h; } // Get the height of the sprite.

private:
	std::shared_ptr<Tmpl8::Surface> image; // Image of sprite.
	HBox hbox; // Coordinates of sprite in image.
};

class SpriteSheet { // SpriteSheet class.

public:
	SpriteSheet() = default; // Constructor.
	SpriteSheet(const std::string& path, int tilesize); // Constructor.
	SpriteSheet(const std::string& path, int VTileSize, int HTileSize); // Constructor.
	~SpriteSheet() = default; // Destructor.

	const Sprites& GetSprite(int TileNumber);

private:
	std::shared_ptr<Tmpl8::Surface> image; // Image of sprite.
	std::vector<Sprites> sprites; // Sprites.
	int columns = 0; // Number of columns.
	int rows = 0; // Number of rows.
};


