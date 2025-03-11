/********************************************************
* 通用路径查询
*********************************************************/
#pragma once
#include <QStandardPaths>
#include <QDir>
#include <shlobj.h>
#include <QSettings>
#include <QCoreApplication>
#include <QProcessEnvironment>
#include <QUuid>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <qDebug>
#define TPSAPPNAME "/Manteia TPS"
#define TPSCONFIGNAME "/Manteia"

class TPSFileSystemUtil
{
public:
    /// <summary>
    /// 获取病人数据以外，其他TPS数据存放的总目录
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS"</returns>
    static QString getDocManteiaAppDir()
    {
        QString docManteiaAppDirPath = QProcessEnvironment::systemEnvironment().value("PROGRAMDATA") + TPSAPPNAME;
        docManteiaAppDirPath = docManteiaAppDirPath.replace("\\", "/");
        tryCreateDir(docManteiaAppDirPath);
        return QDir(docManteiaAppDirPath).absoluteFilePath(docManteiaAppDirPath) + "/";
    }
    /// <summary>
    /// 获取账号目录
    /// </summary>
    /// <returns>"C:/Users/UserName/MOZIConfig/"</returns>
    static QString GetUserConfigPath(const QString& userName)
    {
        QString dirPath = QString("C:/Users/") + userName + "/MOZIConfig/";
        dirPath = dirPath.replace("\\", "/");
        tryCreateDir(dirPath);
        return dirPath;
    }
    /// <summary>
    /// TPS配置文件存放的总目录
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia\"</returns>
    static QString getManteiaConfigDir()
    {
        QString manteiaConfigPath = QProcessEnvironment::systemEnvironment().value("PROGRAMDATA") + TPSCONFIGNAME;
        manteiaConfigPath = manteiaConfigPath.replace("\\", "/");
        tryCreateDir(manteiaConfigPath);
        return QDir(manteiaConfigPath).absoluteFilePath(manteiaConfigPath) + "/";
    }

    static QString getMTBitAnswerServiceINIFilePath()
    {
        QString docManteiaAppDirPath = QProcessEnvironment::systemEnvironment().value("PROGRAMDATA");
        docManteiaAppDirPath = docManteiaAppDirPath.replace("\\", "/");
        tryCreateDir(docManteiaAppDirPath);
        return docManteiaAppDirPath + "/Manteia/MtClientService.ini";
    }
    /// <summary>
    /// 获取Manteia Application目录
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia Application/""</returns>
    static QString getManteiaApplicationDir()
    {
        QString appDirPath = "C:/Users/Public/Documents/Manteia Application/";
        tryCreateDir(appDirPath);
        return appDirPath;
    }

    /// <summary>
    /// 获取TPS其他dicom存放的总目录
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\DICOM\"</returns>
    static QString getDocDICOMDir()
    {
        QString dicomDirPath = getDocManteiaAppDir() + "DICOM/";
        tryCreateDir(dicomDirPath);
        return dicomDirPath;
    }

    /// <summary>
    /// 获取TPS的ini配置文件文件夹路径
    /// </summary>
    /// <returns>"C:\ProgramData\AccuTPSServer\TPSConfiguration\"</returns>
    static QString getManteiaTPSINIFileDire()
    {
        QString tpsiniDir = getDocManteiaAppDir() + "TPSConfiguration/";
        tryCreateDir(tpsiniDir);
        return tpsiniDir;
    }

    /// <summary>
    /// 获取TPS的ini配置文件
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\TPSConfiguration\ManteiaTPS.ini"</returns>
    static QString getManteiaTPSINIFilePath()
    {
        QString tpsiniDir = getDocManteiaAppDir() + "TPSConfiguration/";
        tryCreateDir(tpsiniDir);
        QString tpsiniPath = tpsiniDir + "ManteiaTPS.ini";
        tryCreateFile(tpsiniPath);
        return tpsiniPath;
    }

    /// <summary>
    /// 获取TPS的ini配置文件中的语言
    /// </summary>
    /// <returns>语言字符串：ch或en</returns>
    static QString getLanguageFromManteiaTPSINI()
    {
        QString ConfigFilePath = TPSFileSystemUtil::getManteiaTPSINIFilePath();
        QSettings settings(ConfigFilePath, QSettings::IniFormat);
        QString languageStr = settings.value("language", "ch").toString();      //语言类型
        return languageStr;
    }

