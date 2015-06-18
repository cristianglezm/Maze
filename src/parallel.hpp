#include <thread>
#include <future>
#include <atomic>

#include <iostream>

namespace parallel{
    /**
     * @brief
     */
    template<typename Iterator, class Function>
    void for_each(const Iterator& first, const Iterator& last, Function&& f){
        const auto length = std::distance(first,last);
        const auto minPerThread = 1000;
        if(length<minPerThread){
            std::for_each(first,last,f);
            return;
        }
        const auto maxThreads = (length+minPerThread-1)/minPerThread;
        const int hwThreads = std::thread::hardware_concurrency();
        const auto numThreads = std::min(hwThreads,maxThreads);
        const auto blockSize = length/numThreads;
        std::vector<std::thread> threads(numThreads-1);
        auto BlockStart = first;
        for(auto i = 0;i<(numThreads-1);++i){
            auto BlockEnd = BlockStart;
            std::advance(BlockEnd,blockSize);
            threads[i] = std::thread([=,&f](){ std::for_each(BlockStart, BlockEnd, f); });
            BlockStart = BlockEnd;
        }
        std::for_each(BlockStart, last, f);
        for(auto& t:threads){ t.join(); }
    }
    /**
     *
     */
    template<class CondFunction,class Function>
    void _while(CondFunction&& c,Function&& f){
        std::thread([&](){ while(c()){ f(); } }).detach();
    }
}
