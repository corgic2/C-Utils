#ifndef GETCOMPONENTSTYLESHEET_H
#define GETCOMPONENTSTYLESHEET_H
#include <QMap>
#include <QPluginLoader>
#include <QWidget>

#include "ChooseMtType.h"

namespace Ui
{
class GetComponentStyleSheetClass;
}

class GetComponentStyleSheet : public QWidget
{
    Q_OBJECT

  public:
    explicit GetComponentStyleSheet(QWidget *parent = nullptr);
    //初始化测试用例Map
    void InitComponent();
    ~GetComponentStyleSheet();
    //获取Widget的text属性信息
    QStringList GetTypeData(QWidget* widget, const QString& text);
    //组件对象改变
    void onWidgetChanged(int index);
    //获取组件中包含Type的所有属性
    bool InitType(QWidget* widget, const QStringList& listType);
    //获取某个组件的Type内容
    QStringList GetTypeProperty(QWidget* obj);
    //清除当前显示组件
    void ClearView();
    //生成内容
    void onGenerateClicked(const QString& text);
signals:
    void SigShow(const QString &text);
private:
    Ui::GetComponentStyleSheetClass *ui;
    QMap<QString, QWidget *> contentMap; // 用于管理动态创建的 Content 组件
    ChooseMtType* m_chooseType;
};

#endif // GETCOMPONENTSTYLESHEET_H