    /// <summary>
    /// 获取TPS的账号服务ini配置文件
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\TPSConfiguration\ManteiaTPS_Account.ini"</returns>
    static QString getManteiaTPSWebAccountFilePath()
    {
        QString  tpsAccountiniDir = getDocManteiaAppDir() + "TPSConfiguration/";
        tryCreateDir(tpsAccountiniDir);
        QString tpsAccountiniPath = tpsAccountiniDir + "ManteiaTPS_Account.ini";
        tryCreateFile(tpsAccountiniPath);
        return tpsAccountiniPath;
    }

    /// <summary>
    /// 获取TPS的rpc ini配置文件
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\TPSConfiguration\MTRPCTcpclientserviceConfig.ini"</returns>
    static QString getManteiaTPSRPCIniFilePath()
    {
        QString  tpsAccountiniDir = getDocManteiaAppDir() + "TPSConfiguration/";
        tryCreateDir(tpsAccountiniDir);
        QString tpsAccountiniPath = tpsAccountiniDir + "MTRPCTcpclientserviceConfig.ini";
        tryCreateFile(tpsAccountiniPath);
        return tpsAccountiniPath;
    }

    /// <summary>
    /// 获取数据库配置文件
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\TPSConfiguration\MTDataBaseServiceConfig.ini"</returns>
    static QString getManteiaTPSDatabaseIniPath()
    {
        QString  tempDir = getDocManteiaAppDir() + "TPSConfiguration/";
        tryCreateDir(tempDir);
        QString tempIniFile = tempDir + "MTDataBaseServiceConfig.ini";
        tryCreateFile(tempIniFile);
        return tempIniFile;
    }

    /// <summary>
    /// 获取算法运行的参数配置文件，优化参数/运行参数等
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\TPSConfiguration\AlgorithmSettings.ini"</returns>
    static QString getManteiaTPSAlgorithmIniPath()
    {
        QString  tempDir = getDocManteiaAppDir() + "TPSConfiguration/";
        tryCreateDir(tempDir);
        QString tempIniFile = tempDir + "AlgorithmSettings.ini";
        tryCreateFile(tempIniFile);
        return tempIniFile;
    }

    /// <summary>
    /// 获取算法服务的Ini配置文件，插件目录算法目录等
    /// </summary>
    /// <returns>" C:/ProgramData/Manteia TPS/AlgorithmConfig/AlgorithmPathConfig.ini"</returns>
    static QString getManteiaTPSAlgoPathIniPath()
    {
        QString tempIniFile = getAlgorithmConfigPath() + "AlgorithmPathConfig.ini";
        tryCreateFile(tempIniFile);
        return tempIniFile;
    }

    /// <summary>
    /// 获取TPS的限制的ini配置文件
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\TPSConfiguration\ManteiaTPS_Limit.ini"</returns>
    static QString getManteiaTPSLimitConfigFile()
    {
        QString  tpsAccountiniDir = getDocManteiaAppDir() + "TPSConfiguration/";
        tryCreateDir(tpsAccountiniDir);
        QString tpsAccountiniPath = tpsAccountiniDir + "ManteiaTPS_Limit.ini";
        tryCreateFile(tpsAccountiniPath);
        return tpsAccountiniPath;
    }
    /// <summary>
    /// 获取ART助手与MOZI交互的配置文件
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\TPSConfiguration\ARTAssitantMOZI.ini"</returns>
    static QString getARTAssitantMOZIConfigFile()
    {
        QString  configPath = getDocManteiaAppDir() + "TPSConfiguration/";
        tryCreateDir(configPath);
        QString iniPathFile = configPath + "ARTAssitantMOZI.ini";
        tryCreateFile(iniPathFile);
        return iniPathFile;
    }

    /// <summary>
    /// 获取TPS的翻译文件目录
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\TranslationConfig"</returns>
    static QString getManteiaTPSTranslationDir()
    {
        QString  tpsTranslationDir = getDocManteiaAppDir() + "TranslationConfig/";
        tryCreateDir(tpsTranslationDir);
        return tpsTranslationDir;
    }

