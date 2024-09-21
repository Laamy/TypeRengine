#pragma once

#ifndef CONCURRENT_VECTOR_H
#define CONCURRENT_VECTOR_H

#include <vector>
#include <mutex>
#include <thread>
#include <stdexcept>

template<typename T>
class LockableVector
{
public:
	void push_back(const T& value)
	{
		std::lock_guard<std::mutex> lock(conCurrencyMutex);
		data_.push_back(value);
	}

	inline T as(size_t index) const
	{
		std::lock_guard<std::mutex> lock(conCurrencyMutex);

		if (index >= data_.size())
			throw std::out_of_range("Index out of range");

		return data_.at(index);
	}

	inline void remove(size_t index)
	{
		std::lock_guard<std::mutex> lock(conCurrencyMutex);

		if (index >= data_.size())
			throw std::out_of_range("Index out of range");

		data_.erase(data_.begin() + index);
	}

	inline size_t size() const
	{
		std::lock_guard<std::mutex> lock(conCurrencyMutex);
		return data_.size();
	}

private:
	std::vector<T> data_;
	mutable std::mutex conCurrencyMutex;
};

#endif // CONCURRENT_VECTOR_H