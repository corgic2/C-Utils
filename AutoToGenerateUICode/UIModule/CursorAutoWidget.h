#pragma once

#include <QMainWindow>

#include <qprocess.h>
#include <qstandarditemmodel.h>

#include "ui_CursorAutoWidget.h"

class QMessageBox;
QT_BEGIN_NAMESPACE
namespace Ui
{
class CursorAutoWidgetClass;
};
QT_END_NAMESPACE

class CursorAutoWidget : public QMainWindow
{
    Q_OBJECT

  public:
    CursorAutoWidget(QWidget *parent = nullptr);
    ~CursorAutoWidget();
    void on_pushButton_4Clicked();
    void loadCursorPathFromConfig();
    void saveCursorPathToConfig(const QString &cursorPath);
    void on_pushButtonClicked();
    void on_pushButton_5Clicked();
    void on_pushButton_3Clicked();
    void on_pushButton_2ClickedForTab2();
    void on_pushButton_2Clicked();
    void on_pushButton_2ClickedForTab1();
    void on_pushButton_6Clicked();
    void on_pushButton_7Clicked();
    void on_pushButton_8Clicked();

  private:
    Ui::CursorAutoWidgetClass *ui;
    QProcess *m_cursorProcess = nullptr;
    QStandardItemModel *m_model = nullptr;
    QStringList m_fileNames;
    QMessageBox *m_msg = nullptr;

};
