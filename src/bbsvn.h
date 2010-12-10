/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_SVN_H_
#define _BB_SVN_H_

#include <QProcess>

class BBSvnInfo;
class BBSvnStatus;

class BBSvn : public QProcess
{
    Q_OBJECT

public:
    BBSvn(QObject *parent = 0);
    ~BBSvn();

public:
    void cleanup();
    void addFile(const QString &filename);
    void deleteFile(const QString &filename);

    void status();
    QList<BBSvnStatus*> parseStatus();
    QList<BBSvnStatus*> parseUpdate();

    void remoteInfo(const QString& url);
    void localInfo();
    BBSvnInfo* parseInfo();

    void commit();
    void update();

    void resolveConflict(const QString& file, bool isLocal);

Q_SIGNALS:
    void done(bool ok);

private:
    QString commitMessage();
    QString userName();

    void start(const QStringList &arguments);

private Q_SLOTS:
    void onFinished(int exitCode, QProcess::ExitStatus extiStatus);
};

#endif
