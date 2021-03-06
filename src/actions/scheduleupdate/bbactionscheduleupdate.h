/* *** This file is part of bbox ***
 *
 * Copyright (C) 2010 Andrea Marchesini <baku@ippolita.net>.
 *
 * This program is free software. It is released under the terms of
 * the BSD License. See license.txt for more details.
 */

#ifndef _BB_ACTION_SCHEDULEUPDATE_H_
#define _BB_ACTION_SCHEDULEUPDATE_H_

#include "bbaction.h"

class BBSvn;

class BBActionScheduleUpdate : public BBAction
{
    Q_OBJECT

public:
    BBActionScheduleUpdate(QObject *parent = 0);
    ~BBActionScheduleUpdate();

public:
    void run();
    bool compare(const BBAction *action);
};

#endif
