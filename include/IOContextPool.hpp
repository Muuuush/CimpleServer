#pragma once

#include "Singleton.hpp"
#include <boost/asio.hpp>

class IOContextPool : public Singleton<IOContextPool>{
    friend Singleton<IOContextPool>;
public:
    IOContextPool(int size = std::thread::hardware_concurrency());
    boost::asio::io_context& getIOContext();
    ~IOContextPool();

private:
    int nextIndex;
    std::vector<boost::asio::io_context> ioContexts;
    std::vector<boost::asio::executor_work_guard<boost::asio::io_context::executor_type>> workGuards;
    std::vector<std::thread> threads;
};
