#include "create-link-menu-plugin.h"

#include <file-operation-manager.h>
#include <file-link-operation.h>

#include <QStandardPaths>
#include <QFileDialog>
#include <QTranslator>
#include <QApplication>

using namespace Peony;

CreateLinkMenuPlugin::CreateLinkMenuPlugin(QObject *parent) : QObject (parent)
{
    QTranslator *t = new QTranslator(this);
    qDebug()<<"system().name:"<<QLocale::system().name();
    qDebug()<<"\n\n\n\n\n\n\ntranslate:"<<t->load(":/translations/peony-qt-create-link-extension_"+QLocale::system().name());
    QApplication::installTranslator(t);
}

QList<QAction *> CreateLinkMenuPlugin::menuActions(Types types, const QString &uri, const QStringList &selectionUris)
{
    QList<QAction *> l;
    if (types == MenuPluginInterface::DesktopWindow || types == MenuPluginInterface::DirectoryView) {
        if (selectionUris.count() == 1) {
            auto createLinkToDesktop = new QAction(QIcon::fromTheme("emblem-symbolic-link"), tr("Create Link to Desktop"));
            connect(createLinkToDesktop, &QAction::triggered, [=](){
                QUrl src = selectionUris.first();
                QString desktopUri = "file://" + QStandardPaths::writableLocation(QStandardPaths::DesktopLocation);
                FileLinkOperation *op = new FileLinkOperation(selectionUris.first(), desktopUri);
                op->setAutoDelete(true);
                FileOperationManager::getInstance()->startOperation(op, true);
            });
            l<<createLinkToDesktop;

            auto createLinkTo = new QAction(tr("Create Link to..."));
            connect(createLinkTo, &QAction::triggered, [=](){
                QUrl targetDir = QFileDialog::getExistingDirectoryUrl(nullptr,
                                                                      tr("Choose a Directory to Create Link"),
                                                                      uri);
                if (!targetDir.isEmpty()) {
                    QUrl src = selectionUris.first();
                    QString target = targetDir.url();
                    FileLinkOperation *op = new FileLinkOperation(selectionUris.first(), target);
                    op->setAutoDelete(true);
                    FileOperationManager::getInstance()->startOperation(op, true);
                }
            });
            l<<createLinkTo;
        }
    }
    return l;
}
