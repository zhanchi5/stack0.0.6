#include "stack.h"
#include <thread>
#include <unistd.h>

template<typename T>
void producer(stack<T> &stack_, std::mutex& mtx){


    for (;;){
        int a = rand();
        stack_.push(a);
        std::cout<< a << "+" << "\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % (3) + 1));


    }
}
template<typename T>
void consumer(stack<T>& stack_,std::mutex& mtx){
    for (;;){

        try {
            std::cout<<stack_.pop() << "-" << "\n";
            stack_.pop();


        }
        catch(...) {
        }


        std::this_thread::sleep_for(std::chrono::milliseconds(std::rand() % (3) + 2));
    }
}

int main(){
    stack<int> st1;
    std::mutex mt1;
    std::thread t1(consumer<int>, std::ref(st1), std::ref(mt1));
    std::thread t2(producer<int>, std::ref(st1), std::ref(mt1));
    std::thread t3(consumer<int>, std::ref(st1), std::ref(mt1));
    std::thread t4(producer<int>, std::ref(st1), std::ref(mt1));
    t1.join();
    t2.join();
    t3.join();
    t4.join();
    return 0;
}
