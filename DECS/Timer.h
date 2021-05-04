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

	void PrintResults();

	void SetSlowest(std::chrono::microseconds time);
	void SetFastest(std::chrono::microseconds time);
	void SetAverage();
	std::string name = "";
private:

	std::chrono::time_point<std::chrono::system_clock> start;
	std::chrono::time_point<std::chrono::system_clock> end;

	std::chrono::microseconds fastest;
	std::chrono::microseconds slowest;
	long long average;

	std::vector<std::chrono::microseconds> times = std::vector<std::chrono::microseconds>();

	bool firstFastestSet = false;
	bool firstSlowestSet = false;
};

Timer::Timer(std::string s)
{
	name = s;
}

Timer::~Timer()
{
}

void Timer::Start()
{
	start = std::chrono::system_clock::now();
}

void Timer::Stop()
{
	end = std::chrono::system_clock::now();
	auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

	SetFastest(microseconds);
	SetSlowest(microseconds);
	times.push_back(microseconds);
}

inline void Timer::PrintResults()
{
	std::cout << "======" << name << "=====" << std::endl;

	std::cout << "Fastest: " << fastest.count() << " ns" << std::endl;

	std::cout << "Slowest: " << slowest.count() << " ns" << std::endl;

	SetAverage();
	std::cout << "Mean: " << average << " ns" << std::endl;
	std::cout << std::endl;
	std::cout << std::endl;
}

void Timer::SetFastest(std::chrono::microseconds time)
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


void Timer::SetSlowest(std::chrono::microseconds time)
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
	std::chrono::microseconds time = std::chrono::duration_cast<std::chrono::microseconds>(times.at(0));
	int j = 1;

	for (int i = 1; i < times.size(); i++)
	{
		time = std::chrono::duration_cast<std::chrono::microseconds>(time + times.at(i));
		++j;
	}
	average = time.count() / j;
}
