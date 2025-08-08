//
//  ThreadManager.hpp
//  Visual-Physics Engine
//
//  Created by Kilian Brecht on 08.08.25.
//
#ifndef THREAD_MANAGER_HPP
#define THREAD_MANAGER_HPP

#include <SFML/Graphics/VertexArray.hpp>

#include <queue>
#include <functional>
#include <mutex>
#include <vector>
#include <thread>
#include <atomic>
#include <condition_variable>
#include <future>


class ThreadManager {
public:
    ThreadManager();
    ~ThreadManager();
    
    size_t getThreadCount() const { return m_threadCount; }
    
    template<class F, class ...Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result<F, Args...>::type> {
        
        // Create a packaged task to wrap the function and its arguments
        using return_type = typename std::invoke_result<F, Args...>::type;
        
        auto task = std::make_shared<std::packaged_task<return_type()>>(
            std::bind(std::forward<F>(f), std::forward<Args>(args)...)
        );
        
        // Get the future from the task
        std::future<return_type> res = task->get_future();
        
        {
            std::lock_guard<std::mutex> lock(m_queueMutex);
            
            // Add the task to the queue
            m_taskQueue.emplace([task]() { (*task)(); });
        }

        m_condition.notify_one();
        
        return res;
    }
    
private:
    void initialize();
    
    void workerLoop();
    
private:
    size_t m_threadCount;
    
    std::queue<std::function<void()>> m_taskQueue;
    std::mutex m_queueMutex;
    
    std::vector<std::thread> m_threads;
    
    std::atomic<bool> m_running;
    
    std::condition_variable m_condition;
};


#endif // THREAD_MANAGER_HPP
