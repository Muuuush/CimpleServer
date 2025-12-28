#include "IOContextPool.hpp"

#include <iostream>

using namespace boost::asio;

IOContextPool::IOContextPool(int size)
    : nextIndex(0), ioContexts(size) {
    workGuards.reserve(size);
    for (auto& ioc : ioContexts) {
        workGuards.emplace_back(make_work_guard(ioc));
    }
    threads.reserve(size);
    for (auto& ioc : ioContexts) {
        threads.emplace_back([&ioc]{
            ioc.run();
        });
    }
}

io_context& IOContextPool::getIOContext() {
    io_context& rtn = ioContexts[nextIndex];
    nextIndex = (nextIndex + 1) % ioContexts.size();
    return rtn;
}

IOContextPool::~IOContextPool() {
    workGuards.clear();
    for (auto& ioc : ioContexts)
        ioc.stop();
    for (auto& thrd : threads)
        thrd.join();
    std::cout << "[LOG]: IO context pool stopped." << std::endl;
}
