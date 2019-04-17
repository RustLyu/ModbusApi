#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

#include "recordmanager.h"
#include "Log.h"

RecordManager* RecordManager:: Instance_ = new RecordManager();
RecordManager::RecordManager()
{
}

RecordManager::~RecordManager()
{
	m_context.close();
}

bool RecordManager::connect()
{
	m_context = QSqlDatabase::addDatabase("QMYSQL");
	m_context.setHostName("127.0.0.1");
    m_context.setDatabaseName("testmodbuslib");
	m_context.setPort(3306);
	m_context.setUserName("root");
	m_context.setPassword("root");
	if (m_context.open())
	{
        LOG("database connect success");
		return true;
	}
	else
	{
        ERROR("database connect error");
		return false;
	}
}

bool RecordManager::update(QString sql)
{
	QSqlQuery query;
	bool success = query.exec(sql);
	if (!success)
	{
		QSqlError error = query.lastError();
        WARNING(sql.toStdString ());
        WARNING(error.text ().toStdString ());
	}
	return success;
}

bool RecordManager::insert(QString sql)
{
	QSqlQuery query;
	bool success = query.exec(sql);
	if (!success)
	{
		QSqlError error = query.lastError();
        WARNING(sql.toStdString ());
        WARNING(error.text ().toStdString ());
	}
	return success;
}

bool RecordManager::querry(QSqlQuery *querry, QString sql)
{
	bool success = querry->exec(sql);
	if (!success)
	{
		QSqlError error = querry->lastError();
        WARNING(sql.toStdString ());
        WARNING(error.text ().toStdString ());
	}
	return success;
}
