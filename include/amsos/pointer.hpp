#ifndef AMSOS_POINTER_HPP
#define AMSOS_POINTER_HPP

#include <stdint.h>

#define _packed __attribute__ ((packed))

namespace kern
{

typedef uint32_t ptr_t;

template<typename T>	
class pointer
{
	T* ptr;
public:
	pointer()
	{
		ptr = nullptr;
	}

	pointer(T* p)
	{
		ptr = p;
	}
	
	pointer(ptr_t p)
	{
		ptr = reinterpret_cast<T*>(p);
	}

	template<typename T1>
	T1* cast_to()
	{
		return reinterpret_cast<T1*>(ptr);
	}
	
	T* get()
	{
		return ptr;
	}
	
	T* get(ptr_t p)
	{
		return ptr + p;
	}
	
	void set(ptr_t p)
	{
		ptr = reinterpret_cast<T*>(p);
	}
	
	ptr_t get_location()
	{
		return reinterpret_cast<ptr_t>(ptr); 
	}
	
	template<typename T1>
	pointer& operator=(const pointer<T1>& r)
	{
		ptr = r.cast_to<T>();
		return *this;
	}
	
	pointer& operator=(const ptr_t r)
	{
		ptr = reinterpret_cast<T*>(r);
		return *this;
	}
	
	explicit operator ptr_t()
	{
		return cast_to<ptr_t>();
	}
};

}
#endif