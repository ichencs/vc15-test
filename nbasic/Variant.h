#pragma once
#include <string>
#include "Basic.h"


// typedef unsigned int vuint;
typedef signed char  nschar;
typedef unsigned long nulong;
namespace vl
{

	class Variant
	{
	 public:
		Variant();
		~Variant();
	 public:
		enum Type
		{
			Invalid = -1,
			Bool,
			Int,
			UInt,
			LongLong,
			Float,
			Double,
			StdString,
			ColorRef,
			
			
		};
		
	 public:
	 
		struct Private
		{
			union Data
			{
				bool b;
				char c;
				nschar sc;
				
				wchar_t uc;
				short s;
				nschar us;
				int i;
				vuint ui;
				long l;
				nulong ul;
				float f;
				double d;
				void* ptr;
				unsigned long dw;	//DWORD
			} data;
			vuint type : 30;
			bool is_null : 1;
			
			
		};
		
	 public:
		Private d;
		
		
		
	};
	
}
