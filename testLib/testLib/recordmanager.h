#ifndef RECORDMANAGER_H
#define RECORDMANAGER_H

#include <QSqlQuery>

class RecordManager
{
public:
    RecordManager();
    ~RecordManager();
    bool static querry(QSqlQuery *querry, QString sql);
    bool static insert(QString sql);
    bool static update(QString sql);
    bool connect();
    static RecordManager* Instance()
    {
    return Instance_;
    }
private:
    QSqlDatabase m_context;
private:
    static RecordManager* Instance_;
};

#endif // RECORDMANAGER_H