    /// <summary>
    /// 获取TPS的翻译文件目录下的算法配置
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\TranslationConfig\AlgorithmLanguage.xlsx"</returns>
    static QString getManteiaTPSTranslationAlgoFile()
    {
        QString  tpsTranslationDir = getDocManteiaAppDir() + "TranslationConfig/";
        tryCreateDir(tpsTranslationDir);
        QString algoTransFile = tpsTranslationDir + "AlgorithmLanguage.xlsx";
        return algoTransFile;
    }
    /// <summary>
    /// 获取TPS专属的文件夹路径
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia TPS/"</returns>
    static QString getTPSDocumentDir()
    {
        QString docDicomDirPath = "C:/Users/Public/Documents/Manteia TPS/";
        docDicomDirPath = docDicomDirPath.replace("\\", "/");
        tryCreateDir(docDicomDirPath);
        return docDicomDirPath;
    }

    /// <summary>
    /// 获取程序Debug输出的日志目录
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia TPS/DebugDayLog"</returns>
    static QString getDebugDayLogDir()
    {
        QString debugDayLog = getTPSDocumentDir() + "DebugDayLog/";
        tryCreateDir(debugDayLog);
        return debugDayLog;
    }
    /// <summary>
    /// 设置程序崩溃文件目录
    /// 由于3.3.5保存崩溃文件从配置文件中获取
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia TPS/ErrorReport"</returns>
    static void setErrorReportDir(QString reportDir)
    {
        QString ConfigFilePath = TPSFileSystemUtil::getManteiaTPSINIFilePath();
        QSettings settings(ConfigFilePath, QSettings::IniFormat);
        settings.beginGroup("ConfigDump");
        settings.setValue("dumpPath", reportDir);
        settings.endGroup();
        //QString ReportDir = settings.value("dumpPath").toString();
    }
    /// <summary>
    /// 获取程序崩溃文件目录
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia TPS/ErrorReport"</returns>
    static QString getErrorReportDir()
    {
        QString ConfigFilePath = TPSFileSystemUtil::getManteiaTPSINIFilePath();
        QSettings settings(ConfigFilePath, QSettings::IniFormat);
        settings.beginGroup("ConfigDump");
        QString reportDir = settings.value("dumpPath").toString();
        tryCreateDir(reportDir);
        settings.endGroup();
        return reportDir;
    }

    /// <summary>
    /// 获取物理模块Debug输出的日志目录
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia TPS/PhysicistDebugDayLog"</returns>
    static QString getPhysicistDebugDayLogDir()
    {
        QString debugDayLog = getTPSDocumentDir() + "PhysicistDebugDayLog/";
        tryCreateDir(debugDayLog);
        return debugDayLog;
    }

    /// <summary>
    /// 获取物理模块崩溃文件目录
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia TPS/PhysicistErrorReport"</returns>
    static QString getPhysicistErrorReportDir()
    {
        QString reportDir = getTPSDocumentDir() + "PhysicistErrorReport/";
        tryCreateDir(reportDir);
        return reportDir;
    }

    /// <summary>
    /// 获取TPS专属的临时文件夹路径
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia TPS/Temp/"</returns>
    static QString getTPSTempDir()
    {
        QString docDicomDirPath = "C:/Users/Public/Documents/Manteia TPS/";
        QString tempdir = docDicomDirPath + "Temp/";
        tempdir = tempdir.replace("\\", "/");
        tryCreateDir(tempdir);
        return tempdir;
    }


    /// <summary>
    /// 获取算法临时文件夹路径
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia TPS/Temp/Algorithm/"</returns>
    static QString getAlgorithmTempDir()
    {
        QString algorithmTemp = getTPSTempDir();
        algorithmTemp = algorithmTemp + "Algorithm/";
        tryCreateDir(algorithmTemp);
        return algorithmTemp;
    }

    /// <summary>
    /// 获取计划报告临时文件夹路径
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia TPS/Temp/PlanReport/"</returns>
    static QString getPlanReportTempDir()
    {
        QString reportTemp = getTPSTempDir();
        reportTemp = reportTemp + "PlanReport/";
        tryCreateDir(reportTemp);
        return reportTemp;
    }
/// <summary>
/// 获取相似性分析产生的临时nii文件夹路径
/// </summary>
/// <returns>"C:/Users/Public/Documents/Manteia TPS/Temp/niiTemp"</returns>
    static QString getNiiTempDir()
    {
        QString niiTemp = getTPSTempDir();
        niiTemp = niiTemp + "niiTemp/";
        tryCreateDir(niiTemp);
        return niiTemp;
    }

