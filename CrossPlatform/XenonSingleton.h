#ifndef SAINTENGINEROOT_H
#define SAINTENGINEROOT_H

#include <assert.h>
namespace CrossPlatform {


	template<typename T>
	class XenonSingleton
	{
	public:
		//ScaffoldSingleton() 
		XenonSingleton(const XenonSingleton&) = delete;
		void operator=(const XenonSingleton&) = delete;

		virtual ~XenonSingleton() = default;

		//Get singleton class, have and the only have one instance;
		static inline T& Get() {

			static T* gpSingleton = nullptr;
			
			if (gpSingleton == nullptr)
			{
				gpSingleton = new T;
			}
			
			assert(gpSingleton);

			return *gpSingleton;
		}
	protected:
		XenonSingleton() = default;
	};

}

#endif // SAINTENGINEROOT_H