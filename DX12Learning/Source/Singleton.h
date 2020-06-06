#pragma once

namespace Alrescha {

	template<class T>
	class Singleton {
	public:
		static T& GetInstance(void)
		{
			static T instance;
			return instance;
		}

	private:
		Singleton(const Singleton& aAnother) = delete;
		void operator =(const Singleton& aAnother) = delete;

	protected:
		Singleton(void) {}
		virtual ~Singleton(void) {}
	};
}