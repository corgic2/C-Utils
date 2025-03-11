#include "CursorAutoWidget.h"
#include <QAbstractItemView>
#include <QApplication>
#include <QClipboard>
#include <QFileDialog>
#include <QMessageBox>
#include <QProcess>
#include <QSettings>
#include <QTimer>
#include <qDebug>

#include <qt_windows.h>

#include "ui_CursorAutoWidget.h"

#pragma execution_character_set("utf-8")
CursorAutoWidget::CursorAutoWidget(QWidget *parent) : QMainWindow(parent), ui(new Ui::CursorAutoWidgetClass()), m_model(new QStandardItemModel(0, 2, this)), m_cursorProcess(new QProcess(this))
{
    ui->setupUi(this);
    ui->listView->setModel(m_model);
    ui->listView->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(ui->pushButton, &QPushButton::clicked, this, &CursorAutoWidget::on_pushButtonClicked);
    connect(ui->pushButton_2, &QPushButton::clicked, this, &CursorAutoWidget::on_pushButton_2Clicked);
    connect(ui->pushButton_3, &QPushButton::clicked, this, &CursorAutoWidget::on_pushButton_3Clicked);
    connect(ui->pushButton_4, &QPushButton::clicked, this, &CursorAutoWidget::on_pushButton_4Clicked);
    connect(ui->pushButton_5, &QPushButton::clicked, this, &CursorAutoWidget::on_pushButton_5Clicked);
    connect(ui->pushButton_6, &QPushButton::clicked, this, &CursorAutoWidget::on_pushButton_6Clicked);
    connect(ui->pushButton_7, &QPushButton::clicked, this, &CursorAutoWidget::on_pushButton_7Clicked);
    connect(ui->pushButton_8, &QPushButton::clicked, this, &CursorAutoWidget::on_pushButton_8Clicked);

    loadCursorPathFromConfig();
    ui->pushButton_5->hide();
}
// 回调函数，用于枚举窗口
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam)
{
    // 获取窗口标题
    TCHAR title[256];
    GetWindowText(hwnd, title, sizeof(title) / sizeof(TCHAR));

    // 跳过没有标题的窗口
    if (title[0] == 0)
    {
        return TRUE;
    }

    // 将 TCHAR 转换为 wchar_t
    wchar_t *wTitle = reinterpret_cast<wchar_t *>(title);

#ifdef UNICODE
    // 如果启用了 Unicode 编译，直接使用宽字符
    QString qTitle = QString::fromWCharArray(title);
#else
    // 如果未启用 Unicode 编译，使用多字节字符
    QString qTitle = QString::fromLocal8Bit(title);
    if (qTitle.count("Cursor"))
    {
        SetForegroundWindow(hwnd);
        return FALSE;
    }
#endif

    qDebug() << "Window Title:" << qTitle;

    // 如果需要，可以在这里添加逻辑来查找特定的窗口标题
    // 例如：
    // if (lstrcmpi(title, L"目标应用程序窗口标题") == 0) {
    //     *(HWND *)lParam = hwnd;
    //     return FALSE; // 停止枚举
    // }

    return TRUE;
}
CursorAutoWidget::~CursorAutoWidget()
{
    delete ui;
}
// 新增槽函数
void CursorAutoWidget::on_pushButton_4Clicked()
{
    // 清除模型中的所有行
    while (m_model->rowCount() > 0)
    {
        m_model->removeRow(0);
    }

    // 可选：清空文件名列表（如果需要）
    m_fileNames.clear();
}
void CursorAutoWidget::loadCursorPathFromConfig()
{
    QString configFilePath = QApplication::applicationDirPath() + "/AutoCursor.ini";
    QSettings settings(configFilePath, QSettings::IniFormat);
    settings.beginGroup("Cursor");
    QString cursorPath = settings.value("CursorPath").toString();
    if (!cursorPath.isEmpty())
    {
        ui->lineEdit->setText(cursorPath);
    }
    settings.endGroup();
}

