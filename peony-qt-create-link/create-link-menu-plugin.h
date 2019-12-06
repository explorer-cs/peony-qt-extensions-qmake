#ifndef CREATELINKMENUPLUGIN_H
#define CREATELINKMENUPLUGIN_H

#include "peony-qt-create-link_global.h"
#include "menu-plugin-iface.h"

#include <QObject>

namespace Peony {

class PEONYQTCREATELINKSHARED_EXPORT CreateLinkMenuPlugin : public QObject, public MenuPluginInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID MenuPluginInterface_iid)
    Q_INTERFACES(Peony::MenuPluginInterface)
public:
    explicit CreateLinkMenuPlugin(QObject *parent = nullptr);

    PluginInterface::PluginType pluginType() override {return PluginInterface::MenuPlugin;}
    const QString name() override {return tr("Peony-Qt Create Link Extension");}
    const QString description() override {return tr("Create Link Menu Extension.");}
    const QIcon icon() override {return QIcon::fromTheme("emblem-symbolic-link");}
    void setEnable(bool enable) override {m_enable = enable;}
    bool isEnable() override {return m_enable;}

    QString testPlugin() override {return "test create link";}
    QList<QAction *> menuActions(Types types, const QString &uri, const QStringList &selectionUris) override;

private:
    bool m_enable = true;
};

}

#endif // CREATELINKMENUPLUGIN_H
