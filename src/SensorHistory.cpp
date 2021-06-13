#include "SensorHistory.h"
#include "SensorBME280.h"
#include "MeasureDataBase.h"

void SensorHistory::Init(uv::EventLoop& vEventLoop)
{
	uint64_t time_out_ms = (uint64_t)1100 * m_DelayInSeconds; // 1.1 * m_DelayInSeconds
	uint64_t time_repeat_ms = (uint64_t)1000 * m_DelayInSeconds; // m_DelayInSeconds

	m_Task = std::unique_ptr<uv::Timer>(new uv::Timer(&vEventLoop, time_out_ms, time_repeat_ms, [](uv::Timer* ptr)
		{
			SensorBME280DatasStruct datas;
			if (SensorBME280::Instance()->GetSensorBME280Datas(&datas))
			{
				MeasureDataBase::Instance()->AddBME280SensorDatas(datas.epoc, datas.temp, datas.pres, datas.humi);
			}
		}));
	m_Task->start();
}