void CursorAutoWidget::saveCursorPathToConfig(const QString &cursorPath)
{
    QString configFilePath = QApplication::applicationDirPath() + "/AutoCursor.ini";
    QSettings settings(configFilePath, QSettings::IniFormat);
    settings.beginGroup("Cursor");
    settings.setValue("CursorPath", cursorPath);
    settings.endGroup();
}

void CursorAutoWidget::on_pushButtonClicked()
{
    QStringList fileNames = QFileDialog::getOpenFileNames(this, tr("选择文件"), "", tr("所有文件 (*.*)"));
    foreach (const QString &fileName, fileNames)
    {
        QFileInfo fileInfo(fileName);
        m_model->appendRow(new QStandardItem(fileInfo.fileName()));
    }
}

void CursorAutoWidget::on_pushButton_5Clicked()
{
    QString dirPath = QFileDialog::getExistingDirectory(this, "选择目录");
    QString dirName = dirPath.right(dirPath.size() - dirPath.lastIndexOf("/") - 1);
    m_fileNames.append(dirName);
    m_model->appendRow(new QStandardItem(dirName));
}

void CursorAutoWidget::on_pushButton_3Clicked()
{
    QString cursorPath = QFileDialog::getOpenFileName(this, tr("选择Cursor程序"), "", tr("可执行文件 (*.exe)"));
    ui->lineEdit->setText(cursorPath);
    if (!cursorPath.isEmpty())
    {
        saveCursorPathToConfig(cursorPath);
    }
}

