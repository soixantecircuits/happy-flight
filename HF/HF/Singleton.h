#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <iostream>

template <typename T>
class Singleton
{
protected:
	Singleton (){}
	~Singleton (){}

public:
	static T *GetInstance ()
	{
		if (NULL == m_pInstance)
		{
			std::cout << "creating singleton." << std::endl;
			m_pInstance = new T;
		}

		return (static_cast<T*> (m_pInstance));
	}

	static void Delete ()
	{
	if (NULL != m_pInstance)
		{
		delete m_pInstance;
		m_pInstance = NULL;
		}
	}

private:
	// Unique instance
	static T *m_pInstance;
};

template <typename T>
T *Singleton<T>::m_pInstance = NULL;

#endif //__SINGLETON_H__