    /// <summary>
    /// 获取RT临时保存目录，如果需要其他Dicom临时的统一建立在这个目录下，参考getSketchTempDir
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\DICOM\RTTemp"</returns>
    static QString getRTTempDir()
    {
        QString docDicomDirPath = getDocDICOMDir();//"C:\ProgramData\Manteia TPS\DICOM\"
        QString tempdir = docDicomDirPath + "RTTemp/";
        tryCreateDir(tempdir);
        return tempdir;
    }

    /// <summary>
    /// 获取RT临时保存目录，TPS目前用于自动勾画产生的临时文件
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\DICOM\RTTemp\sketchTemp"</returns>
    static QString getSketchTempDir()
    {
        QString rtTempDir = getAlgorithmTempDir() + "sketchTemp/";
        tryCreateDir(rtTempDir);
        return rtTempDir;
    }

/// <summary>
/// 获取ART助手的临时文件夹
/// </summary>
/// <returns>"C:/Users/Public/Documents/Manteia TPS/Temp/ARTAssistant"</returns>
    static QString getARTAssistantTempDir()
    {
        QString temp = getTPSTempDir();
        temp = temp + "ARTAssistant/";
        tryCreateDir(temp);
        return temp;
    }


/// <summary>
/// 获取缩略图临时文件夹路径。moduleName用于防止每个模块互相影响
/// </summary>
/// <returns>"C:/Users/Public/Documents/Manteia TPS/Temp/ThumbnailImg/"</returns>
    static QString getThumbnailPath(const QString& moduleName = QString())
    {
        QString tempPath = getTPSTempDir();
        tempPath = tempPath + "ThumbnailImg/";

        if (moduleName.size() > 0)
        {
            tempPath = tempPath + moduleName + "/";
        }

        tryCreateDir(tempPath);
        return tempPath;
    }

    /// <summary>
    /// 获取配准信息缩略图路径 type 0：主序列缩略图 1：次序列缩略图 2：融合缩略图
    /// </summary>
    /// <returns>"C:/Users/Public/Documents/Manteia TPS/Temp/ThumbnailImg/"</returns>
    static QString getRegistrationThumbnailPath(int type, const QString& dirPath, const QString& patientID, const QString& mainSeriesUID, const QString& secondSeriesUID)
    {
        QString errMsg;
        QString uid = QString();

        if (0 == type)
        {
            uid = mainSeriesUID;
        }
        else if (1 == type)
        {
            uid = secondSeriesUID;
        }
        else
        {
            uid = mainSeriesUID + "_" + secondSeriesUID;
        }

        QString pixPathFile = dirPath + "/" + patientID + "/Registration/Thumbnail/" + uid + "/";
        tryCreateDir(pixPathFile);
        pixPathFile += QString("2.png");
        return pixPathFile;
    }

    /// <summary>
    /// 获取Dicom第三方接收到的文件目录
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\DICOM\receivefile"</returns>
    static QString getReceiveDICOMFileTempDir()
    {
        QString docDicomDirPath = getDocDICOMDir();
        QString tempdir = docDicomDirPath + "receivefile/";
        tryCreateDir(tempdir);
        return tempdir;
    }

    /// <summary>
    /// 获取剂量计算的文件夹拼接后缀
    /// </summary>
    /// <returns>"Temp\"</returns>
    static QString getDoseCalcTempPath()
    {
        return "Temp/";
    }

    /// <summary>
    /// 获取剂量预测文件夹拼接后缀
    /// </summary>
    /// <returns>"RtDicom/PredictionDose/"</returns>
    static QString getPredictedDosePath()
    {
        return "PredictionDose/";
    }

    /// <summary>
    /// 获取QA模体存放目录文件夹拼接后缀
    /// </summary>
    /// <returns>"modelDicom/"</returns>
    static QString getQAModelDicomPath()
    {
        return "modelDicom/";
    }

    /// <summary>
    /// 获取tps未上传（上传失败）到web的日志目录
    /// </summary>
    /// <returns>"C:\ProgramData\Manteia TPS\ErrorLog\"</returns>
    static QString getErrorLogPath()
    {
        return getDocManteiaAppDir() + "ErrorLog/";
    }

