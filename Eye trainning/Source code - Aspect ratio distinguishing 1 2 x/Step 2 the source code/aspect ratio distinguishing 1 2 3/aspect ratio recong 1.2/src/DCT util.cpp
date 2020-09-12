#include <cstdlib>
#include <algorithm>
#include <utility>
#include <vector>

#include "../include/DCT util.h"


float DCT_random01()
{
	// 0 <= result <= RAND_MAX(which is 0x7fff in my compiler.)
	return static_cast<float>(std::rand()) / RAND_MAX;

	// for debug.
	//int r = std::rand();
	//float f = static_cast<float>(r);
	//float f2 = f / RAND_MAX;
	//return f2;
}
inline float DCT_random(float min, float max)
{
	if (max < min) std::swap(max, min);
	return DCT_random01()*(max - min) + min;
}
inline int DCT_randomi(int min, int max_included)
{
	if (max_included < min) std::swap(max_included, min);
	return (std::rand() % (max_included - min + 1))+min;
}

/*头文件那边无法编译，不知道原因。先不用。
template<typename T>
inline DCT_rand::error_type DCT_rand_choose(const std::vector<T>& src, T * p_result)
{
	if (!p_result)return DCT_rand::error_type::pointer_for_result_is_not_available;
	unsigned int element_count = src.size();
	if (0 == element_count)return DCT_rand::error_type::source_is_empty;

	unsigned int index = DCT_random(0, element_count-0.000001f);
	p_result = &(src[index]);

	return DCT_rand::error_type::OK;
}
*/

//这个的类型推断，无论如何都无法正常找到特化版本。实在是不明白。应该不是我的问题。
//template<typename T>
//std::tuple<bool, T> DCT_rand_choose(const std::vector<T>& src)
//{
//	unsigned int element_count= src.size();
//	if (0 == element_count)return { false,src.end() };
//
//	unsigned int index = DCT_random(0, element_count);
//	if (element_count == index)index -= 1;
//	return std::tuple<bool, T>(true, src[index]);
//}



















//inside the class, all ***STATIC***.
float DCT_rand::rand01(){return DCT_random01();}
float DCT_rand::rand(float min, float max){return DCT_random(min, max);}
int DCT_rand::rand_i(int min, int max_included) { return DCT_randomi(min, max_included); }




//
//
//template<typename T>
//std::tuple<bool, typename std::vector<T>::iterator> DCT_rand_choose(const std::vector<T>& src)
//{
//	unsigned int element_count= src.size();
//	if (0 == element_count)return { false,src.end() };
//
//	unsigned int index = DCT_random(0, element_count);
//	if (element_count == index)index -= 1;
//	return std::tuple<bool, typename std::vector<T>::iterator>(true, src.begin() + index);
//}


int DCT_round2i(float src)
{
	return static_cast<int>(src+0.5f);
}