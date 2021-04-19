/*
	Created by Paul O'Callaghan
*/
#pragma once
#include <iostream>
#include <chrono>

class Timer
{
public:
	Timer(std::string s);
	~Timer();

	void Start();
	void Stop();

	void SetSlowest(std::chrono::nanoseconds time);
	void SetFastest(std::chrono::nanoseconds time);
	void SetAverage();
	std::string name = "";
private:

	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;

	std::chrono::nanoseconds fastest;
	std::chrono::nanoseconds slowest;
	long long average;

	std::vector<std::chrono::nanoseconds> times = std::vector<std::chrono::nanoseconds>();

	bool firstFastestSet = false;
	bool firstSlowestSet = false;
};

Timer::Timer(std::string s)
{
	name = s;
}

Timer::~Timer()
{
	std::cout << "======" << name << "=====" << std::endl;

	std::cout << "Fastest: " << fastest.count() << " ns" << std::endl;

	std::cout << "Slowest: " << slowest.count() << " ns" << std::endl;

	SetAverage();
	std::cout << "Mean: " << average << "ns" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

void Timer::Start()
{
	start = std::chrono::system_clock::now();
}

void Timer::Stop()
{
	end = std::chrono::system_clock::now();
	auto nanoSeconds = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

	SetFastest(nanoSeconds);
	SetSlowest(nanoSeconds);
	times.push_back(nanoSeconds);
}

void Timer::SetFastest(std::chrono::nanoseconds time)
{
	if (!firstFastestSet)
	{
		fastest = time;
		firstFastestSet = true;
		return;
	}

	if (time.count() < fastest.count())
	{
		fastest = time;
	}

}


void Timer::SetSlowest(std::chrono::nanoseconds time)
{
	if (!firstSlowestSet)
	{
		slowest = time;
		firstSlowestSet = true;
		return;
	}

	if (time.count() > slowest.count())
	{
		slowest = time;
	}
}

void Timer::SetAverage()
{
	std::chrono::nanoseconds time = std::chrono::duration_cast<std::chrono::nanoseconds>(times.at(0));
	int j = 1;

	for (int i = 1; i < times.size(); i++)
	{
		time = std::chrono::duration_cast<std::chrono::nanoseconds>(time + times.at(i));
		++j;
	}
	average = time.count() / j;
}