    /// <summary>
    /// 获取TPS加速器配置路径
    /// </summary>
    /// <returns> C:/ProgramData/Manteia TPS/LinacConfiguration/</returns>
    static QString getLinacConfigPath()
    {
        QString  dirPath = getDocManteiaAppDir() + "LinacConfiguration/";
        dirPath = dirPath.replace("\\", "/");
        tryCreateDir(dirPath);
        return dirPath;
    }

    /// <summary>
    /// 获取TPS物理模块 CT配置路径
    /// </summary>
    /// <returns> C:/ProgramData/Manteia TPS/LinacConfiguration/</returns>
    static QString getCTSimConfigPath()
    {
        QString  dirPath = getDocManteiaAppDir() + "CTSimConfiguration/";
        dirPath = dirPath.replace("\\", "/");
        tryCreateDir(dirPath);
        return dirPath;
    }


    /// <summary>
    /// 获取床结构配置路径
    /// </summary>
    /// <returns> C:/ProgramData/Manteia TPS/CouchConfig/</returns>
    static QString getCouchConfigPath()
    {
        QString  dirPath = getDocManteiaAppDir() + "CouchConfig/";
        dirPath = dirPath.replace("\\", "/");
        tryCreateDir(dirPath);
        return dirPath;
    }
    /// <summary>
    /// 获取算法配置路径
    /// </summary>
    /// <returns> C:/ProgramData/Manteia TPS/AlgorithmConfig/</returns>
    static QString getAlgorithmConfigPath()
    {
        QString  dirPath = getDocManteiaAppDir() + "AlgorithmConfig/";
        dirPath = dirPath.replace("\\", "/");
        tryCreateDir(dirPath);
        return dirPath;
    }

    //获取算法文件夹存放路径--插件版的算法目录
    static QString getAlgorithmExecutePath_plugin()
    {
        QString  dirPath = "C:/ProgramData/TpsAlgorithmAlgo/";
        tryCreateDir(dirPath);
        return dirPath;
    }

    /// <summary>
    /// 获取服务配置文件路径
    /// </summary>
    static QString getServiceConfigPath()
    {
        QString  dirPath = getDocManteiaAppDir() + "ServiceConfig/";
        dirPath = dirPath.replace("\\", "/");
        tryCreateDir(dirPath);
        return dirPath;
    }

    /// <summary>
    /// 获取布局配置路径
    /// </summary>
    static QString getModuleLayoutConfigPath(const QString& userName, const QString& modulePluginName)
    {
        QString  dirPath = GetUserConfigPath(userName) + "ModuleLayoutConfig/";
        dirPath = dirPath + modulePluginName + "/";
        dirPath = dirPath.replace("\\", "/");
        tryCreateDir(dirPath);
        return dirPath;
    }

    /// <summary>
    /// 获取大模块布局配置文件(全路径)。modulePluginName插件名称
    /// </summary>
    static QString getModuleLayoutConfigPathFile(const QString& userName, const QString& modulePluginName)
    {
        QString dirPath = getModuleLayoutConfigPath(userName, modulePluginName);
        QString pathFile = dirPath + modulePluginName + "Layout.json";
        return pathFile;
    }

    /// <summary>
    /// 获取Tps工作流配置路径
    /// </summary>
    static QString getTpsWorkFlowConfigPath()
    {
        QString  dirPath = getDocManteiaAppDir() + "WorkFlowConfig/";
        dirPath = dirPath.replace("\\", "/");
        tryCreateDir(dirPath);
        return dirPath;
    }

    /// <summary>
    /// 获取等剂量线的配置文件路径
    /// </summary>
    static QString getIsodoseLineConfigPath()
    {
        QString dirPath = getDocManteiaAppDir() + "IsodoseLineConfig/";
        dirPath = dirPath.replace("\\", "/");
        tryCreateDir(dirPath);
        return dirPath;
    }

    static QString GetPlanOptSettingFile(const QString& userName)
    {
        QString planOptSetPathFile = GetUserConfigPath(userName) + "PlanOptSetting.json";
        return planOptSetPathFile;
    }
    /// <summary>
    /// 文件名转码，有些有特殊符号不能当文件名的转掉
    /// </summary>
    /// <returns>转码后的文件名</returns>
    static QString encodeIllegalChar(const QString& filename)
    {
        QString str = filename;
        str = str.replace("\\", "5C");
        str = str.replace("/", "2F");
        str = str.replace(":", "3A");
        str = str.replace("*", "2A");
        str = str.replace("?", "3F");
        str = str.replace("\"", "22");
        str = str.replace("<", "3C");
        str = str.replace(">", "3E");
        str = str.replace("|", "7C");
        return str;
    }

