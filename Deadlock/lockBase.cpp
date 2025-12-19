#include "pch.h"
#include <iostream>
#include "CorePch.h"
#include <thread>
#include <mutex>

vector<int32> v;

// Mutual Exclusive (상호 배타적)
mutex m;

//RAII (Resource Acquisition Is InitialIzation
template<typename T>
class LockGuard
{
public:
	LockGuard(T& m)	// 생성자
	{
		_mutex = &m;
		_mutex->lock();
	}

	~LockGuard()	// 소멸자
	{
		_mutex->unlock();
	}
private:
	T* _mutex;
};
 
void Push()
{
	for (int i = 0; i < 10000; i++)
	{
		// m.lock();
		LockGuard<std::mutex> lockGuard(m); // lockGuard가 유효하지 않으면 알아서 unlock!
		std::lock_guard<std::mutex> lockGuard(m); // 이미 정의되어 있는 클래스
		std::unique_lock<std::mutex> uniqueLock(m, defer_lock); // lockGuard + 당장 lock을 잠그지 않고
																// 잠기는 순간을 정할 수 있음

		v.push_back(i);

		if (i == 5000)
			break;	// lock을 영영 풀지 못함 -> 무한 대기

		// m.unlock();
	}
}

int main()
{
	std::thread t1(Push);
	std::thread t2(Push);

	t1.join();
	t2.join();

	cout << v.size() << endl;
}
