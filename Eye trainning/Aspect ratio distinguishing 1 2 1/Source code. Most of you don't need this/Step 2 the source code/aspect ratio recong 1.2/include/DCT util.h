/*
Random functions provide 2 ways of invoking, global functions and static functions inside a class.
For the global functions, their names are all DCT_random*. (* represents some characters)
But I personally prefer to gather them inside a class which works like a namespace. The functions should be invoked like DCT_rand::*. In this way, the automatically completion feature of compile helps.
*/
#pragma once 

#include <cstdlib>
#include <algorithm>
#include <tuple>


template<class _Ty1,
	class _Ty2>
	struct std::pair;

template<class _Ty>
class std::allocator;
template<class _Ty, class _Alloc = std::allocator<_Ty> >
class std::vector;

//prefix declearation inside this file.
class DCT_rand;




float DCT_random01();
float DCT_random(float min, float max);

// The max param is possible to be in results.
int DCT_randomi(int min, int max_included);

//check the return value! If the return value==false, the param 2 "result"
//this function doesn't compile. I don't know the reason. The type of return value is already defined inside the class below and the presix declaration is definitely correct. Don't know the reason. So, let's comment it for now.
//template<typename T>DCT_rand::error_type DCT_rand_choose(const std::vector<T>& src, T* p_result);

//template ver of randomly choose. Notice, manually ensure all the type, manually match all the type. NEVER rely on the auto type devivation!!!
//But, I failed again. The type derivation just doesn't work no matter how much info I provide. Great, I'm not gonna return any pair or tuple. Let's comment this as well.
/*
template<typename T>
std::tuple<bool, T> DCT_rand_choose(const std::vector<T>& src);
*/





class DCT_rand
{
public:
	//simply invoke the global functions for the 
	static float rand01();
	static float rand(float min, float max);
	int rand_i(int min, int max_included);

	//some complex tool.
	enum class error_type
	{
		OK,

		source_is_empty,
		pointer_for_result_is_not_available
	};

	//check the return value! If the return value==false, the param 2 "result"
	template<typename T>
	static DCT_rand::error_type rd_choose(const std::vector<T>& src, T* p_result);




};



//总觉得不太对
//template<typename T>
//const T& clamp_ref(T& v, const T& lo, const T& hi);

