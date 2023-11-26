#include "stdafx.h"
#include "../include/Helper.h"

glm::vec3 Helper::GetOppositeQuadrantPosition(const glm::vec3& playerPos, float screenWidth, float screenHeight)
{
	glm::vec3 enemyPos;
	if (playerPos.x < screenWidth / 2) {
		enemyPos.x = screenWidth * 0.75f; // Spawn in the right half
	}
	else {
		enemyPos.x = screenWidth * 0.25f; // Spawn in the left half
	}
	if (playerPos.y < screenHeight / 2) {
		enemyPos.y = screenHeight * 0.75f; // Spawn in the lower half
	}
	else {
		enemyPos.y = screenHeight * 0.25f; // Spawn in the upper half
	}
	return enemyPos;
}

SpriteDimensions Helper::GetSpriteDimensions(shared_ptr<CSimpleSprite> sprite, float multiplier)
{
	SpriteDimensions dimensions;
	if (sprite) {
		dimensions.width = sprite->GetWidth();
		dimensions.height = sprite->GetHeight();
		dimensions.adjustedWidth = dimensions.width * multiplier;
		dimensions.adjustedHeight = dimensions.height * multiplier;
	}
	else {
		dimensions.width = 0;
		dimensions.height = 0;
		dimensions.adjustedWidth = 0;
		dimensions.adjustedHeight = 0;
	}
	return dimensions;
}
