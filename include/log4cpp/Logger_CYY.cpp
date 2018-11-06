#include<iostream>
#include "Logger_CYY.hh"
#include<log4cpp/PatternLayout.hh>
#include<log4cpp/OstreamAppender.hh>
#include<log4cpp/FileAppender.hh>
#include<log4cpp/Priority.hh>
#include<direct.h>
#include <stdlib.h>
#include <io.h>
#include <assert.h>
#include <time.h>
#include <iomanip>
using namespace std;


class Mylog::Impl
{
public:
    Impl():m_root(log4cpp::Category::getRoot() ),
        m_loglevel(E_Info)
    {
        Init();
    }

    ~Impl()
    {
        log4cpp::Category::shutdown();
    }

public:
    /**
     * @brief 调试日志
     * @param stringFormat 日志格式字符串
     * @param ...
     */
    void Debug(const std::string& message) throw()
    {
        m_root.debug(message);
       // CustomMessage(message, E_Debug);
    }

    /**
     * @brief 一般之日
     * @param stringFormat
     */
    void Info(const std::string& message) throw()
    {
        m_root.info(message);
    //    CustomMessage(message, E_Info);
    }

    /**
     * @brief Notice 注意日志
     * @param message
     */
    void Notice(const std::string& message) throw()
    {
        m_root.notice(message);
      //  CustomMessage(message, E_Notice);
    }

    /**
     * @brief 警告日志
     * @param stringFormat
     */
    void Warn(const std::string& message) throw()
    {
        m_root.warn(message);
       // CustomMessage(message, E_Warn);
    }

    /**
     * @brief 错误日志
     * @param stringFormat
     */
    void Error(const std::string& message) throw()
    {
        m_root.error(message);
       // CustomMessage(message, E_Error);
    }

    /**
     * @brief 致命错误日志
     * @param stringFormat
     */
    void Fatal(const std::string& message) throw()
    {
        m_root.fatal(message);
      //  CustomMessage(message, E_Fatal);
    }

    /**
     * @brief 设置日志等级
     * @param level 日志等级
     */
    void SetLogLevel(ELogLevel level)
    {
        m_loglevel = level;
        log4cpp::Priority::Value plevel;
        switch(level)
        {
        case E_Fatal:
            plevel = log4cpp::Priority::FATAL;
            break;
        case E_Error:
            plevel = log4cpp::Priority::ERROR;
            break;
        case E_Warn:
            plevel = log4cpp::Priority::WARN;
            break;
        case E_Notice:
            plevel = log4cpp::Priority::NOTICE;
            break;
        case E_Info:
            plevel = log4cpp::Priority::INFO;
            break;
        case E_Debug:
            plevel = log4cpp::Priority::DEBUG;
            break;
        case E_NotSet:
            plevel = log4cpp::Priority::DEBUG;
            break;
        case E_Unknow:
            plevel = log4cpp::Priority::DEBUG;
            break;
        default:
            plevel = log4cpp::Priority::DEBUG;
            break;
        }

        m_root.setPriority(plevel);

//        m_root.setPriority((log4cpp::Priority::Value)level);
    }

    /**
     * @brief IsLevelEnable 检测当前等级是否使能
     * @param level 被检测的日志等级
     * @return
     */
    bool IsLevelEnable(ELogLevel level)
    {
        return (m_root.getPriority() >= level);
    }

    std::string vFrom(const char* format, va_list args)
    {
#if defined(_MSC_VER)
    #define VSNPRINTF _vsnprintf
#else
#ifdef LOG4CPP_HAVE_SNPRINTF
    #define VSNPRINTF vsnprintf
#else

#define HAVE_SNPRINTF
#define PREFER_PORTABLE_SNPRINTF

#include <stdlib.h>
#include <stdarg.h>

extern "C" {
#include "snprintf.c"
}

#define VSNPRINTF portable_vsnprintf

#endif // LOG4CPP_HAVE_SNPRINTF
#endif // _MSC_VER

        size_t size = 1024;
        char* buffer = new char[size];

        while (1) {
            va_list args_copy;

        #if defined(_MSC_VER) || defined(__BORLANDC__)
            args_copy = args;
        #else
            va_copy(args_copy, args);
        #endif

            int n = VSNPRINTF(buffer, size, format, args_copy);

            va_end(args_copy);

            // If that worked, return a string.
            if ((n > -1) && (static_cast<size_t>(n) < size)) {
                std::string s(buffer);
                delete [] buffer;
                return s;
            }

            // Else try again with more space.
            size = (n > -1) ?
                        n + 1 :   // ISO/IEC 9899:1999
                        size * 2; // twice the old size

            delete [] buffer;
            buffer = new char[size];
        }
    }

