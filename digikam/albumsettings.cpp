/* ============================================================
 * Authors: Renchi Raju <renchi@pooh.tam.uiuc.edu>
 *          Caulier Gilles <caulier dot gilles at kdemail dot net>
 * Date   : 2003-16-10
 * Description : 
 * 
 * Copyright 2003-2004 by Renchi Raju and Gilles Caulier
 * Copyright 2005-2006 by Gilles Caulier
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

// Qt includes.
 
#include <qstring.h>

// KDE includes.

#include <kconfig.h>
#include <klocale.h>
#include <kapplication.h>

// Local includes.

#include "thumbnailsize.h"
#include "albumsettings.h"
#include "rawfiles.h"

namespace Digikam
{

class AlbumSettingsPrivate 
{
public:

    bool showToolTips;
    bool showSplash;
    bool useTrash;
    bool scanAtStart;
    
    bool iconShowName;
    bool iconShowSize;
    bool iconShowDate;
    bool iconShowComments;
    bool iconShowResolution;
    bool iconShowTags;
    bool iconShowRating;
    bool saveExifComments;
    bool exifRotate;
    bool exifSetOrientation;
    bool saveIptcRating;

    int  thumbnailSize;

    QString      currentTheme;
    QString      albumLibraryPath;
    QStringList  albumCollectionNames;
    QString      imageFilefilter;
    QString      movieFilefilter;
    QString      audioFilefilter;
    QString      rawFilefilter;
    
    KConfig     *config;

    AlbumSettings::AlbumSortOrder albumSortOrder;
    AlbumSettings::ImageSortOrder  imageSortOrder;

};


AlbumSettings* AlbumSettings::instance_ = 0;

AlbumSettings * AlbumSettings::instance()
{
    return instance_;
}

AlbumSettings::AlbumSettings()
{
    d = new AlbumSettingsPrivate;
    d->config = kapp->config();

    instance_ = this;
    
    init();
}

AlbumSettings::~AlbumSettings()
{
    delete d;
    instance_ = 0;
}

void AlbumSettings::init()
{
    d->albumCollectionNames.clear();
    d->albumCollectionNames.append(i18n("Family"));
    d->albumCollectionNames.append(i18n("Travel"));
    d->albumCollectionNames.append(i18n("Holidays"));
    d->albumCollectionNames.append(i18n("Friends"));
    d->albumCollectionNames.append(i18n("Nature"));
    d->albumCollectionNames.append(i18n("Party"));
    d->albumCollectionNames.append(i18n("Todo"));
    d->albumCollectionNames.append(i18n("Miscellaneous"));
    d->albumCollectionNames.sort();

    d->albumSortOrder  = AlbumSettings::ByFolder;
    d->imageSortOrder  = AlbumSettings::ByIName;
                                          
    d->imageFilefilter = "*.png *.jpg *.jpeg *.tif *.tiff *.gif *.bmp *.xpm *.ppm *.pnm *.xcf *.pcx";
    d->movieFilefilter = "*.mpeg *.mpg *.avi *.mov *.wmf *.asf";
    d->audioFilefilter = "*.ogg *.mp3 *.wma *.wav";
    
    // RAW files estentions supported by dcraw program and 
    // defines to digikam/libs/dcraw/rawfiles.h
    d->rawFilefilter   = QString::QString(raw_file_extentions);
      
    d->thumbnailSize   = ThumbnailSize::Medium;

    d->showToolTips       = true;
    d->showSplash         = true;
    d->useTrash           = true;
    
    d->iconShowName       = false;
    d->iconShowSize       = false;
    d->iconShowDate       = true;
    d->iconShowComments   = true;
    d->iconShowResolution = false;
    d->iconShowTags       = true;
    d->iconShowRating     = true;
    d->saveExifComments   = false;
    d->exifRotate         = false;
    d->exifSetOrientation = false;
    d->saveIptcRating     = false;
}

void AlbumSettings::readSettings()
{
    KConfig* config = d->config;

    config->setGroup("Album Settings");
    
    d->albumLibraryPath = config->readPathEntry("Album Path", QString::null);

    QStringList collectionList = config->readListEntry("Album Collections");
    if (!collectionList.isEmpty())
    {
        collectionList.sort();
        d->albumCollectionNames = collectionList;
    }

    d->albumSortOrder =
        AlbumSettings::AlbumSortOrder(config->readNumEntry("Album Sort Order",
                                                           (int)AlbumSettings::ByFolder));

    d->imageSortOrder =
        AlbumSettings::ImageSortOrder(config->readNumEntry("Image Sort Order",
                                                          (int)AlbumSettings::ByIName));
    
    d->imageFilefilter = config->readEntry("File Filter",
                                           d->imageFilefilter);

    d->movieFilefilter = config->readEntry("Movie File Filter",
                                           d->movieFilefilter);

    d->audioFilefilter = config->readEntry("Audio File Filter",
                                           d->audioFilefilter);
                              
    d->rawFilefilter = config->readEntry("Raw File Filter",
                                         d->rawFilefilter);
                              
    d->thumbnailSize = config->readNumEntry("Default Icon Size",
                             ThumbnailSize::Medium);

    d->showToolTips   = config->readBoolEntry("Show ToolTips", true);
    
    d->iconShowName = config->readBoolEntry("Icon Show Name", false); 

    d->iconShowResolution = config->readBoolEntry("Icon Show Resolution",
                                                  false);                                 

    d->iconShowSize = config->readBoolEntry("Icon Show Size",
                              false);

    d->iconShowDate = config->readBoolEntry("Icon Show Date",
                                            true);

    d->iconShowComments = config->readBoolEntry("Icon Show Comments",
                                                true);

    d->iconShowTags = config->readBoolEntry("Icon Show Tags", true);

    d->iconShowRating = config->readBoolEntry("Icon Show Rating", true);
    
    d->currentTheme = config->readEntry("Theme", i18n("Default"));
    
    config->setGroup("EXIF Settings");
    d->saveExifComments = config->readBoolEntry("Save EXIF Comments", false);
    d->exifRotate = config->readBoolEntry("EXIF Rotate", false);
    d->exifSetOrientation = config->readBoolEntry("EXIF Set Orientation", false);

    config->setGroup("IPTC Settings");
    d->saveIptcRating = config->readBoolEntry("Save IPTC Rating", false);
                                                  
    config->setGroup("General Settings");
    d->showSplash  = config->readBoolEntry("Show Splash", true);
    d->useTrash    = config->readBoolEntry("Use Trash", true);
    d->scanAtStart = config->readBoolEntry("Scan At Start", true);
}

void AlbumSettings::saveSettings()
{
    KConfig* config = d->config;

    config->setGroup("Album Settings");

    config->writePathEntry("Album Path", d->albumLibraryPath);

    config->writeEntry("Album Collections",
                       d->albumCollectionNames);

    config->writeEntry("Album Sort Order",
                       (int)d->albumSortOrder);

    config->writeEntry("Image Sort Order",
                       (int)d->imageSortOrder);
    
    config->writeEntry("File Filter",
                       d->imageFilefilter);

    config->writeEntry("Movie File Filter",
                       d->movieFilefilter);
    
    config->writeEntry("Audio File Filter",
                       d->audioFilefilter);
                           
    config->writeEntry("Raw File Filter",
                       d->rawFilefilter);
    
    config->writeEntry("Default Icon Size",
                       QString::number(d->thumbnailSize));

    config->writeEntry("Show ToolTips", d->showToolTips);
    
    config->writeEntry("Icon Show Name",
                       d->iconShowName);

    config->writeEntry("Icon Show Resolution",
                       d->iconShowResolution);
                                                      
    config->writeEntry("Icon Show Size",
                       d->iconShowSize);
                       
    config->writeEntry("Icon Show Date",
                       d->iconShowDate);
                       
    config->writeEntry("Icon Show Comments",
                       d->iconShowComments);
                       
    config->writeEntry("Icon Show Tags",
                       d->iconShowTags);

    config->writeEntry("Icon Show Rating",
                       d->iconShowRating);
    
    config->writeEntry("Theme", d->currentTheme);
    
    config->setGroup("EXIF Settings");
    config->writeEntry("Save EXIF Comments", d->saveExifComments);
    config->writeEntry("EXIF Rotate", d->exifRotate);
    config->writeEntry("EXIF Set Orientation", d->exifSetOrientation);

    config->setGroup("IPTC Settings");
    config->writeEntry("Save IPTC Rating", d->saveIptcRating);
                           
    config->setGroup("General Settings");
    config->writeEntry("Show Splash", d->showSplash);
    config->writeEntry("Use Trash", d->useTrash);
    config->writeEntry("Scan At Start", d->scanAtStart);
    
    config->sync();
}

void AlbumSettings::setAlbumLibraryPath(const QString& path)
{
    d->albumLibraryPath = path;    
}

QString AlbumSettings::getAlbumLibraryPath() const
{
    return d->albumLibraryPath;
}

void AlbumSettings::setShowSplashScreen(bool val)
{
    d->showSplash = val;    
}

bool AlbumSettings::getShowSplashScreen() const
{
    return d->showSplash;
}

void AlbumSettings::setScanAtStart(bool val)
{
    d->scanAtStart = val;
}

bool AlbumSettings::getScanAtStart() const
{
    return d->scanAtStart;
}

void AlbumSettings::setAlbumCollectionNames(const QStringList& list)
{
    d->albumCollectionNames = list;    
}

QStringList AlbumSettings::getAlbumCollectionNames()
{
    return d->albumCollectionNames;
}

bool AlbumSettings::addAlbumCollectionName(const QString& name)
{
    if (d->albumCollectionNames.contains(name))
        return false;

    d->albumCollectionNames.append(name);
    return true;
}

bool AlbumSettings::delAlbumCollectionName(const QString& name)
{
    uint count = d->albumCollectionNames.remove(name);
    return (count > 0) ? true : false; 
}

void AlbumSettings::setAlbumSortOrder(const AlbumSettings::AlbumSortOrder order)
{
    d->albumSortOrder = order;
}

AlbumSettings::AlbumSortOrder AlbumSettings::getAlbumSortOrder() const
{
    return d->albumSortOrder;
}

void AlbumSettings::setImageSortOrder(const ImageSortOrder order)
{
    d->imageSortOrder = order;    
}

AlbumSettings::ImageSortOrder AlbumSettings::getImageSortOrder() const
{
    return d->imageSortOrder;
}

void AlbumSettings::setImageFileFilter(const QString& filter)
{
    d->imageFilefilter = filter;    
}

QString AlbumSettings::getImageFileFilter() const
{
    return d->imageFilefilter;
}

void AlbumSettings::setMovieFileFilter(const QString& filter)
{
    d->movieFilefilter = filter;    
}

QString AlbumSettings::getMovieFileFilter() const
{
    return d->movieFilefilter;
}

void AlbumSettings::setAudioFileFilter(const QString& filter)
{
    d->audioFilefilter = filter;    
}

QString AlbumSettings::getAudioFileFilter() const
{
    return d->audioFilefilter;
}

void AlbumSettings::setRawFileFilter(const QString& filter)
{
    d->rawFilefilter = filter;    
}

QString AlbumSettings::getRawFileFilter() const
{
    return d->rawFilefilter;
}

bool AlbumSettings::addImageFileExtension(const QString& newExt)
{
    if ( QStringList::split(" ", d->imageFilefilter).contains(newExt) ||
         QStringList::split(" ", d->movieFilefilter).contains(newExt) ||
         QStringList::split(" ", d->audioFilefilter).contains(newExt) ||
         QStringList::split(" ", d->rawFilefilter  ).contains(newExt) )
         return false; 

    d->imageFilefilter = d->imageFilefilter + " " + newExt;
    return true;
}

QString AlbumSettings::getAllFileFilter() const
{
    return d->imageFilefilter + " "
        +  d->movieFilefilter + " "
        +  d->audioFilefilter + " "
        +  d->rawFilefilter;
}

void AlbumSettings::setDefaultIconSize(int val)
{
    d->thumbnailSize = val;
}

int AlbumSettings::getDefaultIconSize() const
{
    return d->thumbnailSize;
}

void AlbumSettings::setIconShowName(bool val)
{
    d->iconShowName = val;
}

bool AlbumSettings::getIconShowName() const
{
    return d->iconShowName;
}

void AlbumSettings::setIconShowSize(bool val)
{
    d->iconShowSize = val;
}

bool AlbumSettings::getIconShowSize() const
{
    return d->iconShowSize;
}

void AlbumSettings::setIconShowComments(bool val)
{
    d->iconShowComments = val;
}

bool AlbumSettings::getIconShowComments() const
{
    return d->iconShowComments;
}

void AlbumSettings::setIconShowResolution(bool val)
{
    d->iconShowResolution = val;
}

bool AlbumSettings::getIconShowResolution() const
{
    return d->iconShowResolution;
}

void AlbumSettings::setIconShowTags(bool val)
{
    d->iconShowTags = val;    
}

bool AlbumSettings::getIconShowTags() const
{
    return d->iconShowTags;
}

void AlbumSettings::setIconShowDate(bool val)
{
    d->iconShowDate = val;
}

bool AlbumSettings::getIconShowDate() const
{
    return d->iconShowDate;
}

void AlbumSettings::setIconShowRating(bool val)
{
    d->iconShowRating = val;    
}

bool AlbumSettings::getIconShowRating() const
{
    return d->iconShowRating;
}

void AlbumSettings::setSaveExifComments(bool val)
{
    d->saveExifComments = val;
}

bool AlbumSettings::getSaveExifComments() const
{
    return d->saveExifComments;
}

void AlbumSettings::setExifRotate(bool val)
{
    d->exifRotate = val;
}

bool AlbumSettings::getExifRotate() const
{
    return d->exifRotate;
}

void AlbumSettings::setExifSetOrientation(bool val)
{
    d->exifSetOrientation = val;
}

bool AlbumSettings::getExifSetOrientation() const
{
    return d->exifSetOrientation;
}

void AlbumSettings::setSaveIptcRating(bool val)
{
    d->saveIptcRating = val;
}

bool AlbumSettings::getSaveIptcRating() const
{
    return d->saveIptcRating;
}

void AlbumSettings::setShowToolTips(bool val)
{
    d->showToolTips = val;
}

bool AlbumSettings::getShowToolTips() const
{
    return d->showToolTips;
}

void AlbumSettings::setCurrentTheme(const QString& theme)
{
    d->currentTheme = theme;    
}

QString AlbumSettings::getCurrentTheme() const
{
    return d->currentTheme;
}

void AlbumSettings::setUseTrash(bool val)
{
    d->useTrash = val;
}

bool AlbumSettings::getUseTrash() const
{
    return d->useTrash;
}

}  // namespace Digikam
