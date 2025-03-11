#pragma once

#include <QDialog>
#include "ccommontools_global.h"

class QListWidget;
class CCOMMONTOOLS_EXPORT CPathListDialog  : public QDialog
{
    Q_OBJECT
public:
    CPathListDialog(const QStringList& pathList, QWidget *parent = nullptr);
    ~CPathListDialog();

    QString GetSelectedPath() const;

protected:
    virtual void accept() override;

private slots:
    void OnOpenNewFolder();

private:
    QListWidget* m_pListWidget;
    QString m_newfolderPath;
};
