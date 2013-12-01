#ifndef SINGLETON_H
#define SINGLETON_H

template <typename T>
class Singleton {
protected:
	static T& instance() {
		static T object;
		return object;
	}
};

#endif
