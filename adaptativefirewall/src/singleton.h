#ifndef SINGLETON_H
#define SINGLETON_H

#include <string>

template <typename T>
class Singleton
{
public:
  template <typename... Args>
  static T* getInstance(Args... args)
  {
    if(!_instance)
    {
      _instance = new T(std::forward<Args>(args)...);
    }
    return _instance;
  }

  static void destroyInstance()
  {
    delete _instance;
    _instance = nullptr;
  }

private:
  static T* _instance;
};

template <class T> T* Singleton<T>::_instance = nullptr;

#endif
