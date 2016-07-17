#ifndef AMSOS_STACK_HPP
#define AMSOS_STACK_HPP

#include <amsos/pointer.hpp>

namespace kern
{

// A very dangerous stack class but is meant to serve as a temporary memory manager
class stack
{
	pointer<void> _top;
	pointer<void> _bottom;
	ptr_t _size;
	
	ptr_t get_available_space()
	{
		ptr_t t = _top.get_location();
		ptr_t b = _bottom.get_location();
		return _size - b - t;
	}
	
public:
	stack()
	{
		_top = 0;
		_bottom = 0;
		_size = 0;
	}
	
	void set_memory(ptr_t ptr, ptr_t size)
	{
		_bottom = ptr + size + 1;
		_top = _bottom;
		_size = _size;
	}
	
	template<typename T>
	T* push()
	{
		const ptr_t s = sizeof(T);
		if (get_available_space() < s)
			return nullptr;
		_top = _top.get_location() - s;
		return _top.cast_to<T>();
	}
	
	template<class T>
	T* emplace()
	{
		T* retptr = push<T>();
		if (!retptr) return nullptr;
		*retptr = T();
		return retptr;
	}
	
	template<typename T>
	int pop()
	{
		const ptr_t s = sizeof(T);
		if (get_available_space() + s > _size)
			return 1;
		_top = _top.get_location() + s;
		return 0;
	}
	
	template<typename T>
	T* top()
	{
		return _top.cast_to<T>();
	}
};

}

#endif