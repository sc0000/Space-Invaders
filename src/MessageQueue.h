#pragma once
#include <deque>
#include <mutex>
#include <condition_variable>
#include <memory>

template <typename T>
class MessageQueue
{
public:
	void send(T&& msg)
	{
		std::lock_guard<std::mutex> lckG(_mtx);
		_queue.emplace_back(std::move(msg));
		_cond.notify_one();
	}

	T receive()
	{
		std::unique_lock<std::mutex> lckU(_mtx);
		_cond.wait(lckU, [this]() { return !_queue.empty(); });
		T t = std::move(_queue.front());
		_queue.pop_front();
		return t;
	}
	
private:
	std::deque<T> _queue;
	std::mutex _mtx;
	std::condition_variable _cond;
};

enum AudioTrigger
{
	LeftBorder,
	MidBorder,
	RightBorder,
	HighPaddle,
	MidPaddle,
	LowPaddle,
	Goal,
	NoAudio
};

enum ShootingTrigger
{
	Shoot,
	DontShoot
};