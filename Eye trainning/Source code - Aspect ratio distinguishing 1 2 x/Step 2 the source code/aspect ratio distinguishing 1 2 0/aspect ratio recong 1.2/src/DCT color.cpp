#include "../include/DCT color.h"
#include <algorithm>

//DCT_color_rgba_f DCT_color::DCT_color_tran(const DCT_color_rgba_uchar& in)
//{return DCT_color_rgba_f(in.r / 255.f, in.g / 255.f, in.b / 255.f, in.a / 255.f);}

DCT_color_rgba_uchar DCT_color::DCT_color_tran(const DCT_color_rgba_f& in)
{
	return DCT_color_rgba_uchar(
		static_cast<unsigned char>(in.r * 255.f),
		static_cast<unsigned char>(in.g * 255.f),
		static_cast<unsigned char>(in.b * 255.f),
		static_cast<unsigned char>(in.a * 255.f));
}
DCT_color_rgba_uchar DCT_color::DCT_color_tran_safe(const DCT_color_rgba_f & in)
{
	int r = static_cast<int>(in.r * 255.f);
	r = std::max(0, std::min(255, r));
	int g = static_cast<int>(in.g * 255.f);
	g = std::max(0, std::min(255, g));
	int b = static_cast<int>(in.b * 255.f);
	b = std::max(0, std::min(255, b));
	int a = static_cast<int>(in.a * 255.f);
	a = std::max(0, std::min(255, a));

	return DCT_color_rgba_uchar(
		static_cast<unsigned char>(r),
		static_cast<unsigned char>(g),
		static_cast<unsigned char>(b),
		static_cast<unsigned char>(a));
}

DCT_color_rgb_uchar DCT_color::DCT_color_tran_float4_to_uchar3(const DCT_color_rgba_f & in)
{
	return DCT_color_rgb_uchar(
		static_cast<unsigned char>(in.r * 255.f),
		static_cast<unsigned char>(in.g * 255.f),
		static_cast<unsigned char>(in.b * 255.f));
}
//DCT_color_rgb_uchar DCT_color::DCT_color_tran_safe(const DCT_color_rgb_f & in)
//{
//	int r = in.r * 255.f;
//	r = std::max(0, std::min(255, r));
//	int g = in.g * 255.f;
//	g = std::max(0, std::min(255, g));
//	int b = in.b * 255.f;
//	b = std::max(0, std::min(255, b));
//
//	return DCT_color_rgb_uchar(
//		static_cast<unsigned char>(r),
//		static_cast<unsigned char>(g),
//		static_cast<unsigned char>(b));
//}