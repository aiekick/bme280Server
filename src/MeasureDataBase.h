#pragma once

#include <string>

struct sqlite3;
class MeasureDataBase
{
private:
	sqlite3* m_SqliteDB = nullptr;
	std::string m_DataBaseFilePathName;

public:
	void AddBME280SensorDatas(const uint64_t& vDateTime, const float& vTemperature, const float& vPressure, const float& vHumidity);
	std::string GetHistoryToJson(const int& vCountLasts);

private:
	bool OpenDB();
	void CreateDB();
	void CloseDB();

public: // singleton
	static MeasureDataBase* Instance(const std::string& vDataBaseFilePathName = "")
	{
		static MeasureDataBase _instance(vDataBaseFilePathName);
		return &_instance;
	}
	MeasureDataBase(const std::string& vDataBaseFilePathName) : m_DataBaseFilePathName(vDataBaseFilePathName) {};
	MeasureDataBase(const MeasureDataBase&) = delete;
	MeasureDataBase& operator =(const MeasureDataBase&) = delete;
	~MeasureDataBase() = default;
};