#ifndef OBSERVER_H
#define OBSERVER_H

#include<vector>
#include<algorithm>

class Observer
{
public: 
	virtual ~Observer() = default;
	virtual void update() = 0;

};

class Observable
{
private: 
	std::vector<Observer*>observers;

public: 
	virtual ~Observable() = default;
	void addObserver(Observer* observer)
	{
		observers.push_back(observer);
	}
	void removeObserver(Observer* observer) 
	{
		observers.erase(std::remove(observers.begin(), observers.end(), observer),
			observers.end());
	}

	void notifyObservers()
	{
		for (Observer* observer : observers) 
		{
			observer->update();
		}
	}
};
#endif
