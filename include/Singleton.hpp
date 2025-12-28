#pragma once

#include <mutex>

template <typename T>
class Singleton {
protected:
    Singleton() = default;
private:
    Singleton(const Singleton<T>&) = delete;
    Singleton<T>& operator= (const Singleton<T>&) = delete;
    inline static T* instance = nullptr;

public:
    ~Singleton() = default;
    template<typename... Args>
    static T* getInstance(Args&&... args) {
        static std::once_flag flag;
        std::call_once(flag, [&] {
            instance = new T(std::forward<Args>(args)...);
        });
        return instance;
    }

    static void destoryInstance() {
        delete instance;
    }
};
