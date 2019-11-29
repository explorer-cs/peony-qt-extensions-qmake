#include "gtk2-style-enhanced-plugin.h"
#include <complementary-style.h>

#include <QIcon>
#include <QTimer>
#include <QApplication>

#include <QDebug>

using namespace Peony;

void Gtk2StyleEnhancedPlugin::icon_theme_changed_cb(GtkIconTheme *theme, Gtk2StyleEnhancedPlugin *p_this)
{
    gchararray value;
    GtkSettings *settings = gtk_settings_get_default();
    g_object_get(settings, "gtk-icon-theme-name", &value, NULL);
    QString str = QString::fromUtf8(value);
    qDebug()<<str;
    g_free(value);
    if (str != p_this->m_icon_theme_name) {
        p_this->m_icon_theme_name = str;
        QIcon::setThemeName(p_this->m_icon_theme_name);
        qApp->paletteChanged(QPalette());
    }
}

Gtk2StyleEnhancedPlugin::Gtk2StyleEnhancedPlugin(QObject *parent) : QObject(parent)
{
    QIcon::setFallbackThemeName("hicolor");
    m_gtk_icon_theme = gtk_icon_theme_get_default();
    m_gtk_settings = gtk_settings_get_default();
    g_signal_connect(m_gtk_icon_theme, "changed", G_CALLBACK(icon_theme_changed_cb), m_gtk_settings);

    m_timer = new QTimer(this);
    m_timer->setInterval(5000);
    m_timer->setSingleShot(false);
    connect(m_timer, &QTimer::timeout, this, [=](){
        GtkSettings *settings = gtk_settings_get_default();
        gchararray value;
        g_object_get(settings, "gtk-icon-theme-name", &value, NULL);
        QString str = QString::fromUtf8(value);
        qDebug()<<str;
        g_free(value);
        if (str != m_icon_theme_name) {
            m_icon_theme_name = str;
            QIcon::setThemeName(m_icon_theme_name);
            qApp->paletteChanged(QPalette());
        }
    });
    m_timer->start();
}

Gtk2StyleEnhancedPlugin::~Gtk2StyleEnhancedPlugin()
{
    g_signal_handlers_disconnect_by_func(m_gtk_icon_theme, gpointer(icon_theme_changed_cb), nullptr);
    g_object_unref(m_gtk_icon_theme);
}

QProxyStyle *Gtk2StyleEnhancedPlugin::getStyle()
{
    return ComplementaryStyle::getStyle();
}
