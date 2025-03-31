#include "GetComponentStyleSheet.h"
#include <QMessageBox>
#include <QMetaObject>
#include <QDir>
#include <qDebug>
#include <QMetaObject>
#include <QMetaProperty>
#include <QPluginLoader>
#include <QPushButton>
#include <QVBoxLayout>
#include "ChooseMtType.h"
#include "ui_GetComponentStyleSheet.h"

GetComponentStyleSheet::GetComponentStyleSheet(QWidget *parent) : QWidget(parent), ui(new Ui::GetComponentStyleSheetClass())
{
    ui->setupUi(this);
    m_chooseType = new ChooseMtType;
    // 初始化 frame 的布局
    if (!ui->frame->layout())
    {
        ui->frame->setLayout(new QVBoxLayout());
    }
    ui->verticalLayout_2->addWidget(m_chooseType);

    InitComponent();
    // 初始化 Widget 的 ComboBox
    for (const auto &key : contentMap.keys())
    {
        ui->comboBox->addItem(key);
    }
    // 默认显示第一个
    // onWidgetChanged(0);
    // 连接信号和槽
    connect(ui->comboBox, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &GetComponentStyleSheet::onWidgetChanged);
    connect(m_chooseType,&ChooseMtType::SigChanged,this,&GetComponentStyleSheet::SigShow);
    connect(this, &GetComponentStyleSheet::SigShow, this, &GetComponentStyleSheet::onGenerateClicked);
}
void GetComponentStyleSheet::InitComponent()
{
    // 添加自定义组组件
    // contentMap.insert("QCalendarWidget", new MtCalendarWidget(nullptr));
    //添加测试用例
    //auto tmp0 = dynamic_cast<QCalendarWidget *>(contentMap["MtCalendarWidget"]);
}
GetComponentStyleSheet::~GetComponentStyleSheet()
{
    // 清空 frame 中的所有组件
    ClearView();
    // 删除 contentMap 中的所有控件
    for (auto *widget : contentMap.values())
    {
        delete widget;
    }
    delete ui;
}
QStringList GetComponentStyleSheet::GetTypeData(QWidget *widget, const QString &text)
{
    QStringList listTypeForOne;
    if (nullptr == widget)
    {
        return listTypeForOne;
    }
    // 获取 自定义 属性的值
    if (widget)
    {
        QMetaObject metaObject = *widget->metaObject();
        int mtTypePropertyIndex = metaObject.indexOfProperty(text.toStdString().c_str());

        if (mtTypePropertyIndex != -1)
        {
            QMetaProperty mtTypeProperty = metaObject.property(mtTypePropertyIndex);
            QMetaEnum mtTypeEnum = mtTypeProperty.enumerator();

            for (int i = 0; i < mtTypeEnum.keyCount(); ++i)
            {
                QString mtTypeName = mtTypeEnum.key(i);
                listTypeForOne<<mtTypeName;
            }
        }
    }
    return listTypeForOne;
}

void GetComponentStyleSheet::onWidgetChanged(int index)
{
    m_chooseType->ClearAllTypes();
    ClearView();

    QString widgetName = ui->comboBox->currentText(); // 获取选中的 Widget 名称
    QWidget *widget = contentMap.value(widgetName); // 获取对应的 QWidget 对象
    QStringList listType = GetTypeProperty(widget);
    if (false == InitType(widget,listType))
    {
        onGenerateClicked("");
    }
}

bool GetComponentStyleSheet::InitType(QWidget* widget, const QStringList& listType)
{
    if (nullptr == widget)
    {
        return false;
    }

    bool bContains = false;
    for (auto& tmp:listType)
    {
        QStringList listTemp = GetTypeData(widget, tmp);
        if (listTemp.size() > 0)
        {
            m_chooseType->AddType(tmp, listTemp);
            bContains = true;
        }
    }
    return bContains;
}

QStringList GetComponentStyleSheet::GetTypeProperty(QWidget* obj)
{
    QStringList typeLists;
    if (obj == nullptr)
    {
        return typeLists;
    }
    const QMetaObject *metaObject = obj->metaObject();
    int propertyCount = metaObject->propertyCount();
    for (int i = metaObject->propertyOffset(); i < propertyCount; ++i)
    {
        QMetaProperty metaProperty = metaObject->property(i);
        const char *propertyName = metaProperty.name();// 获取属性名称
        const char *propertyType = metaProperty.typeName();
        bool containsType = QString(propertyType).contains("Type");// 获取属性类型
        QVariant propertyValue = metaProperty.read(obj);// 获取属性值
        if (containsType)
        {
            typeLists<<propertyName;
        }
    }
    return typeLists;
}

void GetComponentStyleSheet::ClearView()
{
    // 清空 Frame
    QLayoutItem *child;
    while ((child = ui->frame->layout()->takeAt(0)) != nullptr)
    {
        QWidget *widget = child->widget();
        if (widget)
        {
            widget->hide(); // 隐藏组件
        }
        delete child; // 删除布局项
    }
}

void GetComponentStyleSheet::onGenerateClicked(const QString& text)
{
    ClearView();
    // 获取选中的 Widget 名称和 MtType 名称
    QString widgetName = ui->comboBox->currentText();
    // 获取对应的 QWidget 对象
    QWidget *widget = contentMap.value(widgetName);

    if (widget)
    {
        bool flag = false;
        for (auto &tmp:m_chooseType->m_map.keys())
        {
            flag = true;
            // 获取 Type 属性的值
            QMetaObject metaObject = *widget->metaObject();
            int mtTypePropertyIndex = metaObject.indexOfProperty(tmp.toStdString().c_str());

            if (mtTypePropertyIndex != -1)
            {
                QMetaProperty mtTypeProperty = metaObject.property(mtTypePropertyIndex);
                QMetaEnum mtTypeEnum = mtTypeProperty.enumerator();

                // 设置 Type
                int mtTypeValue = mtTypeEnum.keyToValue(m_chooseType->m_map[tmp]->currentText().toUtf8().constData());
                mtTypeProperty.write(widget, mtTypeValue);

                // 将 Widget 添加到 Frame
                QVBoxLayout *layout = static_cast<QVBoxLayout *>(ui->frame->layout());
                layout->addWidget(widget);
                widget->show(); // 显示组件
            }
        }
        if (!flag)
        {
            // 将 Widget 添加到 Frame
            QVBoxLayout *layout = static_cast<QVBoxLayout *>(ui->frame->layout());
            layout->addWidget(widget);
            widget->show(); // 显示组件
        }
    }
}