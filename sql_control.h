#pragma once 
#include <mysql/mysql.h>
#include<iostream>
#define TB_NAME "login"
#define MAX_SQL_LEN 1024
using std::cout;
using std::endl;
class MysqlOperator {
  public:
    MysqlOperator() {
      mysql_init(&_mysql); // 初始化数据库句柄
      MYSQL* ret = mysql_real_connect(&_mysql, "47.94.140.62", "root", 
          "LC199938","project", 0, nullptr, 0);
      if (ret == nullptr) {
        cout << "mysql connect error:" << mysql_errno(&_mysql) << endl;
        exit(-1);
      }
      // 字符集调整
    }
    ~MysqlOperator() {
      mysql_close(&_mysql);
    }
    // 增
    bool Insert(const std::string& name, const std::string& passwd, const std::string& number) {
      char sql[MAX_SQL_LEN] = {0};
      sprintf(sql, "insert into %s values(NULL, '%s', '%s', '%s');", 
          TB_NAME, name.c_str(), passwd.c_str(), number.c_str());
      int ret = mysql_query(&_mysql, sql);
      if (ret != 0) {
        cout << "mysql insert error:" << mysql_error(&_mysql) << endl;
        return false;
      }
      return true;
    }
    // 删
    bool Delete(const std::string& id) {
      char sql[MAX_SQL_LEN] = {0};
      sprintf(sql, "delete from %s where id = %s;", TB_NAME, id.c_str());
      int ret = mysql_query(&_mysql, sql);
      if (ret != 0) {
        cout << "mysql delete error:" << mysql_error(&_mysql) << endl;
        return false;
      }
      return true;
    }
    // 改
    bool Modify(const std::string& id, const std::string& newpasswd, 
        const std::string& newnumer) {
      char sql[MAX_SQL_LEN] = {0};
      if (!newpasswd.empty()) {
        sprintf(sql, "update %s set password = '%s' where id = %s;", 
            TB_NAME, newpasswd.c_str(), id.c_str());
      } else if (!newnumer.empty()) {
        sprintf(sql, "update %s set number = '%s' where id = %s",
            TB_NAME, newnumer.c_str(), id.c_str());
      } else {
        cout << "mysql modify with empty field" << endl;
        return false;
      }
      int ret = mysql_query(&_mysql, sql);
      if (ret != 0) {
        cout << "mysql modify error:" << mysql_error(&_mysql) << endl;
        return false;
      }
      return true;
    }
    // 查
    bool Query(std::string& info, const std::string& username) {
      char sql[MAX_SQL_LEN] = {0};
      sprintf(sql, "select password from %s where username = '%s';", 
          TB_NAME, username.c_str());
      int ret = mysql_query(&_mysql, sql);
      if (ret != 0) {
        cout << "mysql select error:" << mysql_error(&_mysql) << endl;
        return false;
      }
      MYSQL_RES* result = mysql_store_result(&_mysql);
      //int nums = mysql_num_fields(result);
      MYSQL_ROW row;
      row = mysql_fetch_row(result);
      info = row[0];
      //while ((row = mysql_fetch_row(result))) {
      //  for (int i = 0; i < nums; ++i) {
      //    info += row[i];
      //    info += " ";
      //  }
      //}
      mysql_free_result(result);
      return true;
    }
  private:
    MYSQL _mysql;
};
