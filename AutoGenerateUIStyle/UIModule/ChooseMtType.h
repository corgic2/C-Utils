#pragma once

#include <QComboBox>
#include <QLabel>
#include <QWidget>
#include "ui_ChooseMtType.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ChooseMtTypeClass; };
QT_END_NAMESPACE

class ChooseMtType : public QWidget
{
    Q_OBJECT

public:
    ChooseMtType(QWidget *parent = nullptr);
    ~ChooseMtType();
    void AddType(const QString& type, const QStringList& stringList);
    void ClearAllTypes();
    QMap<QString, QComboBox *>m_map;
    QMap<QString,QLabel*>m_mapLabel;
signals:
    void SigChanged(const QString& text);
private:
    Ui::ChooseMtTypeClass *ui;
    int cnt = 0;
};
