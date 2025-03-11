#include "PathListDialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QListWidget>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>

CPathListDialog::CPathListDialog(const QStringList& pathList, QWidget *parent)
    : QDialog(parent)
{
    setWindowTitle(u8"选择一个文件夹");
    //添加控件
    QVBoxLayout* pLayout = new QVBoxLayout();
    setLayout(pLayout);
    m_pListWidget = new QListWidget();
    pLayout->addWidget(m_pListWidget);
    QHBoxLayout* pButtonsLayout = new QHBoxLayout();
    pLayout->addLayout(pButtonsLayout);
    QPushButton* addBtn = new QPushButton(u8"打开新文件夹");
    pButtonsLayout->addWidget(addBtn);
    connect(addBtn, SIGNAL(clicked()), this, SLOT(OnOpenNewFolder()));
    QDialogButtonBox* buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);
    pButtonsLayout->addWidget(buttonBox);
    connect(buttonBox, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);

    //向列表中填充数据
    for (const auto& path : pathList)
    {
        if (!path.isEmpty())
            m_pListWidget->addItem(path);
    }
}

CPathListDialog::~CPathListDialog()
{
}

QString CPathListDialog::GetSelectedPath() const
{
    //没有选择新的文件夹，则返回列表选中的项
    if (m_newfolderPath.isEmpty())
    {
        auto* listItem = m_pListWidget->currentItem();
        if (listItem != nullptr)
        {
            return listItem->text();
        }
    }
    //返回新文件夹
    return m_newfolderPath;
}

void CPathListDialog::accept()
{
    if (m_pListWidget->selectedItems().isEmpty())
    {
        QMessageBox::warning(this, QString(), u8"请选择一个路径！", QMessageBox::Ok);
        return;
    }
    QDialog::accept();
}

void CPathListDialog::OnOpenNewFolder()
{
    //选择一个新文件夹
    m_newfolderPath = QFileDialog::getExistingDirectory(this, QString(), QString(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if (!m_newfolderPath.isEmpty())
    {
        //添加了一个新文件后直接关闭窗口
        QDialog::accept();
    }
}