/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2012-01-20
 * Description : new items finder.
 *
 * Copyright (C) 2012 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "newitemsfinder.moc"

// KDE includes

#include <kapplication.h>
#include <klocale.h>
#include <kicon.h>
#include <kdebug.h>

// Local includes

#include "scancontroller.h"
#include "knotificationwrapper.h"

namespace Digikam
{

NewItemsFinder::NewItemsFinder()
    : ProgressItem(0, "NewItemsFinder", QString(), QString(), true, true)
{
    ProgressManager::addProgressItem(this);

    m_duration.start();

    connect(ScanController::instance(), SIGNAL(scanningProgress(float)),
            this, SLOT(slotProgressValue(float)));

    connect(ScanController::instance(), SIGNAL(collectionScanFinished()),
            this, SLOT(slotScanCompleted()));

    connect(ScanController::instance(), SIGNAL(collectionScanStarted(QString)),
            this, SLOT(slotScanInformation(QString)));

    connect(this, SIGNAL(progressItemCanceled(ProgressItem*)),
            this, SLOT(slotCancel()));

    setLabel(i18n("Find new items"));
    setThumbnail(KIcon("view-refresh").pixmap(22));
    setUsesBusyIndicator(true);

    ScanController::instance()->allowToScanDeferredFiles();
}

NewItemsFinder::~NewItemsFinder()
{
}

void NewItemsFinder::slotProgressValue(float v)
{
    // FIXME: sound like progress indication from ScanController are not suitable... Why ?
//    setProgress((int)(v*100.0));
}

void NewItemsFinder::slotScanInformation(const QString& info)
{
    setStatus(info);
}

void NewItemsFinder::slotScanCompleted()
{
    QTime now, t = now.addMSecs(m_duration.elapsed());
    // Pop-up a message to bring user when all is done.
    KNotificationWrapper(id(),
                         i18n("Find new items is done.\nDuration: %1", t.toString()),
                         kapp->activeWindow(), label());

    emit signalComplete();

    setComplete();
}

void NewItemsFinder::slotCancel()
{
    ScanController::instance()->cancelCompleteScan();

    emit signalComplete();

    setComplete();
}

}  // namespace Digikam
