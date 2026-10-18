#pragma once 
#include <vector>
#include <memory>

struct Event
{
	virtual ~Event() = default;
};


class IObserver
{
public:
	virtual ~IObserver() = default;
	virtual void notify(Event& event) = 0;
};

class ISubject
{

public:
	bool subscribe(const std::shared_ptr<IObserver>& observer)
	{
		if (observer == nullptr) return false;
		for (const auto& weakObserver : observers_)
		{
			if (auto existing = weakObserver.lock();
				existing && existing == observer)
				return false; 
		}
		observers_.push_back(observer);
		return true;
	}

protected:
	void Emit(Event& event)
	{
		for (auto it = observers_.begin(); it != observers_.end();)
		{
			if (auto observer = it->lock())
			{
				observer->notify(event);
				++it;
			}
			else
			{
				it = observers_.erase(it);
			}
		}
	}



private:
	std::vector<std::weak_ptr<IObserver>> observers_;
};
