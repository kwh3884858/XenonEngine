#ifndef SAINTENGINEROOT_H
#define SAINTENGINEROOT_H

#include <assert.h>
namespace CrossPlatform {


	template<typename T>
	class ScaffoldSingleton
	{
	public:
		//ScaffoldSingleton() 
		ScaffoldSingleton(const ScaffoldSingleton&) = delete;
		void operator=(const ScaffoldSingleton&) = delete;

		virtual ~ScaffoldSingleton() = default;

		//initilize function, take the place of constructor
		virtual bool initialize() = 0;

		//destroy function, take the  place of deconstructor
		virtual bool shutdown() = 0;

		//Get singleton class, have and the only have one instance;
		static inline T& get() {

			static T* gpSingleton = nullptr;
			
			if (gpSingleton == nullptr)
			{
				gpSingleton = new T;
			}
			
			assert(gpSingleton);

			return *gpSingleton;
		}
	protected:
		ScaffoldSingleton() = default;
	};

}

#endif // SAINTENGINEROOT_H