//
//  ThreadManager.cpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 08.08.25.
//
#include "ThreadManager.hpp"


#include <print>


ThreadManager::ThreadManager() :
    // Initialize as much threads as available, but leave one for the main thread
        m_threadCount(std::thread::hardware_concurrency() - 1) {
        
    if (m_threadCount < 1) {
        
        // Ensure at least 1 threads for performance
        m_threadCount = 1;
    }
        
    initialize();
    std::print("ThreadManager initialized with {} threads.\n", m_threadCount);
}

ThreadManager::~ThreadManager() {
    m_running = false;
    
    m_condition.notify_all();
    
    for (auto& thread : m_threads) {
        
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    m_threads.clear();
    std::print("ThreadManager destroyed.\n");
}


void ThreadManager::initialize() {
    
    m_running = true;
    
    // If threads are already initialized, do nothing
    if (!m_threads.empty()) { return; }
    
    for (int i = 0; i < m_threadCount; ++i) {
        m_threads.emplace_back(&ThreadManager::workerLoop, this);
    }
    
    std::print("ThreadManager initialized with {} worker threads.\n", m_threads.size());
}

void ThreadManager::workerLoop() {
    while(m_running) {
        std::function<void()> task;
        
        {
            // Lock the queue mutex to access the task queue
            std::unique_lock<std::mutex> lock(m_queueMutex);
            
            // Wait until there is a task or the manager is not running
            m_condition.wait(lock, [this] {
                return !m_taskQueue.empty() || !m_running;
            });
            
            // Stop the worker if not running and no tasks are left
            if (!m_running && m_taskQueue.empty()) {
                return;
            }
            
            // Get a task
            task = std::move(m_taskQueue.front());
            m_taskQueue.pop();
        }
        
        task();
    }
}
