#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

std::mutex g_lock;
static const int num_threads = 10;

void call_from_thread(int& t_id)
{
	g_lock.lock();
	cout << "Launched by thread " << ++t_id << endl;
	g_lock.unlock();
}

void func()
{
	//lock_guard在构造时会自动锁定互斥量,而在退出作用域后进行析构时就会自动解锁.
	lock_guard<std::mutex> lock(g_lock);
	cout << "in id: " << this_thread::get_id() << endl;
	this_thread::sleep_for(chrono::seconds(1));
	cout << "out id: " << this_thread::get_id() << endl;
}

int main()
{
	thread t[num_threads];
	int j = 0;
	for (int i = 0; i < num_threads; ++i)
		t[i] = thread(call_from_thread, j);
	cout << "Launched from the main function";
	for (int i = 0; i < num_threads; ++i)
		t[i].join();

	std::thread t1(func);
	std::thread t2(func);
	std::thread t3(func);

	t1.join();
	t2.join();
	t3.join();
	return 0;
}