  /*  void AddAppender(void* appender, bool customappender = true)
        {
            if (!customappender)
            {
                log4cpp::Appender *obj = static_cast<log4cpp::Appender *>(appender);
                if (obj != NULL)
                {
                    m_pattern_layout = new log4cpp::PatternLayout();
                    m_pattern_layout->setConversionPattern("%d: [%p] %c %x: %m%n");
                    obj->setLayout(m_pattern_layout);
                    m_root.addAppender(obj);
                }
            }
            else
            {
                LoggerAppender *custom = static_cast<LoggerAppender *>(appender);
                if (custom != NULL)
                {
                    m_custom_appeders.push_back(custom);
                }
            }
        }

        void RemoveAppender(void *appender, bool customappender = true)
        {
            if (customappender)
            {
                LoggerAppender *custom = static_cast<LoggerAppender *>(appender);
                if (custom != NULL)
                {
                    std::vector<LoggerAppender *>::iterator iter = std::find(m_custom_appeders.begin(),
                                                                             m_custom_appeders.end(),
                                                                             custom);
                    if (iter != m_custom_appeders.end())
                    {
                        m_custom_appeders.erase(iter);
                    }
                }
            }
            else
            {
                log4cpp::Appender *obj = static_cast<log4cpp::Appender *>(appender);
                if (obj != NULL)
                {
                    m_root.removeAppender(obj);
                }
            }
        }*/
private:
    void Init()
    {
		char buffer[260];
        getcwd(buffer, 260);
		if (_access(buffer,0) == -1)
		{
          int re= _mkdir(buffer); 
		   assert(re);
		}
		std::string curpath(buffer);
        std::string filepath = curpath+ "/" + GetCurDateTime() + ".log";

        m_file_appender  = new log4cpp::FileAppender("osfileAppender",filepath); //log4cpp::Category::shutdown()时会delete
        m_pattern_layout = new log4cpp::PatternLayout();
        m_pattern_layout->setConversionPattern("%d: [%p] %c %x: %m%n");
        m_file_appender->setLayout(m_pattern_layout);

        m_root.addAppender(m_file_appender);
    }
    /**
     * @brief 获取当前日期
     * @return 返回日期的string 形式：YY-MM-DD
     */
    std::string GetCurDate()
    {
		 time_t rawtime;
         time(&rawtime);
		 char   pblgtime[20];
         strftime(pblgtime, 20, "%Y-%m-%d", localtime(&rawtime));
		 std::string str_day(pblgtime);
         return str_day;
    }

    /**
     * @brief 获取系统当前时间
     * @return 返回当前时间的字符串 形式：YYYYMMDDTHHMMSS,FFFF  FFFF是毫秒 T是日期与时间的分隔符
     */
    std::string GetCurDateTime()
    {    
		 time_t rawtime;
         time(&rawtime);
		 char   pblgtime[20];
         strftime(pblgtime, 20, "%Y-%m-%d %X", localtime(&rawtime));
		 std::string str_day(pblgtime);
         return str_day;
    }

    /**
    * @brief 获取当前系统时间
    * @return 返回当前时间字符串 形式：YYYY-MM-dd hh:mm:ss
    */
    std::string GetCurTime()
    {
         time_t rawtime;
         time(&rawtime);
		 char   pblgtime[20];
         strftime(pblgtime, 20, "%Y-%m-%d %X", localtime(&rawtime));
		 std::string str_day(pblgtime);
         return str_day;
    }

