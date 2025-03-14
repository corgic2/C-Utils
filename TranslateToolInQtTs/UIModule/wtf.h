// ************************************************************
// <remarks>
// Author      : chenguanhua
// CreateTime  : 2023-08-23
// Description : 语言家demo
// </remarks>
// ************************************************************
#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_wtf.h"
#include "QExcel.h"
#include "QMap"

/// <summary>
/// Class
/// </summary>
class wtf : public QMainWindow
{
    Q_OBJECT

public:
    wtf(QWidget* parent = Q_NULLPTR);
    /// <summary>
    /// Finalizes an instance of the <see cref="wtf"/> class.
    /// </summary>
    ~wtf();
    /// <summary>
    /// 去灰
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="strSrc">The string source.</param>
    /// <param name="strDest">The string dest.</param>
    /// <returns>bool.</returns>
    bool RemvoeTsFileVanished(const QString& strSrc, const QString& strDest);
    /// <summary>
    /// 从ts文件中获取键值对
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="strSrc">The string source.</param>
    /// <returns>bool.</returns>
    bool GetAllMap(const QString& strSrc);
    /// <summary>
    /// 创建表格
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="strSrc">The string source.</param>
    /// <returns>bool.</returns>
    bool CreatXlsx(const QString& strSrc);
    bool CreateXlsxForRepetition(const QString& strSrc);
    bool CreatSentenceXlsx(const QString& strSrc);
    /// <summary>
    /// 在原有基础上创建表格
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="strSrc">The string source.</param>
    /// <returns>bool.</returns>
    bool KeepXlsx(const QString& strSrc);
    /// <summary>
    /// 替换表格中的数据，不新加表格
    /// </summary>
    /// <param name="strSrc">The string source.</param>
    /// <returns>bool.</returns>
    bool ChangeXlsxByTs(QString& strSrc);
    /// <summary>
    /// Reads the XLSX.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="strSrc">The string source.</param>
    /// <returns>bool.</returns>
    bool ReadXlsx(const QString& strSrc);
    /// <summary>
    /// 继续翻译的时候将原本表格的翻译记录起来不再添加翻译
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="strSrc">The string source.</param>
    /// <param name="nowKeyValue">The now key value.</param>
    /// <returns>bool.</returns>
    bool ReadXlsxToMap(const QString& strSrc, std::map<QString, std::pair< std::vector<QString>, std::map<QString, int> > >& nowKeyValue);
    /// <summary>
    /// Changes the path file.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="filename1">The filename1.</param>
    /// <returns>QStringList.</returns>
    QStringList ChangePathFile(QString filename1);
    /// <summary>
    /// Exchanges the ts.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="strSrc">The string source.</param>
    /// <returns>bool.</returns>
    bool exchangeTS(const QString& strSrc);
    /// <summary>
    /// 将所有翻译打勾
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="strSrc">The string source.</param>
    /// <returns>bool.</returns>
    bool approverTS(const QString& strSrc);
    /// <summary>
    /// Exchanges the color of the ts.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="strSrc">The string source.</param>
    /// <returns>bool.</returns>
    bool exchangeTsColor(const QString& strSrc);
    /// <summary>
    /// Readxlsxes the specified string source.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="strSrc">The string source.</param>
    /// <returns>bool.</returns>
    bool readxlsx(const QString& strSrc);

    void initTips();
    void initConnect();
public slots:
    void CLearGray();
    /// <summary>
    /// Creats the XLSX for new.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    void CreatXlsxForNew();
    /// <summary>
    /// Keeps the fix XLSX.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    void KeepFixXlsx();
    /// <summary>
    /// 替换表格中的翻译
    /// </summary>
    void ReplaceXlsxByTs();
    /// <summary>
    /// Shows the ts dialogtrr.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    void showTSDialogtrr();
    /// <summary>
    /// Shows the ts dialogpopo.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    void showTSDialogpopo();
    /// <summary>
    /// Shows the ts dialogok.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    void showTSDialogok();
    /// <summary>
    /// Shows the ts dialogin.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    void showTSDialogin();
    /// <summary>
    /// Shows the ts dialogout.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    void showTSDialogout();
    /// <summary>
    /// Changes all of ts.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    void ChangeAllOfTs();
    /// <summary>
    /// Refreshes the XLSX.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    void refreshXlsx();
    /// <summary>
    /// Refreshes the has refresh.
    /// </summary>
    /// <remarks>
    /// <para> [Version]:</para>
    /// ChangeInfo:
    /// </remarks>
    /// <param name="">The .</param>
    void refreshHasRefresh(QString);
    void RepetitionClicked(bool isclicked);

    void FilterSentence();
private:
    /// <summary>
    /// The UI
    /// </summary>
    Ui::wtfClass ui;
    /// <summary>
    /// The excel
    /// </summary>
    QExcel* excel = nullptr;
    /// <summary>
    /// The un approval
    /// </summary>
    std::map<QString, bool> unApproval;
    /// <summary>
    /// The color for name
    /// </summary>
    std::map<QString, QColor> colorForName;
    /// <summary>
    /// All key value
    /// </summary>
    std::map<QString, std::pair< std::vector<QString>, std::map<QString, int> > > allKeyValue;
    /// <summary>
    /// The now name
    /// </summary>
    QString nowName = nullptr;
    /// <summary>
    /// The warning nub
    /// </summary>
    int warningNub = 0;
    /// <summary>
    /// The has refresh
    /// </summary>
    bool hasRefresh = false;

    QStringList showfilevec;
};
