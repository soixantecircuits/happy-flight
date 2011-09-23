#ifndef __SINGLETON_H__
#define __SINGLETON_H__

#include <iostream>

template <typename T>
class Singleton
{
protected:
  Singleton () { }
  ~Singleton () { delete instance; }

public:
  static T *GetInstance ()
  {
    if (NULL == instance)
      {
        std::cout << "creating singleton." << std::endl;
        instance = new T;
      }

    return (static_cast<T*> (instance));
  }

  static void Delete ()
  {
    if (NULL != instance)
      {
        delete instance;
        instance = NULL;
      }
  }

private:
  // Unique instance
  static T *instance;
};

template <typename T>
T *Singleton<T>::instance = NULL;

#endif //__SINGLETON_H__