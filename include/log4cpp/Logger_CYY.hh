#ifndef _MYLOG_H
#define _MYLOG_H

#include<log4cpp/Category.hh>
#include<iostream>

//日志优先级
enum ELogLevel
{
    E_Fatal  = 0,    ///>致命错误日志等级
    E_Error  = 300,  ///>错误日志等级
    E_Warn   = 400,  ///>警告日志等级
    E_Notice = 500,  ///>注意日志等级
    E_Info   = 600,  ///>一般日志等级
    E_Debug  = 700,  ///>调试日志等级
    E_NotSet = 800,
    E_Unknow = 900  /**< don't use */
};

//用单例模式封装log4cpp
class Mylog {
 public: 
    static Mylog* getInstance()
	{
		static Mylog instance ;
        return  &instance ;
	}
	~Mylog();

    void setPriority(ELogLevel priority);	

	 /**
     * @brief 调试日志
     * @param stringFormat 日志格式字符串
     * @param ...
     */
    void Debug_f(const char* stringFormat, ...) throw();
    void Debug(const std::string& message) throw();

    /**
     * @brief 一般之日
     * @param stringFormat
     */
    void Info_f(const char* stringFormat, ...) throw();
    void Info(const std::string& message) throw();

    /**
     * @brief Notice 注意日志
     * @param message
     */
    void Notice_f(const char* stringFormat, ...) throw();
    void Notice(const std::string& message) throw();

    /**
     * @brief 警告日志
     * @param stringFormat
     */
    void Warn_f(const char* stringFormat, ...) throw();
    void Warn(const std::string& message) throw();

    /**
     * @brief 错误日志
     * @param stringFormat
     */
    void Error_f(const char* stringFormat, ...) throw();
    void Error(const std::string& message) throw();

    /**
     * @brief 致命错误日志
     * @param stringFormat
     */
    void Fatal_f(const char* stringFormat, ...) throw();
    void Fatal(const std::string& message) throw();

    /**
     * @brief 设置日志等级
     * @param level 日志等级
     */
    void SetLogLevel(ELogLevel level);

    /**
     * @brief IsLevelEnable 检测当前等级是否使能
     * @param level 被检测的日志等级
     * @return
     */
    bool IsLevelEnable(ELogLevel level);

private:
	Mylog();  //单例模式：构造函数私有化	

public:
	void AddAppender(void*, bool customappender = true);
	void RemoveAppender(void*, bool customappender = true);

 private:
    class Impl;
	std::shared_ptr<Impl> m_pimpl;
};

//*****************************************************
//注意：
//文件名 __FILE__ ,函数名 __func__ ，行号__LINE__ 是编译器实现的
//并非C++头文件中定义的 
//前两个变量是string类型，且__LINE__是整形，所以需要转为string类型
//******************************************************

//整数类型文件行号 ->转换为string类型
inline std::string int2string(int line) {
    std::ostringstream oss;
    oss << line;
    return oss.str();
}


//定义一个在日志后添加 文件名 函数名 行号 的宏定义
#define suffix(msg)  std::string(msg).append(" ##")\
        .append(__FILE__).append(":").append(__func__)\
        .append(":").append(int2string(__LINE__))\
        .append("##").c_str()


//不用每次使用时写 getInstance语句
//只需要在主函数文件中写: #define _LOG4CPP_即可在整个程序中使用
#ifdef _LOG4CPP_
#define LOGGER()  Mylog::getInstance()
#endif

//缩短并简化函数调用形式
#define logError(msg) LOGGER()->Error(suffix(msg))
#define logWarn(msg) LOGGER()->Warn(suffix(msg))
#define logInfo(msg) LOGGER()->Info(suffix(msg))
#define logDebug(msg) LOGGER()->Debug(suffix(msg))
#define logNotice(msg) LOGGER()->Notice(suffix(msg))
#define logFatal(msg) LOGGER()->Fatal(suffix(msg))

#endif