    /// <summary>
    /// 文件名拆分，用于模体获取目录UUID
    /// </summary>
    /// <returns>模体目录UUID</returns>
    static QString getQAModelUUID( QString fileSuffixPath)
    {
        QString result;
        QStringList  tempPathList = fileSuffixPath.split("/");

        if (tempPathList.size() < 3)
        {
            tempPathList = fileSuffixPath.split("\\");
        }

        //两种解析方式都不行。直接返回空
        if (tempPathList.size() < 3)
        {
            return result;
        }

        result = tempPathList[tempPathList.size() - 3];

        if (!result.isEmpty())
        {
            result += "/";
        }

        return result;
    }



    /// <summary>
    ///递归创建目录
    /// </summary>
    /// <returns>是否成功</returns>
    static bool tryCreateDir(const QString& dirPath)
    {
        QString path = dirPath;
        path = path.replace("\\", "/");
        QDir dir(path);

        if (dir.exists(path))
        {
            return true;
        }

        //父亲目录
        QString parentDir = path.mid(0, path.lastIndexOf('/'));

        if (parentDir == path)//已经是顶级目录了，顶级目录不存在，直接错误
        {
            return false;
        }

        if (!tryCreateDir(parentDir))
        {
            return false;
        }

        QString dirname = path.mid(path.lastIndexOf('/') + 1);
        QDir parentPath(parentDir);

        if (!dirname.isEmpty())
            return parentPath.mkpath(dirname);

        return true;
    }

    /// <summary>
    ///创建文件
    /// </summary>
    /// <returns>是否成功</returns>
    static bool tryCreateFile(const QString& filePath)
    {
        QFile file(filePath);

        if (!file.exists())
        {
            if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
            {
                return false;
            }

            file.close();
        }

        return true;
    }

    /// <summary>
    ///创建UUID
    /// </summary>
    /// <returns>是否成功</returns>
    static QString createUUIDQString()
    {
        QString uuid = QUuid::createUuid().toString();
        uuid = uuid.remove("{").remove("}");
        return uuid;
    }

    //
    static bool GetJsonFromFile(QString& fileName, QJsonObject& outJsonObj)
    {
        if (fileName.isEmpty())
            return false;

        QFile loadFile(fileName);

        if (!loadFile.open(QIODevice::ReadOnly))
        {
            qWarning() << "could't open projects json";
            return false;
        }

        QByteArray allData = loadFile.readAll();
        loadFile.close();
        QJsonParseError json_error;
        QJsonDocument jsonDoc(QJsonDocument::fromJson(allData, &json_error));

        if (json_error.error != QJsonParseError::NoError)
        {
            qWarning() << "json error!";
            return false;
        }

        outJsonObj = jsonDoc.object();
        loadFile.close();
        return true;
    }


    static void SaveJsonToFile(const QString& fileName, QJsonObject& fileObj)
    {
        //clear
        QFile file1(fileName);
        file1.open(QFile::WriteOnly | QFile::Truncate);
        file1.close();
        //update
        QFile file(fileName);

        if (!file.open(QIODevice::ReadWrite))
        {
            qDebug() << "File open error";
            return;
        }

        QJsonDocument document;
        document.setObject(fileObj);
        file.write(document.toJson());
        file.flush();
        file.close();
    }
    /// <summary>
/// 获取等剂量线的配置文件路径
/// </summary>
    static QString getReportTemplatePath()
    {
        QString dirPath = getDocManteiaAppDir() + "ReportTemplateFile/";
        dirPath = dirPath.replace("\\", "/");
        tryCreateDir(dirPath);
        return dirPath;
    }
    /// <summary>
    /// 获取账号管理系统的总目录
    /// </summary>
    /// <returns>"C:/Program Files (x86)/MOZIAccountManager"</returns>
    static QString getAccountManagerDir()
    {
        QString docAccountManagerDirPath =  "C:/Program Files (x86)/MOZIAccountManager";
        docAccountManagerDirPath = docAccountManagerDirPath.replace("\\", "/");
        tryCreateDir(docAccountManagerDirPath);
        return QDir(docAccountManagerDirPath).absoluteFilePath(docAccountManagerDirPath) + "/";
    }
};

