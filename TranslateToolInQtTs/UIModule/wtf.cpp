#include "wtf.h"
#include<ActiveQt/QAxObject>
#include <QDomDocument>
#include <QDomElement>
#include <QDomNodeList>
#include <QDir>
#include <QStringList>
#include <QIODevice>
#include <QDebug>
#include "QExcel.h"
#include "QMessageBox.h"
#include "QFileDialog.h"
#include "QApplication.h"
#include "QXmlStreamWriter"
#include <QDomText>
#include "QStringListModel.h"
#include "QTableWidget.h"
#include <QString.h>
#include <QRegularExpression>
#pragma execution_character_set("utf-8")
wtf::wtf(QWidget* parent)
    : QMainWindow(parent)
{
    excel = new QExcel();
    ui.setupUi(this);
    initTips();
    initConnect();
//     ui.trr->setText(tr("..."));
//     ui.popo->setText(tr("..."));
//     ui.okk->setText(tr("..."));
//     ui.in->setText(tr("..."));
//     ui.out->setText(tr("..."));
//     connect(ui.trr, SIGNAL(clicked()), this, SLOT(showTSDialogtrr()));
//     connect(ui.popo, SIGNAL(clicked()), this, SLOT(showTSDialogpopo()));
//     connect(ui.okk, SIGNAL(clicked()), this, SLOT(showTSDialogok()));
//     connect(ui.in, SIGNAL(clicked()), this, SLOT(showTSDialogin()));
//     connect(ui.out, SIGNAL(clicked()), this, SLOT(showTSDialogout()));
//     ui.label_gray->setText(tr("如果输入文件夹绝对路径则会遍历文件夹本层内可修改的文件\n\n\n将目标文件的绝对路径输入，原ts文件将去除不可用的翻译（原本翻译移动位置后会导致翻译不可用）"));
//     ui.testforgray->setText(tr("目标文件的绝对路径"));
//     ui.clear_Gray->setText(tr("去灰"));
//     connect(ui.clear_Gray, SIGNAL(clicked()), this, SLOT(CLearGray()));
//     ui.label_toxlsx->setText(tr("将目标文件的绝对路径输入，在保存位置下生成拥有所有ts的excel表格，配合生成qph使用。\n后续翻译仅需维护表格即可\nPS:标红为多种翻译，标蓝为未通过审批"));
//     ui.testforxlsx->setText(tr("目标文件的绝对路径"));
//     ui.label_outputpath->setText(tr("保存表格位置"));
//     ui.creatXlsx->setText(tr("重新创建表格"));
//     ui.KeepXlsx->setText(tr("继续生成表格"));
//     //有缺陷，暂时关闭
//     //ui.KeepXlsx->hide();
//     connect(ui.creatXlsx, SIGNAL(clicked()), this, SLOT(CreatXlsxForNew()));
//     connect(ui.KeepXlsx, SIGNAL(clicked()), this, SLOT(KeepFixXlsx()));
//     ui.label_toqph->setText(tr("根据输入表格来修改ts文件起到自动化作用\nPS：如果表格有多种翻译且ts出现该翻译会提示"));
//     ui.testforqph->setText(tr("表格位置"));
//     ui.label_ChangeTs->setText(tr("需要修改的ts文件"));
//     ui.refreshXls->setText(tr("表格更改"));
//     connect(ui.refreshXls, SIGNAL(clicked()), this, SLOT(refreshXlsx()));
//     ui.changeTs->setText(tr("开始替换"));
//     connect(ui.changeTs, SIGNAL(clicked()), this, SLOT(ChangeAllOfTs()));
//     connect(ui.lineEdit_xlsx, SIGNAL(textChanged(QString)), this, SLOT(refreshHasRefresh(QString)));
//     ui.checkBox_isChange->setText(tr("是否置为全部审批"));
//     ui.checkBox_updata->setText(tr("仅生成未审批翻译"));
}

wtf::~wtf()
{
    if (excel)
    {
        delete excel;
        excel = nullptr;
    }
}

bool wtf::RemvoeTsFileVanished(const QString& strSrc, const QString& strDest)

