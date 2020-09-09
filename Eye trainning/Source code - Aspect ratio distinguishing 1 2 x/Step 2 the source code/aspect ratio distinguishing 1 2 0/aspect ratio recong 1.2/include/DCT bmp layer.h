/*
This is the bitmap layer tool of DCT(Dirac cpp tool - bitmap).
search for 
void how_to_inside();暂时没写。。 to do
for help.


这个节点的功能是，当需要在某些格式当中进行转换的时候，比如从rgba uchar 转换到rgb uchar。这个容器自动处理。
只处理有意义的。现在就只有rgba到rgb的转换。只保留一个内存。也就是说暂时没有撤销的功能。




to do :


Hope you like this library.

To get the latest:
Github.com/yagaodirac/Dirac-cpp-tool
*/

#pragma once

template<class T>
void swap(T& a, T& b);

//yeah, this is how I avoid including too many headers. github.com/yagaodirac/cpp-code-snipet-set for the tool.
template<class _Ty>
class std::allocator;
template<class _Ty, class _Alloc = std::allocator<_Ty> >
class std::vector;

template<class _Elem>
struct std::char_traits;
template<class _Ty>
class std::allocator;
template<class _Elem,
	class _Traits = std::char_traits<_Elem>,
	class _Alloc = std::allocator<_Elem>>
	class std::basic_string;
using string = std::basic_string<char, std::char_traits<char>, std::allocator<char>>;





//#include "simple_mem_to_png.h"
//#include "../libpng code/png.h"



#include "DCT bmp layer memory.h"






class DCT_bmp_layer_rgba : public DCT_component
{
public:
	DCT_bmp_layer_memory_uchar_x4* memory;
	
	//ctor & dtor
	DCT_bmp_layer_rgba() { memory = new DCT_bmp_layer_memory_uchar_x4();}
	~DCT_bmp_layer_rgba() {delete memory;}
};

//这个文件以后可能会很长。但是现在就只有这个样子