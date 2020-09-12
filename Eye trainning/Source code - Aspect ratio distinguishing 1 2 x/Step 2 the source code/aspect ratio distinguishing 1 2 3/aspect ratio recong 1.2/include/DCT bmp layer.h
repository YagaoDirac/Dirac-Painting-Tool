/*
This is the bitmap layer tool of DCT(Dirac cpp tool - bitmap).
search for 
void how_to_inside();��ʱûд���� to do
for help.


����ڵ�Ĺ����ǣ�����Ҫ��ĳЩ��ʽ���н���ת����ʱ�򣬱����rgba uchar ת����rgb uchar����������Զ�����
ֻ����������ġ����ھ�ֻ��rgba��rgb��ת����ֻ����һ���ڴ档Ҳ����˵��ʱû�г����Ĺ��ܡ�




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

//����ļ��Ժ���ܻ�ܳ����������ھ�ֻ���������