/* ============================================================
 *
 * This file is a part of digiKam project
 * https://www.digikam.org
 *
 * Date        : 2012-04-19
 * Description : time adjust settings container.
 *
 * Copyright (C) 2012-2019 by Gilles Caulier <caulier dot gilles at gmail dot com>
 *
 * This program is free software; you can redistribute it
 * and/or modify it under the terms of the GNU General
 * Public License as published by the Free Software Foundation;
 * either version 2, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * ============================================================ */

#include "timeadjustcontainer.h"

namespace Digikam
{

TimeAdjustContainer::TimeAdjustContainer()
{
    customDate     = QDateTime::currentDateTime();
    customTime     = QDateTime::currentDateTime();
    adjustmentTime = QDateTime();

    updIfAvailable = true;
    updEXIFModDate = false;
    updEXIFOriDate = false;
    updEXIFDigDate = false;
    updEXIFThmDate = false;
    updIPTCDate    = false;
    updXMPVideo    = false;
    updXMPDate     = false;
    updFileModDate = false;

    dateSource     = APPDATE;
    metadataSource = EXIFIPTCXMP;
    fileDateSource = FILELASTMOD;
    adjustmentType = COPYVALUE;
    adjustmentDays = 0;
}

TimeAdjustContainer::~TimeAdjustContainer()
{
}

bool TimeAdjustContainer::atLeastOneUpdateToProcess() const
{
    return (updFileModDate ||
            updEXIFModDate ||
            updEXIFOriDate ||
            updEXIFDigDate ||
            updEXIFThmDate ||
            updIPTCDate    ||
            updXMPVideo    ||
            updXMPDate);
}

QDateTime TimeAdjustContainer::calculateAdjustedDate(const QDateTime& originalTime) const
{
    int sign = 0;

    switch (adjustmentType)
    {
        case ADDVALUE:
            sign = 1;
            break;
        case SUBVALUE:
            sign = -1;
            break;
        default: // COPYVALUE
            return originalTime;
    }

    int seconds  = adjustmentTime.time().second();
    seconds     += 60*adjustmentTime.time().minute();
    seconds     += 60*60*adjustmentTime.time().hour();
    seconds     += 24*60*60*adjustmentDays;

    return originalTime.addSecs(sign * seconds);
}

QDateTime TimeAdjustContainer::getDateTimeFromUrl(const QUrl& url) const
{
    QStringList regExpStrings;
    regExpStrings << QLatin1String("(.+)?([0-9]{4}[-/]?[0-9]{2}[-/]?[0-9]{2})"
                                   "(.+)?([0-9]{2}[-:]?[0-9]{2}[-:]?[0-9]{2})(.+)?");
    regExpStrings << QLatin1String("(.+)?([0-9]{2}[-/]?[0-9]{2}[-/]?[0-9]{4})"
                                   "(.+)?([0-9]{2}[-:]?[0-9]{2}[-:]?[0-9]{2})(.+)?");
    regExpStrings << QLatin1String("(.+)?([0-9]{4}[-/]?[0-9]{2}[-/]?[0-9]{2})(.+)?");
    regExpStrings << QLatin1String("(.+)?([0-9]{2}[-/]?[0-9]{2}[-/]?[0-9]{4})(.+)?");

    QDateTime dateTime;

    for (int i = 0 ; i < regExpStrings.count() ; ++i)
    {
        QRegExp dateRegExp(regExpStrings.at(i));

        if (dateRegExp.exactMatch(url.fileName()))
        {
            QString dateString = dateRegExp.cap(2);
            QString timeString = dateRegExp.cap(4);
            dateString.remove(QLatin1Char('-'));
            dateString.remove(QLatin1Char('/'));
            timeString.remove(QLatin1Char('-'));
            timeString.remove(QLatin1Char(':'));

            if (i == 0)
            {
                dateTime = QDateTime::fromString(dateString + timeString,
                                                 QLatin1String("yyyyMMddhhmmss"));
            }
            else if (i == 1)
            {
                dateTime = QDateTime::fromString(dateString + timeString,
                                                 QLatin1String("ddMMyyyyhhmmss"));

                if (!dateTime.isValid())
                {
                    dateTime = QDateTime::fromString(dateString + timeString,
                                                     QLatin1String("MMddyyyyhhmmss"));
                }
            }
            else if (i == 2)
            {
                dateTime = QDateTime::fromString(dateString, QLatin1String("yyyyMMdd"));
            }
            else if (i == 3)
            {
                dateTime = QDateTime::fromString(dateString, QLatin1String("ddMMyyyy"));

                if (!dateTime.isValid())
                {
                    dateTime = QDateTime::fromString(dateString, QLatin1String("MMddyyyy"));
                }
            }

            if (!dateTime.isValid())
                continue;

            if (dateTime.date().year() >= 1900 && dateTime.date().year() <= 2100)
                break;
        }
    }

    return dateTime;
}

// -------------------------------------------------------------------

DeltaTime::DeltaTime()
{
    deltaNegative = false;
    deltaDays     = 0;
    deltaHours    = 0;
    deltaMinutes  = 0;
    deltaSeconds  = 0;
}

DeltaTime::~DeltaTime()
{
}

bool DeltaTime::isNull() const
{
    return (deltaDays    == 0 &&
            deltaHours   == 0 &&
            deltaMinutes == 0 &&
            deltaSeconds == 0);
}

} // namespace Digikam
