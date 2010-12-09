#include "bbobserver.h"

#include "bbsettings.h"
#include "bbactionmanager.h"
#include "bbdebug.h"

#include <QFileSystemWatcher>
#include <QFileInfo>
#include <QDir>
#include <QMessageBox>

BBObserver::BBObserver(QObject *parent) :
    QObject(parent)
{
    BBDEBUG;

    connect(BBSettings::instance(),
            SIGNAL(directoryChanged()),
            SLOT(directoryChanged()));

    directoryChanged();

    startTimer(1000);
}

BBObserver::~BBObserver()
{
}

void BBObserver::directoryChanged()
{
    BBDEBUG;

    if (!m_watcher.isNull())
        m_watcher->deleteLater();

    m_watcher = new QFileSystemWatcher(this);
    connect(m_watcher,
            SIGNAL(directoryChanged(QString)),
            SLOT(onSomethingChanged(QString)));
    connect(m_watcher,
            SIGNAL(fileChanged(QString)),
            SLOT(onSomethingChanged(QString)));

    QString dir = BBSettings::instance()->directory();

    QFileInfo info(dir);
    if (!info.exists() || !info.isDir()) {
        QMessageBox::critical(0,
                              BBPACKAGE,
                              tr("The directory '%1' doesn't exist. Please, change it in the preferences!").arg(dir));
        return;
    }

    addDirectory(dir);
}

void BBObserver::addDirectory(const QString& dirname)
{
    BBDEBUG << dirname;

    m_watcher->addPath(dirname);

    QDir dir(dirname);
    QFileInfoList list = dir.entryInfoList();
    foreach (QFileInfo info, list) {
        if (info.fileName().startsWith("."))
            continue;

        addDirectory(info.absoluteFilePath());
    }
}

void BBObserver::onSomethingChanged(const QString &filename)
{
    BBDEBUG << filename;

    QFileInfo info(filename);

    if (!info.exists())
        m_watcher->removePath(filename);
    else if (info.isDir())
        addDirectory(filename);

    if (!m_changes.contains(filename))
        m_changes << filename;
}

void BBObserver::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);

    if (m_changes.isEmpty())
        return;

    foreach(QString filename, m_changes) {
        BBActionManager::instance()->actionAdd(filename);
        BBActionManager::instance()->actionLocalChanges();
    }

    m_changes.clear();
}