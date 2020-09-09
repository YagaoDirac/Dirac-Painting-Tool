#pragma once

#include "../include/DCT vector2.h"

DCT_vector2_i::DCT_vector2_i() { x = y = 0; };
DCT_vector2_i::DCT_vector2_i(int in_x, int in_y){ x = in_x; y = in_y; };
DCT_vector2_i::DCT_vector2_i(float in_x, float in_y)
{x = static_cast<int>(in_x); y = static_cast<int>(in_y);}

DCT_vector2_i::DCT_vector2_i(const DCT_vector2_i& src) { x = src.x; y = src.y; };
DCT_vector2_i::DCT_vector2_i(DCT_vector2_i&& src) { x = src.x; y = src.y; };
DCT_vector2_i& DCT_vector2_i::operator=(const DCT_vector2_i&  rhs) 
{ x = rhs.x; y = rhs.y; return *this; };
DCT_vector2_i& DCT_vector2_i::operator=(DCT_vector2_i&&  rhs) 
{ x = rhs.x; y = rhs.y; return *this; };
DCT_vector2_i::~DCT_vector2_i() {};//not very sure whether this one has to be virtual.


DCT_vector2_i DCT_vector2_i::operator+(int rhs)
{return DCT_vector2_i(x + rhs, y + rhs);}
DCT_vector2_i DCT_vector2_i::operator+(const DCT_vector2_i& rhs)
{return DCT_vector2_i(x + rhs.x, y + rhs.y);}
DCT_vector2_i DCT_vector2_i::operator-(int rhs)
{return DCT_vector2_i(x - rhs, y - rhs);}
DCT_vector2_i DCT_vector2_i::operator-(const DCT_vector2_i& rhs)
{return DCT_vector2_i(x - rhs.x, y - rhs.y);}



