#pragma once

#include "../include/DCT vector2.h"



DCT_vector2_i DCT_vector::convert(const DCT_vector2& rhs)
{return DCT_vector2_i(static_cast<int>(rhs.x), static_cast<int>(rhs.y));
};
DCT_vector2_i DCT_vector::convert(DCT_vector2&& rhs)
{return DCT_vector2_i(static_cast<int>(rhs.x), static_cast<int>(rhs.y));
};

DCT_vector2 DCT_vector::convert(const DCT_vector2_i& rhs)
{return DCT_vector2(static_cast<float>(rhs.x), static_cast<float>(rhs.y));
};
DCT_vector2 DCT_vector::convert(DCT_vector2_i&& rhs)
{return DCT_vector2(static_cast<float>(rhs.x), static_cast<float>(rhs.y));
};

