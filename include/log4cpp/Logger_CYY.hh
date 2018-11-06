#ifndef _MYLOG_H
#define _MYLOG_H

#include<log4cpp/Category.hh>
#include<iostream>

//��־���ȼ�
enum ELogLevel
{
    E_Fatal  = 0,    ///>����������־�ȼ�
    E_Error  = 300,  ///>������־�ȼ�
    E_Warn   = 400,  ///>������־�ȼ�
    E_Notice = 500,  ///>ע����־�ȼ�
    E_Info   = 600,  ///>һ����־�ȼ�
    E_Debug  = 700,  ///>������־�ȼ�
    E_NotSet = 800,
    E_Unknow = 900  /**< don't use */
};

//�õ���ģʽ��װlog4cpp
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
     * @brief ������־
     * @param stringFormat ��־��ʽ�ַ���
     * @param ...
     */
    void Debug_f(const char* stringFormat, ...) throw();
    void Debug(const std::string& message) throw();

    /**
     * @brief һ��֮��
     * @param stringFormat
     */
    void Info_f(const char* stringFormat, ...) throw();
    void Info(const std::string& message) throw();

    /**
     * @brief Notice ע����־
     * @param message
     */
    void Notice_f(const char* stringFormat, ...) throw();
    void Notice(const std::string& message) throw();

    /**
     * @brief ������־
     * @param stringFormat
     */
    void Warn_f(const char* stringFormat, ...) throw();
    void Warn(const std::string& message) throw();

    /**
     * @brief ������־
     * @param stringFormat
     */
    void Error_f(const char* stringFormat, ...) throw();
    void Error(const std::string& message) throw();

    /**
     * @brief ����������־
     * @param stringFormat
     */
    void Fatal_f(const char* stringFormat, ...) throw();
    void Fatal(const std::string& message) throw();

    /**
     * @brief ������־�ȼ�
     * @param level ��־�ȼ�
     */
    void SetLogLevel(ELogLevel level);

    /**
     * @brief IsLevelEnable ��⵱ǰ�ȼ��Ƿ�ʹ��
     * @param level ��������־�ȼ�
     * @return
     */
    bool IsLevelEnable(ELogLevel level);

private:
	Mylog();  //����ģʽ�����캯��˽�л�	

public:
	void AddAppender(void*, bool customappender = true);
	void RemoveAppender(void*, bool customappender = true);

 private:
    class Impl;
	std::shared_ptr<Impl> m_pimpl;
};

//*****************************************************
//ע�⣺
//�ļ��� __FILE__ ,������ __func__ ���к�__LINE__ �Ǳ�����ʵ�ֵ�
//����C++ͷ�ļ��ж���� 
//ǰ����������string���ͣ���__LINE__�����Σ�������ҪתΪstring����
//******************************************************

//���������ļ��к� ->ת��Ϊstring����
inline std::string int2string(int line) {
    std::ostringstream oss;
    oss << line;
    return oss.str();
}


//����һ������־����� �ļ��� ������ �к� �ĺ궨��
#define suffix(msg)  std::string(msg).append(" ##")\
        .append(__FILE__).append(":").append(__func__)\
        .append(":").append(int2string(__LINE__))\
        .append("##").c_str()


//����ÿ��ʹ��ʱд getInstance���
//ֻ��Ҫ���������ļ���д: #define _LOG4CPP_����������������ʹ��
#ifdef _LOG4CPP_
#define LOGGER()  Mylog::getInstance()
#endif

//���̲��򻯺���������ʽ
#define logError(msg) LOGGER()->Error(suffix(msg))
#define logWarn(msg) LOGGER()->Warn(suffix(msg))
#define logInfo(msg) LOGGER()->Info(suffix(msg))
#define logDebug(msg) LOGGER()->Debug(suffix(msg))
#define logNotice(msg) LOGGER()->Notice(suffix(msg))
#define logFatal(msg) LOGGER()->Fatal(suffix(msg))

#endif