#include "MySQLInterface.h"

MySQLInterface::MySQLInterface() :ErrorNum(0), ErrorInfo("ok")
{
	mysql_library_init(0, NULL, NULL);
	mysql_init(&MysqlInstance);

	// �����ַ����������޷���������
	mysql_options(&MysqlInstance, MYSQL_SET_CHARSET_NAME, "gbk");
}

MySQLInterface::~MySQLInterface()
{
}

// ����������Ϣ
void MySQLInterface::SetMySQLConInfo(char* server, char* username, char* password, char* database, int &port)
{
	MysqlConInfo.server = server;
	MysqlConInfo.user = username;
	MysqlConInfo.password = password;
	MysqlConInfo.database = database;
	MysqlConInfo.port = port;
}

// ������
bool MySQLInterface::Open()
{
	if (0 != mysql_real_connect(&MysqlInstance, MysqlConInfo.server, MysqlConInfo.user, \
		MysqlConInfo.password, MysqlConInfo.database, MysqlConInfo.port, 0, 0))
	{
		return true;
	}
	else
	{
		ErrorInfoMySQL();
		return false;
	}
}

// �Ͽ�����
void MySQLInterface::Close()
{
	mysql_close(&MysqlInstance);
}

// ��ȡ����
bool MySQLInterface::Select(const std::string& Querystr, std::vector<std::vector<std::string> >& data)
{
	if (0 != mysql_query(&MysqlInstance, Querystr.c_str()))
	{
		ErrorInfoMySQL();
		return false;
	}

	Result = mysql_store_result(&MysqlInstance);

	// ������
	int row = mysql_num_rows(Result);
	int field = mysql_num_fields(Result);

	MYSQL_ROW line = NULL;
	line = mysql_fetch_row(Result);

	int j = 0;
	std::string temp;
	std::vector<std::vector<std::string> >().swap(data);
	while (NULL != line)
	{
		std::vector<std::string> linedata;
		for (int i = 0; i < field; ++i)
		{
			if (line[i])
			{
				temp = line[i];
				linedata.push_back(temp);
			}
			else
			{
				temp = "";
				linedata.push_back(temp);
			}
		}
		line = mysql_fetch_row(Result);
		data.push_back(linedata);
	}
	return true;
}

// ��������
bool MySQLInterface::Query(const std::string& Querystr)
{
	if (0 == mysql_query(&MysqlInstance, Querystr.c_str()))
		return true;

	ErrorInfoMySQL();
	return false;
}

// ���벢��ȡ�����ID������Զ�����ID
int MySQLInterface::GetInsertID(const std::string& Querystr)
{
	if (!Query(Querystr))
	{
		ErrorInfoMySQL();
		return  ERROR_QUERY_FAIL;
	}

	// ��ȡID
	return mysql_insert_id(&MysqlInstance);
}

// ������Ϣ
void MySQLInterface::ErrorInfoMySQL()
{
	ErrorNum = (int)mysql_error(&MysqlInstance);
	ErrorInfo = mysql_error(&MysqlInstance);
}