{
    QDomDocument doc;
    QString errorMsg;
    int errorLine = 0, errorColumn = 0;
    QFile iFile(strSrc);
    iFile.open(QIODevice::ReadOnly);
    QByteArray baContent = iFile.readAll();
    bool bPase = doc.setContent(baContent, &errorMsg, &errorLine, &errorColumn);

    if (!bPase)
    {
        qDebug() << errorMsg << errorLine << errorColumn;
        return false;
    }

    iFile.close();
    QDomElement elemntTS = doc.elementsByTagName("TS").at(0).toElement();
    QDomNodeList nlContexts = elemntTS.elementsByTagName("context");

    for (int i = 0; i < nlContexts.count(); i++)
    {
        QDomNode nodeContext = nlContexts.at(i);
        QDomNodeList nlMessages = nodeContext.toElement().elementsByTagName("message");
        int nCount = nlMessages.count();

        for (int j = 0; j < nlMessages.count();)
        {
            QDomNode nodeMessage = nlMessages.at(j);
            QDomElement elementTranslation = nodeMessage.firstChildElement("translation");

            if (elementTranslation.attribute("type") == "vanished" || elementTranslation.attribute("type") == "obsolete")
            {
                nodeContext.removeChild(nodeMessage);
            }
            else
            {
                j++;
            }
        }

        nlMessages = nodeContext.toElement().elementsByTagName("message");
        nCount = nlMessages.count();
        nCount = 0;
    }

    QFile iFileWrite(strDest);

    if (!iFileWrite.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return false;

    iFileWrite.write(doc.toByteArray());
    iFileWrite.close();
    return true;
}

bool wtf::GetAllMap(const QString& strSrc)
{
    QDomDocument doc;
    QString errMsg;
    int errorLine = 0, errorColumn = 0;
    QFile iFile(strSrc);
    iFile.open(QIODevice::ReadOnly);
    QByteArray baContent = iFile.readAll();
    bool bPase = doc.setContent(baContent, &errMsg, &errorLine, &errorColumn);

    if (!bPase)
    {
        qDebug() << errMsg << errorLine << errorColumn;
        return false;
    }

    iFile.close();
    QDomElement elemntTS = doc.elementsByTagName("TS").at(0).toElement();
    QDomNodeList nlContexts = elemntTS.elementsByTagName("context");

    for (int i = 0; i < nlContexts.count(); i++)
    {
        QDomNode nodeContext = nlContexts.at(i);
        QDomNodeList nlMessages = nodeContext.toElement().elementsByTagName("message");
        int nCount = nlMessages.count();

        for (int j = 0; j < nlMessages.count(); j++)
        {
            QDomNode nodeMessage = nlMessages.at(j);
            QDomElement elementTranslation = nodeMessage.firstChildElement("source");
            QString text1 = elementTranslation.text();
            qDebug() << elementTranslation.text();
            QDomElement elementTranslation2 = nodeMessage.firstChildElement("translation");
            QString translateText = elementTranslation2.text();
            qDebug() << elementTranslation2.text();
            auto vec = allKeyValue[text1].first;
            auto maps = allKeyValue[text1].second;

            if (maps[translateText] == 0 && translateText != "")
            {
                maps[translateText] += 1;
                vec.push_back(translateText);
            }

            allKeyValue[text1] = make_pair(vec, maps);

//             if (elementTranslation2.attribute("type") == "unfinished")
//             {
//                 unApproval[text1] = true;
//             }
            if (!elementTranslation2.hasAttribute("type") || elementTranslation2.attribute("type") == "")
            {
                unApproval[text1] = true;
            }
        }

        nlMessages = nodeContext.toElement().elementsByTagName("message");
        nCount = nlMessages.count();
        nCount = 0;
    }

    qDebug() << allKeyValue.size();
    return true;
}

bool wtf::CreatXlsx(const QString& strSrc)
{
    excel->deleteFile(strSrc);
    excel->createFile(strSrc);
    int allcloun = allKeyValue.size();
    int i = 1;

    //取巧，如果保存不成功表示再外面打开该表格
    if (!excel->save())
    {
        return false;
    }

    if (!ui.checkBox_updata->isChecked())
    {
        for (auto name : allKeyValue)
        {
            auto test1 = name.first;
            auto tt = name.second.first;
            excel->setCellString(i, 1, test1);
            excel->setCellTextWrap(i, 1, true);
            excel->setAutoFitRow(i);

            if (tt.size() > 1)
            {
                excel->setCellBackground(i, 1, Qt::red);
            }

            if (!unApproval[test1])
            {
                excel->setCellBackground(i, 1, Qt::blue);
            }

            for (int j = 0; j < tt.size(); j++)
            {
                qDebug() << tt.at(j);
                excel->setCellString(i, j + 2, tt.at(j));
                excel->setCellTextWrap(i, j + 2, true);
            }

            i++;
        }
    }
    else
    {
        for (auto name : allKeyValue)
        {
            auto test1 = name.first;
            auto tt = name.second.first;

            if (!unApproval[test1])
            {
                excel->setCellString(i, 1, test1);
                excel->setCellTextWrap(i, 1, true);
                excel->setAutoFitRow(i);

                for (int j = 0; j < tt.size(); j++)
                {
                    qDebug() << tt.at(j);
                    excel->setCellString(i, j + 2, tt.at(j));
                    excel->setCellTextWrap(i, j + 2, true);
                }

                if (tt.size() > 1)
                {
                    excel->setCellBackground(i, 1, Qt::red);
                }

                i++;
            }
        }
    }

    excel->save();
    excel->quit();
    return true;
}

bool wtf::CreateXlsxForRepetition(const QString& strSrc)
{
    excel->deleteFile(strSrc);
    excel->createFile(strSrc);
    int allcloun = allKeyValue.size();
    int i = 1;

    //取巧，如果保存不成功表示再外面打开该表格
    if (!excel->save())
    {
        return false;
    }

    for (auto name : allKeyValue)
    {
        auto test1 = name.first;
        auto tt = name.second.first;

        if (tt.size() <= 1)
        {
            continue;
        }

        excel->setCellString(i, 1, test1);
        excel->setCellTextWrap(i, 1, true);
        excel->setAutoFitRow(i);

        for (int j = 0; j < tt.size(); j++)
        {
            qDebug() << tt.at(j);
            excel->setCellString(i, j + 2, tt.at(j));
            excel->setCellTextWrap(i, j + 2, true);
        }

        i++;
    }

    excel->save();
    excel->quit();
    return true;
}

bool wtf::CreatSentenceXlsx(const QString& strSrc)
{
    excel->deleteFile(strSrc);
    excel->createFile(strSrc);
    int allcloun = allKeyValue.size();
    int i = 1;

    //取巧，如果保存不成功表示再外面打开该表格
    if (!excel->save())
    {
        return false;
    }

    QRegularExpression regex(R"([.,!?;:\"'(){}\[\]\\<>]+)");

    for (auto name : allKeyValue)
    {
        auto test1 = name.first;
        QRegularExpressionMatch match = regex.match(test1);

        if (!match.hasMatch())
        {
            continue;
        }

        auto tt = name.second.first;
        excel->setCellString(i, 1, test1);
        excel->setCellTextWrap(i, 1, true);
        excel->setAutoFitRow(i);

        if (tt.size() > 1)
        {
            excel->setCellBackground(i, 1, Qt::red);
        }

        if (!unApproval[test1])
        {
            excel->setCellBackground(i, 1, Qt::blue);
        }

        for (int j = 0; j < tt.size(); j++)
        {
            qDebug() << tt.at(j);
            excel->setCellString(i, j + 2, tt.at(j));
            excel->setCellTextWrap(i, j + 2, true);
        }

        i++;
    }

    excel->save();
    excel->quit();
    return true;
}

bool wtf::KeepXlsx(const QString& strSrc)
{
//     excel->deleteFile(strSrc);
//     excel->createFile(strSrc);
    //把当前表格内容存储，不再重复插入值
    std::map<QString, std::pair< std::vector<QString>, std::map<QString, int> > > nowKeyValue;
    ReadXlsxToMap(strSrc, nowKeyValue);
    excel->open(strSrc);
    int allcloun = allKeyValue.size();
    int i = excel->getUsedRowsCount() + 1;

    for (auto name : allKeyValue)
    {
        auto test1 = name.first;
        auto tt = name.second.first;

        //如果是之前表格有的就不插入
        if (nowKeyValue.find(test1) != nowKeyValue.end())
        {
            continue;
        }

        excel->setCellString(i, 1, test1);
        excel->setCellTextWrap(i, 1, true);
        excel->setAutoFitRow(i);

        if (tt.size() > 1)
        {
            excel->setCellBackground(i, 1, Qt::red);
        }

        if (unApproval[test1])
        {
            excel->setCellBackground(i, 1, Qt::blue);
        }

        for (int j = 0; j < tt.size(); j++)
        {
            qDebug() << tt.at(j);
            excel->setCellString(i, j + 2, tt.at(j));
            excel->setCellTextWrap(i, j + 2, true);
        }

        i++;
    }

    excel->save();
    excel->quit();
    return true;
}

bool wtf::ChangeXlsxByTs(QString& strSrc)
{
    std::map<QString, std::pair< std::vector<QString>, std::map<QString, int> > > nowKeyValue;
    ReadXlsxToMap(strSrc, nowKeyValue);
    excel->open(strSrc);
    int allcloun = allKeyValue.size();
    int i = 1;

    for (auto name : allKeyValue)
    {
        auto test1 = name.first;
        auto tt = name.second.first;

        //如果是之前表格没有的就不插入
        if (nowKeyValue.find(test1) == nowKeyValue.end())
        {
            continue;
        }

        excel->setCellString(i, 1, test1);
        excel->setCellTextWrap(i, 1, true);
        excel->setAutoFitRow(i);

        for (int j = 0; j < tt.size(); j++)
        {
            qDebug() << tt.at(j);
            excel->setCellString(i, j + 2, tt.at(j));
            excel->setCellTextWrap(i, j + 2, true);
        }

        i++;
    }

    excel->save();
    excel->quit();
    return true;
}

bool wtf::ReadXlsx(const QString& strSrc)
{
    excel->open(strSrc);
    excel->selectSheet(1);
    allKeyValue.clear();
    std::map<QString, int> nullMap;
    int maxj = excel->getUsedColumnCount();
    int maxi = excel->getUsedRowsCount();

    for (int i = 1; i <= excel->getUsedRowsCount(); i++)
    {
        QVariant nowcellname = excel->getCellValue(i, 1);
        QString name = nowcellname.toString();
        std::vector<QString> allvec = allKeyValue[name].first;

        for (int j = 2; j <= maxj; j++)
        {
            QVariant nowcell = excel->getCellValue(i, j);

            if (nowcell.toString() != "")
            {
                allvec.push_back(nowcell.toString());
                qDebug() << nowcell.toString();
            }
        }

        colorForName[name] = excel->getCellBorderColor(i, 1);
        allKeyValue[name] = make_pair(allvec, nullMap);
    }

    excel->quit();
    return true;
}

bool wtf::ReadXlsxToMap(const QString& strSrc, std::map<QString, std::pair< std::vector<QString>, std::map<QString, int> > >& nowKeyValue)
{
    excel->open(strSrc);
    excel->selectSheet(1);
    nowKeyValue.clear();
    std::map<QString, int> nullMap;
    int maxj = excel->getUsedColumnCount();
    int maxi = excel->getUsedRowsCount();

    for (int i = 1; i <= excel->getUsedRowsCount(); i++)
    {
        QVariant nowcellname = excel->getCellValue(i, 1);
        QString name = nowcellname.toString();
        std::vector<QString> allvec = nowKeyValue[name].first;

        for (int j = 2; j <= maxj; j++)
        {
            QVariant nowcell = excel->getCellValue(i, j);

            if (nowcell.toString() != "")
            {
                allvec.push_back(nowcell.toString());
                qDebug() << nowcell.toString();
            }
        }

        nowKeyValue[name] = make_pair(allvec, nullMap);
    }

    excel->quit();
    return true;
}

QStringList wtf::ChangePathFile(QString filename1)
{
    QStringList anslst;
    QFileInfo file_info = filename1;

    if (QString::compare(file_info.suffix(), QString("ts"), Qt::CaseInsensitive) == 0)
    {
        anslst.append(filename1);
        return anslst;
    }

    int s = (filename1).size();

    if (filename1[s - 1] != "\\")
    {
        filename1 += "\\";
    }

    std::string files = (filename1).toStdString();
    QDir dir(filename1);

    if (!dir.exists())
    {
        return anslst;
    }

    //dir.setFilter(QDir::Files | QDir::NoSymLinks);
    QFileInfoList list = dir.entryInfoList();
    int file_count = list.count();

    if (file_count <= 0)
    {
        return anslst;
    }

    qDebug() << file_count;

    for (int i = 0; i < file_count; i++)
    {
        QFileInfo file_info = list.at(i);
        QString suffix = file_info.suffix();

        if (QString::compare(suffix, QString("ts"), Qt::CaseInsensitive) == 0)
        {
            QString absolute_file_path = file_info.absoluteFilePath();
            anslst.append(absolute_file_path);
        }
    }

    for (int i = 0; i < anslst.size(); i++)
    {
        qDebug() << anslst.at(i);
    }

    return anslst;
}

bool wtf::exchangeTS(const QString& strSrc)
{
    bool ishaveCommon = false;
    QDomDocument doc;
    QString errMsg;
    int errorLine = 0, errorColumn = 0;
    QFile iFile(strSrc);
    iFile.open(QIODevice::ReadOnly);
    QByteArray baContent = iFile.readAll();
    bool bPase = doc.setContent(baContent, &errMsg, &errorLine, &errorColumn);

    if (!bPase)
    {
        qDebug() << errMsg << errorLine << errorColumn;
        return false;
    }

    iFile.close();
    QDomElement elemntTS = doc.elementsByTagName("TS").at(0).toElement();
    QDomNodeList nlContexts = elemntTS.elementsByTagName("context");
    /*for (int i = 0; i < nlContexts.count(); i++)
    {
        QDomNode nodeContext = nlContexts.at(i);
        QDomNodeList nlMessages = nodeContext.toElement().elementsByTagName("message");
        int nCount = nlMessages.count();

        for (int j = 0; j < nlMessages.count(); j++)
        {
            QDomNode nodeMessages = nlMessages.at(j);
            QDomElement fileElem = nodeMessages.toElement();
            QString sd = fileElem.tagName();
            QDomElement firstel = nodeMessages.firstChildElement("source");
            QString sdd = firstel.tagName();
            QDomElement secondel = nodeMessages.firstChildElement("translation");
            QString sdda = secondel.tagName();
            secondel.setNodeValue("666");
        }
    }*/
    /**/
    QDomElement docElem = doc.documentElement();
    QDomNode firstNode = docElem.firstChild();   //返回根节点的第一个子节点

    while (!firstNode.isNull()) //若是节点不为空
    {
        if (firstNode.toElement().tagName() == "context")
        {
            QDomNode child_node = firstNode.firstChild();

            while (!child_node.isNull())
            {
                if (child_node.toElement().tagName() == "message")
                {
                    QDomNode child_child_node = child_node.firstChild();

                    while (!child_child_node.isNull())
                    {
                        if (child_child_node.toElement().tagName() == "source")
                        {
                            nowName = child_child_node.toElement().text();
                            qDebug() << nowName;
                        }
                        else if (child_child_node.toElement().tagName() == "translation")
                        {
                            std::vector<QString>vecans = allKeyValue[nowName].first;

                            if (vecans.size() <= 1)
                            {
                                if (vecans.size() == 1)
                                {
                                    if (child_child_node.firstChild().nodeValue() != nullptr)
                                    {
                                        //修改ts文件
                                        child_child_node.firstChild().setNodeValue(vecans.at(0));
                                    }
                                    else
                                    {
                                        //如果原本没有数值就删除之后再插入数值
                                        QDomText tes = child_child_node.firstChild().toText();
                                        child_child_node.removeChild(tes);
                                        QDomText translationText = doc.createTextNode(vecans.at(0));
                                        child_child_node.appendChild(translationText);
                                    }
                                }
                                else
                                {
                                    if (ui.checkBox_changeAll->isChecked())
                                    {
                                        child_child_node.firstChild().setNodeValue("");
                                    }

//
                                }
                            }
                            else
                            {
                                warningNub += 1;
                                ishaveCommon = true;
                            }
                        }

                        child_child_node = child_child_node.nextSibling();
                    }
                }

                child_node = child_node.nextSibling();
                nowName = nullptr;
            }
        }

        firstNode = firstNode.nextSibling();  //下一个兄弟节点
    }

    if (ishaveCommon)
    {
        showfilevec.append(iFile.fileName());
    }

    /**/
    QFile iFileWrite(strSrc);

    if (!iFileWrite.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return false;

    iFileWrite.write(doc.toByteArray());
    iFileWrite.close();
    return true;
}

bool wtf::approverTS(const QString& strSrc)
{
    bool ishaveCommon = false;
    QDomDocument doc;
    QString errMsg;
    int errorLine = 0, errorColumn = 0;
    QFile iFile(strSrc);
    iFile.open(QIODevice::ReadOnly);
    QByteArray baContent = iFile.readAll();
    bool bPase = doc.setContent(baContent, &errMsg, &errorLine, &errorColumn);

    if (!bPase)
    {
        qDebug() << errMsg << errorLine << errorColumn;
        return false;
    }

    iFile.close();
    QDomElement elemntTS = doc.elementsByTagName("TS").at(0).toElement();
    QDomNodeList nlContexts = elemntTS.elementsByTagName("context");
    QDomElement docElem = doc.documentElement();
    QDomNode firstNode = docElem.firstChild();   //返回根节点的第一个子节点

    while (!firstNode.isNull()) //若是节点不为空
    {
        if (firstNode.toElement().tagName() == "context")
        {
            QDomNode child_node = firstNode.firstChild();

            while (!child_node.isNull())
            {
                if (child_node.toElement().tagName() == "message")
                {
                    QDomNode child_child_node = child_node.firstChild();

                    while (!child_child_node.isNull())
                    {
                        if (child_child_node.toElement().tagName() == "source")
                        {
                            nowName = child_child_node.toElement().text();
                        }
                        else if (child_child_node.toElement().tagName() == "translation")
                        {
                            //将unfinished变为打勾状态
                            if (/*colorForName[nowName] != Qt::blue*/ui.checkBox_isChange->isChecked() &&
                                    child_child_node.toElement().hasAttribute("type"))
                            {
                                child_child_node.toElement().setAttribute("type", "");
                            }
                        }

                        child_child_node = child_child_node.nextSibling();
                    }
                }

                child_node = child_node.nextSibling();
                nowName = nullptr;
            }
        }

        firstNode = firstNode.nextSibling();  //下一个兄弟节点
    }

    if (ishaveCommon)
    {
        showfilevec.append(iFile.fileName());
    }

    /**/
    QFile iFileWrite(strSrc);

    if (!iFileWrite.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return false;

    iFileWrite.write(doc.toByteArray());
    iFileWrite.close();
    return true;
}
bool wtf::exchangeTsColor(const QString& strSrc)
{
    return true;
}
bool wtf::readxlsx(const QString& strSrc)
{
    excel->open(strSrc);
    excel->selectSheet(1);
    allKeyValue.clear();
    int maxj = excel->getUsedColumnCount();
    int maxi = excel->getUsedRowsCount();

    for (int i = 1; i <= excel->getUsedRowsCount(); i++)
    {
        std::vector<QString> allvec;
        std::map<QString, int> nullMap;
        QVariant nowcellname = excel->getCellValue(i, 1);
        QString name = nowcellname.toString();

        for (int j = 2; j <= maxj; j++)
        {
            QVariant nowcell = excel->getCellValue(i, j);

            if (nowcell.toString() != "")
            {
                nullMap[nowcell.toString()] = true;
                allvec.push_back(nowcell.toString());
                qDebug() << nowcell.toString();
            }
        }

        colorForName[name] = excel->getCellBorderColor(i, 1);
        allKeyValue[name] = make_pair(allvec, nullMap);
    }

    excel->quit();
    return true;
}
void wtf::initTips()
{
    ui.trr->setText(tr("..."));
    ui.popo->setText(tr("..."));
    ui.okk->setText(tr("..."));
    ui.in->setText(tr("..."));
    ui.out->setText(tr("..."));
    ui.label_gray->setText(tr("如果输入文件夹绝对路径则会遍历文件夹本层内可修改的文件\n\n\n将目标文件的绝对路径输入，原ts文件将去除不可用的翻译（原本翻译移动位置后会导致翻译不可用）"));
    ui.testforgray->setText(tr("目标文件的绝对路径"));
    ui.clear_Gray->setText(tr("去灰"));
    ui.label_toxlsx->setText(tr("将目标文件的绝对路径输入，在保存位置下生成拥有所有ts的excel表格，配合生成qph使用。\n后续翻译仅需维护表格即可\nPS:如果全收集的华标红为多种翻译，标蓝为未审批"));
    ui.testforxlsx->setText(tr("目标文件的绝对路径"));
    ui.label_outputpath->setText(tr("保存表格位置"));
    ui.creatXlsx->setText(tr("重新创建表格"));
    ui.KeepXlsx->setText(tr("继续生成表格"));
    ui.btn_changeXlsx->setText(tr("更新表格项目"));
    ui.label_toqph->setText(tr("根据输入表格来修改ts文件起到自动化作用\nPS：如果表格有多种翻译且ts出现该翻译会提示"));
    ui.testforqph->setText(tr("表格位置"));
    ui.label_ChangeTs->setText(tr("需要修改的ts文件"));
    ui.refreshXls->setText(tr("表格更改"));
    ui.changeTs->setText(tr("开始替换"));
    ui.checkBox_isChange->setText(tr("是否置为全部审批"));
    ui.checkBox_updata->setText(tr("仅生成未审批翻译"));
    ui.checkBox_isrepetition->setText(tr("仅生成多种翻译"));
    ui.checkBox_changeAll->setText(tr("全表格替换（空的也替换）"));
    ui.checkBox_sentence->setText(tr("筛选句子"));
}
void wtf::initConnect()
{
    connect(ui.trr, SIGNAL(clicked()), this, SLOT(showTSDialogtrr()));
    connect(ui.popo, SIGNAL(clicked()), this, SLOT(showTSDialogpopo()));
    connect(ui.okk, SIGNAL(clicked()), this, SLOT(showTSDialogok()));
    connect(ui.in, SIGNAL(clicked()), this, SLOT(showTSDialogin()));
    connect(ui.out, SIGNAL(clicked()), this, SLOT(showTSDialogout()));
    connect(ui.clear_Gray, SIGNAL(clicked()), this, SLOT(CLearGray()));
    connect(ui.creatXlsx, SIGNAL(clicked()), this, SLOT(CreatXlsxForNew()));
    connect(ui.KeepXlsx, SIGNAL(clicked()), this, SLOT(KeepFixXlsx()));
    connect(ui.btn_changeXlsx, SIGNAL(clicked()), this, SLOT(ReplaceXlsxByTs()));
    connect(ui.refreshXls, SIGNAL(clicked()), this, SLOT(refreshXlsx()));
    connect(ui.changeTs, SIGNAL(clicked()), this, SLOT(ChangeAllOfTs()));
    connect(ui.lineEdit_xlsx, SIGNAL(textChanged(QString)), this, SLOT(refreshHasRefresh(QString)));
    connect(ui.checkBox_isrepetition, SIGNAL(clicked(bool)), this, SLOT(RepetitionClicked(bool)));
    connect(ui.checkBox_sentence, SIGNAL(clicked()), this, SLOT(FilterSentence()));
}
void wtf::CLearGray()
{
    QString filePath = ui.lineEdit_gray->text();

    if (filePath == "")
    {
        QMessageBox::warning(this, tr("警告"), tr("目标文件路径为空"));
        return;
    }

    QStringList listall = ChangePathFile(filePath);
    qDebug() << filePath;

    if (listall.size() == 0)
    {
        QMessageBox::warning(this, tr("警告"), tr("该文件夹没有ts文件"));
        return;
    }

    for (auto path : listall)
    {
        RemvoeTsFileVanished(path, path);
    }

    QMessageBox::question(this, "提示信息", "该文件已去除不必要翻译");
}
void wtf::CreatXlsxForNew()
{
    QString filePath = ui.lineEdit_ts->text();
    QString OutPutPath = ui.lineEdit_saveXlsx->text();

    if (filePath == "" || OutPutPath == "")
    {
        QMessageBox::warning(this, tr("警告"), tr("目标文件路径或保存路径为空"));
        return;
    }

    QStringList listall = ChangePathFile(filePath);

    if (listall.size() == 0)
    {
        QMessageBox::warning(this, tr("警告"), tr("该文件夹没有ts文件"));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    unApproval.clear();
    allKeyValue.clear();

    for (auto path : listall)
    {
        GetAllMap(path);
    }

    bool sueesed;

    if (ui.checkBox_isrepetition->isChecked())
    {
        sueesed = CreateXlsxForRepetition(OutPutPath);
    }
    else
    {
        sueesed = CreatXlsx(OutPutPath);
    }

    QApplication::restoreOverrideCursor();

    if (!sueesed)
    {
        QMessageBox::warning(this, "警告", "表格生成失败，该表格正在使用中");
        return;
    }

    QMessageBox::question(this, "提示信息", "表格已经重新生成");
    hasRefresh = false;
}
void wtf::KeepFixXlsx()
{
    QString filePath = ui.lineEdit_ts->text();
    QString OutPutPath = ui.lineEdit_saveXlsx->text();

    if (filePath == "" || OutPutPath == "")
    {
        QMessageBox::warning(this, tr("警告"), tr("目标文件路径或保存路径为空"));
        return;
    }

    QFile files(OutPutPath);

    //如果没有找到这个表格默认自动重新生成
    if (!files.open(QIODevice::ReadOnly))
    {
        CreatXlsxForNew();
        return;
    }

    QStringList listall = ChangePathFile(filePath);

    if (listall.size() == 0)
    {
        QMessageBox::warning(this, tr("警告"), tr("该文件夹没有ts文件"));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    unApproval.clear();
    allKeyValue.clear();
    readxlsx(OutPutPath);

    for (auto path : listall)
    {
        GetAllMap(path);
    }

    //CreatXlsx(OutPutPath);
    KeepXlsx(OutPutPath);
    QApplication::restoreOverrideCursor();
    QMessageBox::question(this, "提示信息", "新表格已经生成");
    unApproval.clear();
    allKeyValue.clear();
}

void wtf::ReplaceXlsxByTs()
{
    QString filePath = ui.lineEdit_ts->text();
    QString OutPutPath = ui.lineEdit_saveXlsx->text();

    if (filePath == "" || OutPutPath == "")
    {
        QMessageBox::warning(this, tr("警告"), tr("目标文件路径或保存路径为空"));
        return;
    }

    QFile files(OutPutPath);

    //如果没有找到这个表格默认自动重新生成
    if (!files.open(QIODevice::ReadOnly))
    {
        CreatXlsxForNew();
        return;
    }

    QStringList listall = ChangePathFile(filePath);

    if (listall.size() == 0)
    {
        QMessageBox::warning(this, tr("警告"), tr("该文件夹没有ts文件"));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    unApproval.clear();
    allKeyValue.clear();
    //readxlsx(OutPutPath);

    for (auto path : listall)
    {
        GetAllMap(path);
    }

    //CreatXlsx(OutPutPath);
    ChangeXlsxByTs(OutPutPath);
    QApplication::restoreOverrideCursor();
    QMessageBox::question(this, "提示信息", "表格已经更新");
    unApproval.clear();
    allKeyValue.clear();
}

void wtf::showTSDialogtrr()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("请选择要操作的文件"), "C:/", tr("xml(*.ts *.qph)"));

    if (fileName.isEmpty())
    {
        return;
        QMessageBox::warning(this, "Warning!", "Failed to open the video!");
    }
    else
    {
        ui.lineEdit_gray->setText(fileName);
    }
}
void wtf::showTSDialogpopo()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("请选择要操作的文件"), "C:/", tr("xml(*.ts *.qph)"));

    if (fileName.isEmpty())
    {
        return;
        QMessageBox::warning(this, "Warning!", "Failed to open the video!");
    }
    else
    {
        ui.lineEdit_ts->setText(fileName);
    }
}
void wtf::showTSDialogok()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("请选择表格保存位置"), "C:/", "excel(*.xls *.xlsx)");

    //将返回一个由用户选择的文件名。该文件不一定存在。
    if (fileName.isEmpty())
    {
        return;
        QMessageBox::warning(this, tr("Warning!"), tr("Failed to open the video!"));
    }
    else
    {
        ui.lineEdit_saveXlsx->setText(fileName);
    }
}
void wtf::showTSDialogin()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("请选择要读取的表格"), "C:/", tr("excel(*.xls *.xlsx)"));

    if (fileName.isEmpty())
    {
        return;
        QMessageBox::warning(this, tr("Warning!"), tr("Failed to open the video!"));
    }
    else
    {
        ui.lineEdit_xlsx->setText(fileName);
    }
}
void wtf::showTSDialogout()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("请选择要操作的文件"), "C:/", tr("xml(*.ts)"));

    if (fileName.isEmpty())
    {
        return;
        QMessageBox::warning(this, tr("Warning!"), tr("Failed to open the video!"));
    }
    else
    {
        ui.lineEdit_finalTs->setText(fileName);
    }
}
void wtf::ChangeAllOfTs()
{
    QString filePath = ui.lineEdit_xlsx->text();
    QString OutPutPath = ui.lineEdit_finalTs->text();

    if (filePath == "" || OutPutPath == "")
    {
        QMessageBox::warning(this, tr("警告"), tr("表格文件路径或Ts路径为空"));
        return;
    }

    QStringList listall = ChangePathFile(OutPutPath);

    if (listall.size() == 0)
    {
        QMessageBox::warning(this, tr("警告"), tr("该文件夹没有ts文件"));
        return;
    }

    if (!hasRefresh)
    {
        unApproval.clear();
        allKeyValue.clear();
        ReadXlsx(filePath);
        hasRefresh = true;
    }

    for (auto path : listall)
    {
        exchangeTS(path);

        if (ui.checkBox_isChange->isChecked())
        {
            approverTS(path);
        }
    }

    if (warningNub)
    {
        QMessageBox::question(this, tr("提示"), tr("TS文件已根据表格校对，%1有多个翻译，请自行判断").arg(warningNub));
        warningNub = 0;
        QString showFile = "";

        for (const QString& str : showfilevec)
        {
            showFile += str + "\n";
        }

        QMessageBox msgBox;
        msgBox.resize(300, 100);
        msgBox.setText("详细信息");
        msgBox.setDetailedText(showFile);
        msgBox.exec();
        showfilevec.clear();
    }
    else
    {
        QMessageBox::question(this, tr("提示"), tr("TS文件已根据表格校对完毕"));
    }
}
void wtf::refreshXlsx()
{
    QString filePaths = ui.lineEdit_xlsx->text();

    if (filePaths == "" )
    {
        QMessageBox::warning(this, tr("警告"), tr("表格文件路径为空"));
        return;
    }

    unApproval.clear();
    allKeyValue.clear();
    QString filePath = ui.lineEdit_xlsx->text();
    ReadXlsx(filePath);
    hasRefresh = true;
}
void wtf::refreshHasRefresh(QString)
{
    unApproval.clear();
    allKeyValue.clear();
    hasRefresh = false;
}
void wtf::RepetitionClicked(bool isclicked)
{
    if (isclicked)
        ui.checkBox_updata->setChecked(!isclicked);

    ui.checkBox_updata->setCheckable(!isclicked);
}

void wtf::FilterSentence()
{
    QString filePath = ui.lineEdit_ts->text();
    QString OutPutPath = ui.lineEdit_saveXlsx->text();

    if (filePath == "" || OutPutPath == "")
    {
        QMessageBox::warning(this, tr("警告"), tr("目标文件路径或保存路径为空"));
        return;
    }

    QStringList listall = ChangePathFile(filePath);

    if (listall.size() == 0)
    {
        QMessageBox::warning(this, tr("警告"), tr("该文件夹没有ts文件"));
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);
    unApproval.clear();
    allKeyValue.clear();

    for (auto path : listall)
    {
        GetAllMap(path);
    }

    bool sueesed;
    sueesed = CreatSentenceXlsx(OutPutPath);
    QApplication::restoreOverrideCursor();

    if (!sueesed)
    {
        QMessageBox::warning(this, "警告", "表格生成失败，该表格正在使用中");
        return;
    }

    QMessageBox::question(this, "提示信息", "表格已经重新生成");
    hasRefresh = false;
}
