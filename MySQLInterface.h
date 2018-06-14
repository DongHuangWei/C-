#ifndef __MYSQL_INTERFACE_H__
#define __MYSQL_INTERFACE_H__

#include <string>
#include <vector>

#include <WinSock2.h> // 远程访问
#include <mysql.h>

// 引入库
#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"libmysql.lib")

#define ERROR_QUERY_FAIL -1	// 操作失败

// 定义MySQL连接信息
typedef struct MySQLConInfo
{
	char* server;
	char* user;
	char* password;
	char* database;
	int port;
};

class MySQLInterface
{
public:
	MySQLInterface();
	virtual ~MySQLInterface();

	void SetMySQLConInfo(char* server, char* username, char* password, char* database, int &port); // 设置连接信息
	bool Open();
	void Close();

	bool Select(const std::string& Querystr, std::vector<std::vector<std::string> >& data);	// 读取数据
	bool Query(const std::string& Querystr);	// 其他操作
	int GetInsertID(const std::string& Querystr);	// 插入并获取插入的ID，针对自动递增ID
	void ErrorInfoMySQL();	// 错误信息

public:
	int ErrorNum;			// 错误码
	const char* ErrorInfo;	// 错误提示

private:
	MySQLConInfo MysqlConInfo;	//连接信息
	MYSQL MysqlInstance;		// MySQL对象
	MYSQL_RES *Result;			// 结果
};

#endif
