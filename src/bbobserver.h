/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_OBSERVER_H_
#define _BB_OBSERVER_H_

#include <QObject>
#include <QPointer>
#include <QStringList>

class QFileSystemWatcher;

class BBObserver : public QObject
{
    Q_OBJECT

public:
    BBObserver(QObject *parent = 0);
    ~BBObserver();

public:
    void operationOnFileSystemRef();
    void operationOnFileSystemUnref();

private:
    void addDirectory(const QString &dirname);
    void timerEvent(QTimerEvent *event);

private Q_SLOTS:
    void directoryChanged();
    void onSomethingChanged(const QString &filename);
    void onAboutToQuit();

private:
    QPointer<QFileSystemWatcher> m_watcher;
    QStringList m_changes;

    uint m_operationOnFs;
};

#endif
