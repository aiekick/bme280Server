#pragma once

#include <uv/include/uv11.hpp>
#include <memory>

class SensorHistory
{
private:
	std::unique_ptr<uv::Timer> m_Task;
	uint64_t m_DelayInSeconds = 0U;

public:
	void Init(uv::EventLoop& vEventLoop);

public: // singleton
	static SensorHistory* Instance(const uint64_t& vDelayInSeconds = 0U)
	{
		static SensorHistory _instance(vDelayInSeconds);
		return &_instance;
	}
	SensorHistory(const uint64_t& vDelayInSeconds) :m_DelayInSeconds(vDelayInSeconds) {}
	SensorHistory(const SensorHistory&) = delete;
	SensorHistory& operator =(const SensorHistory&) = delete;
	~SensorHistory() = default;
};