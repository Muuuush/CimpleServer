#include "LogicSystem.hpp"
#include <spdlog/spdlog.h>

LogicSystem::LogicSystem(int capacity, int workerNum)
    : logicQueue(capacity),
    capacity(capacity), head(0), tail(0), fill(0),
    empty(), full(), stop(false) {
    workers.reserve(workerNum);
    for (int i = 0; i < workerNum; ++i) {
        workers.emplace_back(&LogicSystem::worker, this);
    }
}

void LogicSystem::worker() {
    while (true) {
        std::unique_lock lock(queueMutex);
        while (fill <= 0 && !stop)
            full.wait(lock, [this]() { return fill > 0 || stop; });
        if (stop && fill <= 0) break; // stop

        LogicNode node = std::move(logicQueue[head]);
        fill--;
        head = (head + 1) % capacity;
        empty.notify_one();

        // handle node
        if (callbacks.contains(node.type)) {
            try {
                callbacks[node.type](node.session, node.type, node.message);
            } catch (const std::exception& e) {
                spdlog::error("{}, from {}", e.what(), node.session->toString());
            } catch (...) {
                spdlog::error("Unknown error, from {}", node.session->toString());
            }
        } else {
            spdlog::error("Unknown type: {}, from {}", node.type,node.session->toString());
        }
    }
}

void LogicSystem::registerNode(const LogicNode& node) {
    if (stop) return; // if stop, don't receive new node

    std::unique_lock lock(queueMutex);
    while (fill >= capacity)
        empty.wait(lock);
    logicQueue[tail] = node;
    fill++;
    tail = (tail + 1) % capacity;
    full.notify_one();
}

LogicSystem::~LogicSystem() {
    stop = true;
    full.notify_all();
    for (auto& worker : workers)
        worker.join();
    spdlog::info("Logic system stopped.");
}
