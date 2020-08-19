#include "Collision.h"
#include "Map.h"
Col::Col()
{
}

Col::~Col()
{
}

int Col::collidingWith(int currentY, int currentX, int moveToY, int moveToX, Map& map)
{
	return (map.getGrid( (currentY + (moveToY)),(currentX + (moveToX)) ));
}
