#include "pch.h"
#include <iostream>
#include <vector>
#include <mutex>
#include <thread>

using namespace std;
mutex mtx1;
mutex mtx2;
mutex mtx3;
mutex mtx4;
mutex mtx5;
vector<mutex*> forks;
mutex concole_mtx;


class MyClass
{
	int name;
	int left;
	int right;
public:
	MyClass(int name) {
		this->name = name;
		this->left = name;
		this->right = name == 4 ? 0 : name + 1;
	}

	bool take_left_fork()
	{
		return forks[left]->try_lock();
	}

	bool take_right_fork()
	{
		return forks[right]->try_lock();
	}

	void put_left_fork()
	{
		forks[left]->unlock();
	}

	void put_right_fork()
	{
		forks[right]->unlock();
	}

	void Eat()
	{
		concole_mtx.lock();
		cout << "Prof " << name << " eat. ID " << this_thread::get_id() << endl;
		concole_mtx.unlock();
		this_thread::sleep_for(chrono::seconds(2));
	}

	void Work()
	{
		while (true)
		{
			if (take_left_fork())
			{
				if (take_right_fork())
				{
					Eat();
					put_left_fork();
					put_right_fork();
					concole_mtx.lock();
					cout << "Prof " << name << " think. ID " << this_thread::get_id() << endl;
					concole_mtx.unlock();
					this_thread::sleep_for(chrono::milliseconds(20));
				}
				else
				{
					put_left_fork();
					this_thread::sleep_for(chrono::milliseconds(20));
				}
			}
			else
			{
				this_thread::sleep_for(chrono::milliseconds(20));
			}
		}
	}
};

int main()
{
	forks.push_back(&mtx1);
	forks.push_back(&mtx2);
	forks.push_back(&mtx3);
	forks.push_back(&mtx4);
	forks.push_back(&mtx5);
	MyClass* prof_1 = new MyClass(0);
	MyClass* prof_2 = new MyClass(1);
	MyClass* prof_3 = new MyClass(2);
	MyClass* prof_4 = new MyClass(3);
	MyClass* prof_5 = new MyClass(4);
	thread tr1(&MyClass::Work, prof_1);
	thread tr2(&MyClass::Work, prof_2);
	thread tr3(&MyClass::Work, prof_3);
	thread tr4(&MyClass::Work, prof_4);
	thread tr5(&MyClass::Work, prof_5);
	tr1.join();
	tr2.join();
	tr3.join();
	tr4.join();
	tr5.join();
}