void CursorAutoWidget::on_pushButton_2Clicked()
{
    if (ui->tabWidget->currentIndex() == 0) // Tab1
    {
        on_pushButton_2ClickedForTab1();
    }
    else if (ui->tabWidget->currentIndex() == 1) // Tab2
    {
        on_pushButton_2ClickedForTab2();
    }
}
void CursorAutoWidget::on_pushButton_2ClickedForTab1()
{
    if (ui->textEdit->toPlainText().isEmpty() || ui->textEdit_2->toPlainText().isEmpty())
    {
        QMessageBox::warning(this, tr("警告"), tr("HTML和CSS源码不能为空!"));
        return;
    }

    QString htmlCode = ui->textEdit->toPlainText();
    QString cssCode = ui->textEdit_2->toPlainText();

    QString curPath = QApplication::applicationFilePath();
    QDir curdir(curPath);
    curdir.cdUp();
    QString filePath = curdir.absolutePath() + "/HtmlToQtuiInCursor.txt";
    QString outputFilePath = curdir.absolutePath() + "/Autoui.txt";

    QFile inputFile(filePath);
    QFile outputFile(outputFilePath);
    m_fileNames.clear();
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text) && inputFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        outputFile.write("\xEF\xBB\xBF");

        QTextStream outTemplate(&inputFile);
        QString templateStr = outTemplate.readAll();
        QTextStream out(&outputFile);
        out.setCodec("UTF-8");
        out << templateStr;
        for (int i = 0; i < m_model->rowCount(); i++)
        {
            m_fileNames.append(m_model->item(i)->text());
        }

        if (!m_fileNames.empty())
        {
            out << tr("\n可能需要使用到的组件有：\n");
        }
        for (int i = 0; i < m_fileNames.size(); ++i)
        {
            out << QString::number(i + 1) << ". " << m_fileNames[i] << "\n";
        }

        out << tr("\n源代码：\n");
        out << tr("HTML部分： ") << htmlCode << "\n";
        out << tr("CSS部分： ") << cssCode << "\n";

        outputFile.close();
    }

    QString cursorPath = ui->lineEdit->text();
    if (!cursorPath.isEmpty())
    {
        if (FALSE == EnumWindows(EnumWindowsProc, 0))
        {
        }
        else
        {
            QEventLoop loop;
            QTimer time;
            time.setSingleShot(true);
            connect(&time, &QTimer::timeout, &loop, &QEventLoop::quit);
            m_cursorProcess->startDetached(cursorPath);
            time.start(5000);
            loop.exec();
        }
        // 调用 Python 脚本进行窗口操作和键盘模拟
        QStringList arguments;
        arguments << "-u" << QApplication::applicationDirPath() + "/cursor_operations.py" << m_fileNames.join("\n") << outputFilePath;
        QProcess::startDetached("python", arguments);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("请先选择Cursor程序路径!"));
    }
}
void CursorAutoWidget::on_pushButton_2ClickedForTab2()
{
    QString curPath = QApplication::applicationFilePath();
    QDir curdir(curPath);
    curdir.cdUp();
    QString templatePath = curdir.absolutePath() + "/UiFunction_template.txt";
    QString outputFilePath = curdir.absolutePath() + "/AutouiFunction.txt";

    QFile outputFile(outputFilePath);
    if (outputFile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        outputFile.write("\xEF\xBB\xBF");

        QTextStream out(&outputFile);
        out.setCodec("UTF-8");

        // 读取模板文件
        QFile templateFile(templatePath);
        if (templateFile.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            QTextStream in(&templateFile);
            in.setCodec("UTF-8");
            QString templateContent = in.readAll();
            out << templateContent;
            templateFile.close();
        }

        // 追加内容
        m_fileNames.clear();
        for (int i = 0; i < m_model->rowCount(); i++)
        {
            m_fileNames.append(m_model->item(i)->text());
        }

        if (!m_fileNames.isEmpty())
        {
            out << tr("\n可能需要使用到的组件有：\n");
            for (int i = 0; i < m_fileNames.size(); ++i)
            {
                out << QString::number(i + 1) << ". " << m_fileNames[i] << "\n";
            }
        }

        out << tr("\n组件功能：\n");
        out << ui->textEdit_3->toPlainText() << "\n";

        out << tr("\n组件样式设置：\n");
        out << ui->textEdit_5->toPlainText() << "\n";

        out << tr("\n原子组件交互功能：\n");
        out << ui->textEdit_4->toPlainText() << "\n";

        outputFile.close();
    }

    QString cursorPath = ui->lineEdit->text();
    if (!cursorPath.isEmpty())
    {
        if (FALSE == EnumWindows(EnumWindowsProc, 0))
        {
        }
        else
        {
            QEventLoop loop;
            QTimer time;
            time.setSingleShot(true);
            connect(&time, &QTimer::timeout, &loop, &QEventLoop::quit);
            m_cursorProcess->startDetached(cursorPath);
            time.start(5000);
            loop.exec();
        }
        // 调用 Python 脚本进行窗口操作和键盘模拟
        QStringList arguments;
        arguments << "-u" << QApplication::applicationDirPath() + "/cursor_operations.py" << m_fileNames.join("\n") << outputFilePath;
        QProcess::startDetached("python", arguments);
    }
    else
    {
        QMessageBox::warning(this, tr("警告"), tr("请先选择Cursor程序路径!"));
    }
}

void CursorAutoWidget::on_pushButton_6Clicked()
{
    if (ui->textEdit_3->isEnabled())
    {
        ui->textEdit_3->setDisabled(true);
        ui->pushButton_6->setText(tr("设置可用"));
    }
    else
    {
        ui->textEdit_3->setEnabled(true);
        ui->pushButton_6->setText(tr("设置不可用"));
    }
}

void CursorAutoWidget::on_pushButton_7Clicked()
{
    if (ui->textEdit_4->isEnabled())
    {
        ui->textEdit_4->setDisabled(true);
        ui->pushButton_7->setText(tr("设置可用"));
    }
    else
    {
        ui->textEdit_4->setEnabled(true);
        ui->pushButton_7->setText(tr("设置不可用"));
    }
}

void CursorAutoWidget::on_pushButton_8Clicked()
{
    if (ui->textEdit_5->isEnabled())
    {
        ui->textEdit_5->setDisabled(true);
        ui->pushButton_8->setText(tr("设置可用"));
    }
    else
    {
        ui->textEdit_5->setEnabled(true);
        ui->pushButton_8->setText(tr("设置不可用"));
    }
}