#ifndef WIZATTACHMENTLISTWIDGET_H
#define WIZATTACHMENTLISTWIDGET_H

#include "share/wizpopupwidget.h"
#include "share/wizDatabaseManager.h"
#include "share/wizmultilinelistwidget.h"

#include "share/wizimagepushbutton.h"

#include "share/wizfileiconprovider.h"

#include "wizdownloadobjectdatadialog.h"

class CWizExplorerApp;
class CWizAttachmentListViewItem;

class CWizAttachmentListView : public CWizMultiLineListWidget
{
    Q_OBJECT
public:
    CWizAttachmentListView(CWizExplorerApp& app, QWidget* parent);
    const WIZDOCUMENTDATA& document() const { return m_document; }

private:
    CWizExplorerApp& m_app;
    CWizDatabaseManager& m_dbMgr;
    WIZDOCUMENTDATA m_document;
    CWizFileIconProvider m_iconProvider;
    QPointer<QMenu> m_menu;
    CWizDownloadObjectDataDialog* m_downloadDialog;

    void resetAttachments();
    void resetPermission();

public:
    QAction* findAction(const QString& strName);
    void setDocument(const WIZDOCUMENTDATA& document);
    const CWizAttachmentListViewItem* attachmentItemFromIndex(const QModelIndex& index) const;
    void addAttachments();
    void openAttachment(CWizAttachmentListViewItem* item);

protected:
    virtual int wrapTextLineIndex() const;
    virtual bool imageAlignLeft() const;
    virtual int imageWidth() const;
    virtual QString itemText(const QModelIndex& index, int line) const;
    virtual QPixmap itemImage(const QModelIndex& index) const;

    virtual void contextMenuEvent(QContextMenuEvent * e);

    friend class CWizAttachmentListViewItem;

public slots:
    void on_action_addAttachment();
    void on_action_saveAttachmentAs();
    void on_action_openAttachment();
    void on_action_deleteAttachment();
    void on_list_itemDoubleClicked(QListWidgetItem* item);
};


class CWizAttachmentListWidget : public CWizPopupWidget
{
    Q_OBJECT
public:
    CWizAttachmentListWidget(CWizExplorerApp& app, QWidget* parent);

private:
    CWizExplorerApp& m_app;
    QPointer<CWizAttachmentListView> m_list;
    QPointer<CWizImagePushButton> m_btnAddAttachment;

public:
    void setDocument(const WIZDOCUMENTDATA& document);

private:
    void resetPermission();

public Q_SLOTS:
    void on_addAttachment_clicked();
};

#endif // WIZATTACHMENTLISTWIDGET_H
