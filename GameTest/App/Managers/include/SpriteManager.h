#pragma once
#include "../App/Utilities/include/App.h"
#include "../App/Utilities/include/Helper.h"
#include <memory>
#include <string>
using namespace std;

class SpriteManager {
public:
	static shared_ptr<CSimpleSprite> CreateSprite(const string& path, int columns, int rows);

};