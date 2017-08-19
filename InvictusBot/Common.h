#pragma once

//math durp
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>
#include <cstdlib>
// STL/Bwapi stuff that's useful across files
#include <BWAPI.h>
#include <vector>
#include <string>
#include<map>
#include<array>
#include <stdexcept>
#include<cassert>
#include<iostream>
#include<set>
#include<utility>

#define InvictusBot

class Rect {
	int width;
	int length;
	Rect(int width,int length);
};

