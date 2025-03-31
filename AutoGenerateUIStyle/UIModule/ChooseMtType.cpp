#include "ChooseMtType.h"

#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

ChooseMtType::ChooseMtType(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::ChooseMtTypeClass())
{
    ui->setupUi(this);
    // 获取主窗口的垂直布局
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout *>(ui->verticalLayout_2);
    if (!mainLayout)
    {
        mainLayout = new QVBoxLayout(ui->frame);
        mainLayout->setSpacing(0);
        mainLayout->setContentsMargins(0, 0, 0, 0);
        ui->frame->setLayout(mainLayout);
    }
}

ChooseMtType::~ChooseMtType()
{
    delete ui;
}
void ChooseMtType::AddType(const QString &type, const QStringList &stringList)
{
    // 创建水平布局
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->setSpacing(0);
    hLayout->setContentsMargins(0, 0, 0, 0);

    // 创建Label
    QLabel *label = new QLabel(type);
    hLayout->addWidget(label);
    label->setFixedHeight(30);

    // 创建QComboBox
    QComboBox *comboBox = new QComboBox;
    comboBox->addItems(stringList);
    hLayout->addWidget(comboBox);
    comboBox->setFixedHeight(30);

    // 将水平布局添加到主窗口的垂直布局中
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout *>(ui->verticalLayout_2);
    mainLayout->addLayout(hLayout);

    // 连接信号
    connect(comboBox,&QComboBox::currentTextChanged,this,&ChooseMtType::SigChanged);
    m_map.insert(type,comboBox);
    m_mapLabel.insert(type, label);
    resize(150,++cnt * 30 + 50);
}

void ChooseMtType::ClearAllTypes()
{
    QVBoxLayout *mainLayout = qobject_cast<QVBoxLayout *>(ui->verticalLayout_2);
    for (auto tmp:m_map.values())
    {
        delete tmp;
        tmp = nullptr;
    }
    for (auto tmp : m_mapLabel.values())
    {
        delete tmp;
        tmp = nullptr;
    }
    cnt -= m_map.size();
    m_map.clear();
    m_mapLabel.clear();
    resize(150, 50);

    
}