    /**
    * @brief 将日志等级转化为字符串描述
    * @param level
    * @return
    */
    std::string LogLevelToStr(ELogLevel level)
    {
        std::string level_str;
        switch(level)
        {
        case E_Fatal:
            level_str = " : [FATAL] : ";
            break;
        case E_Error:
            level_str = " : [ERROR] : ";
            break;
        case E_Warn:
            level_str = " : [WARN] : ";
            break;
        case E_Notice:
            level_str = " : [NOTICE] : ";
            break;
        case E_Info:
            level_str = " : [INFO] : ";
            break;
        case E_Debug:
            level_str = " : [DEBUG] : ";
            break;
        case E_NotSet:
            level_str = " : [NOTSET] : ";
            break;
        case E_Unknow:
            level_str = " : [UNKNOW] : ";
            break;
        default:
            level_str = " : [DEBUG] : ";
            break;
        }
        return level_str;
    }

    ///**
    // * @brief 自定义日志终端输出
    // * @param message
    // * @param level
    // */
    //void CustomMessage(const std::string &message, ELogLevel level)
    //{
    //    if (level <= m_loglevel)
    //    {
    //        for (int i = 0; i < m_custom_appeders.size(); i++)
    //        {
    //            m_custom_appeders[i]->Append(GetCurTime() + LogLevelToStr(level) + message + "\n");
    //        }
    //    }
    //}
private:
    log4cpp::FileAppender  *m_file_appender;    //文件输出
    log4cpp::PatternLayout *m_pattern_layout;   //输出格式布局
    log4cpp::Category      &m_root;             //系统日志类别

    ELogLevel              m_loglevel;          //日志等级
  //  std::vector<LoggerAppender*> m_custom_appeders;//自定义日志终端
};

Mylog::Mylog()
{
    m_pimpl = std::shared_ptr<Impl>(new Impl);
}

Mylog::~Mylog()
{

}

void Mylog::Debug_f(const char* stringFormat, ...) throw()
{
    va_list va;
    va_start(va,stringFormat);
    m_pimpl->Debug(m_pimpl->vFrom(stringFormat, va) );
    va_end(va);
}

void Mylog::Debug(const std::string& message) throw()
{
    m_pimpl->Debug(message);
}

void Mylog::Info_f(const char* stringFormat, ...) throw()
{
    va_list va;
    va_start(va,stringFormat);
    m_pimpl->Info(m_pimpl->vFrom(stringFormat, va) );
    va_end(va);
}

void Mylog::Info(const std::string& message) throw()
{
    m_pimpl->Info(message);
}

void Mylog::Notice_f(const char* stringFormat, ...) throw()
{
    va_list va;
    va_start(va,stringFormat);
    m_pimpl->Notice(m_pimpl->vFrom(stringFormat, va) );
    va_end(va);
}

void Mylog::Notice(const std::string& message) throw()
{
    m_pimpl->Notice(message);
}

void Mylog::Warn_f(const char* stringFormat, ...) throw()
{
    va_list va;
    va_start(va,stringFormat);
    m_pimpl->Warn(m_pimpl->vFrom(stringFormat, va) );
    va_end(va);
}

void Mylog::Warn(const std::string& message) throw()
{
    m_pimpl->Warn(message);
}

void Mylog::Error_f(const char* stringFormat, ...) throw()
{
    va_list va;
    va_start(va,stringFormat);
    m_pimpl->Error(m_pimpl->vFrom(stringFormat, va) );
    va_end(va);
}

void Mylog::Error(const std::string& message) throw()
{
    m_pimpl->Error(message);
}


void Mylog::Fatal_f(const char* stringFormat, ...) throw()
{
    va_list va;
    va_start(va,stringFormat);
    m_pimpl->Fatal(m_pimpl->vFrom(stringFormat, va) );
    va_end(va);
}

void Mylog::Fatal(const std::string& message) throw()
{
    m_pimpl->Fatal(message);
}

void Mylog::SetLogLevel(ELogLevel level)
{
    m_pimpl->SetLogLevel(level);
}

bool Mylog::IsLevelEnable(ELogLevel level)
{
    return m_pimpl->IsLevelEnable(level);
}

void Mylog::AddAppender(void *custom, bool customappender)
{
    //m_pimpl->AddAppender(custom, customappender);
}

void Mylog::RemoveAppender(void *custom, bool customappender)
{
   // m_pimpl->RemoveAppender(custom, customappender);
}
