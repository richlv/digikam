/* ============================================================
 *
 * This file is a part of digiKam project
 * http://www.digikam.org
 *
 * Date        : 2016-04-21
 * Description : QtAV based video thumbnailer
 *
 * Copyright (C) 2016-2017 by Gilles Caulier <caulier dot gilles at gmail dot com>
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

#include "videothumbnailer.h"

// Qt includes

#include <QUrl>
#include <QFile>
#include <QImage>
#include <QMimeDatabase>

// QtAV includes

#include <QtAV/version.h>
#include <QtAV/AVDemuxer.h>
#include <QtAV/VideoFrameExtractor.h>

// Local includes

#include "thumbnailsize.h"
#include "digikam_debug.h"

namespace Digikam
{

static uchar sprocket_large_png[] =
{
    0x89, 0x50, 0x4e, 0x47, 0x0d, 0x0a, 0x1a, 0x0a, 0x00, 0x00, 0x00, 0x0d,
    0x49, 0x48, 0x44, 0x52, 0x00, 0x00, 0x00, 0x15, 0x00, 0x00, 0x00, 0x0f,
    0x08, 0x06, 0x00, 0x00, 0x00, 0x0b, 0x5a, 0x84, 0x6b, 0x00, 0x00, 0x00,
    0x06, 0x62, 0x4b, 0x47, 0x44, 0x00, 0xff, 0x00, 0xff, 0x00, 0xff, 0xa0,
    0xbd, 0xa7, 0x93, 0x00, 0x00, 0x00, 0x09, 0x70, 0x48, 0x59, 0x73, 0x00,
    0x00, 0x0b, 0x0e, 0x00, 0x00, 0x0b, 0x0e, 0x01, 0x40, 0xbe, 0xe1, 0x41,
    0x00, 0x00, 0x00, 0x07, 0x74, 0x49, 0x4d, 0x45, 0x07, 0xd6, 0x06, 0x1d,
    0x08, 0x25, 0x03, 0x5a, 0x69, 0xff, 0x95, 0x00, 0x00, 0x02, 0x4a, 0x49,
    0x44, 0x41, 0x54, 0x78, 0xda, 0x45, 0x93, 0x4d, 0x2b, 0xf5, 0x41, 0x18,
    0xc6, 0x7f, 0xc7, 0xb9, 0xbd, 0xbf, 0x53, 0x38, 0x84, 0x28, 0x59, 0x49,
    0x3d, 0x3b, 0x65, 0xc7, 0xea, 0x2c, 0x64, 0x65, 0x23, 0xf9, 0x3a, 0x16,
    0xbe, 0x80, 0x94, 0x4f, 0x20, 0x14, 0x59, 0xe8, 0xf9, 0x06, 0x3c, 0x1b,
    0x2c, 0xc8, 0x42, 0x88, 0xbc, 0xe6, 0xf5, 0x78, 0xe7, 0x3c, 0xae, 0xae,
    0x26, 0x53, 0xd3, 0xcc, 0xfc, 0xff, 0x33, 0xbf, 0xfb, 0xbe, 0xae, 0xb9,
    0x27, 0xc3, 0xc0, 0xc0, 0x3f, 0xf2, 0xf9, 0x3f, 0xb4, 0xb5, 0xc1, 0xc2,
    0x02, 0x64, 0x32, 0x90, 0xcb, 0xc1, 0xf5, 0x35, 0x94, 0x94, 0x40, 0x63,
    0x23, 0x3c, 0x3f, 0xc3, 0xe3, 0x23, 0x34, 0x34, 0x40, 0xb1, 0x08, 0xb7,
    0xb7, 0x50, 0x5e, 0x0e, 0xd9, 0x2c, 0xbc, 0xbc, 0xf8, 0xdb, 0xd8, 0x18,
    0xd4, 0xd7, 0xc3, 0xfa, 0x3a, 0x19, 0xfa, 0xfb, 0xff, 0xfe, 0x80, 0x46,
    0x48, 0x6d, 0x78, 0x18, 0x4a, 0x4b, 0xa1, 0xb9, 0x19, 0xca, 0xca, 0xe0,
    0xe9, 0x09, 0x6a, 0x6a, 0xa0, 0xae, 0x0e, 0x6e, 0x6e, 0xe0, 0xeb, 0x4b,
    0x6b, 0x07, 0x79, 0x78, 0x30, 0x78, 0x63, 0x03, 0x52, 0xeb, 0xe9, 0x21,
    0xa8, 0xad, 0x1d, 0x21, 0x82, 0x9f, 0x11, 0x0e, 0x0f, 0xbd, 0x39, 0x9f,
    0x87, 0xca, 0x4a, 0xc1, 0x3d, 0x7a, 0xee, 0x20, 0x1f, 0x1f, 0xf0, 0xf9,
    0x09, 0xef, 0xef, 0xf0, 0xf6, 0x06, 0xab, 0xab, 0x5e, 0xb7, 0xb6, 0x42,
    0x45, 0x05, 0x54, 0x57, 0x13, 0x34, 0x35, 0x41, 0x77, 0x37, 0x74, 0x74,
    0xc0, 0xfe, 0xbe, 0x01, 0x55, 0x55, 0x3e, 0xf4, 0x7b, 0x58, 0xdf, 0x7d,
    0x28, 0xc2, 0x70, 0x07, 0x91, 0x25, 0xde, 0x33, 0x38, 0x68, 0x35, 0x9b,
    0x9b, 0x04, 0xca, 0x6c, 0x6d, 0x0d, 0x40, 0x1e, 0x6a, 0x93, 0x7e, 0xca,
    0x2b, 0xc1, 0x0c, 0x2e, 0x14, 0x34, 0x37, 0xd4, 0xdd, 0xc0, 0x08, 0xed,
    0xb5, 0x25, 0xcb, 0xcb, 0x66, 0xb4, 0xb4, 0x10, 0xf4, 0xf5, 0xd9, 0x60,
    0x65, 0x2c, 0x6f, 0xe4, 0x93, 0xc0, 0x3a, 0xf4, 0xfa, 0x2a, 0xb8, 0xc0,
    0xee, 0x77, 0x77, 0x52, 0xa1, 0x00, 0xba, 0x28, 0xc3, 0xcf, 0xcf, 0xbd,
    0x1e, 0x1a, 0x42, 0x6a, 0x74, 0x89, 0x81, 0x3e, 0x44, 0x68, 0xb3, 0x0e,
    0x2a, 0x5b, 0x07, 0x31, 0x54, 0x07, 0x75, 0xfb, 0x92, 0xe8, 0x00, 0x56,
    0xa0, 0xff, 0x1e, 0x93, 0xc7, 0x89, 0x71, 0x79, 0x49, 0x70, 0x74, 0x04,
    0x3b, 0x3b, 0xe0, 0x66, 0xc3, 0x2d, 0xcd, 0xa3, 0xa5, 0x1a, 0x16, 0x91,
    0x40, 0x29, 0x90, 0xbc, 0xb5, 0x7c, 0x57, 0x00, 0xba, 0xf0, 0x40, 0x52,
    0x47, 0x47, 0xa1, 0xab, 0x0b, 0xe6, 0xe6, 0x5c, 0x9f, 0xc5, 0xa2, 0x32,
    0xd6, 0x4d, 0xa6, 0x00, 0x92, 0x2b, 0x79, 0x82, 0xb9, 0x67, 0x32, 0x82,
    0xc9, 0x12, 0x07, 0x1a, 0x1f, 0xb7, 0xc2, 0xad, 0x2d, 0x02, 0x6d, 0xdc,
    0xdd, 0x85, 0xd3, 0x53, 0x65, 0xa0, 0xf4, 0x55, 0x56, 0x82, 0x18, 0x98,
    0xcd, 0x0a, 0xee, 0xb5, 0x65, 0x27, 0x05, 0xc9, 0x0e, 0x9f, 0xdb, 0xde,
    0x46, 0xf2, 0xb5, 0x2f, 0x38, 0x39, 0x81, 0x83, 0x03, 0x70, 0x53, 0x61,
    0xab, 0xc8, 0x3d, 0x2a, 0xa0, 0x60, 0xce, 0x54, 0xd9, 0x4b, 0xae, 0xe6,
    0xf2, 0x5b, 0x70, 0xd9, 0x65, 0x65, 0x7b, 0x7b, 0xa8, 0xa9, 0x3c, 0x83,
    0xef, 0x6f, 0x98, 0x98, 0xb0, 0xfc, 0xd9, 0x59, 0x3f, 0xc1, 0x8b, 0x0b,
    0x49, 0x54, 0x86, 0xee, 0x2e, 0xa3, 0xe4, 0x73, 0xaa, 0x53, 0xaf, 0x65,
    0x9f, 0xda, 0xd4, 0x94, 0xe7, 0x2b, 0x2b, 0x04, 0xed, 0xed, 0xb0, 0xb4,
    0xe4, 0x37, 0xae, 0x76, 0x75, 0xa5, 0x2e, 0x59, 0x02, 0xca, 0x78, 0xc9,
    0x52, 0xf7, 0x3a, 0xc2, 0x40, 0x2b, 0xb0, 0xa7, 0x6a, 0x8b, 0x8b, 0x56,
    0xd6, 0xdb, 0x4b, 0x70, 0x76, 0xa6, 0x92, 0x30, 0xc8, 0x0f, 0x40, 0xd9,
    0xca, 0x3f, 0x83, 0x0b, 0x05, 0x59, 0xa1, 0xae, 0xb5, 0xe1, 0xe9, 0xd9,
    0xa6, 0xe7, 0x0d, 0xbf, 0xfe, 0xe6, 0x72, 0x04, 0xc7, 0xc7, 0x30, 0x39,
    0x09, 0x9d, 0x9d, 0x30, 0x33, 0x03, 0xb2, 0xe3, 0xfe, 0x5e, 0x50, 0xf9,
    0x26, 0x80, 0x46, 0xd9, 0x61, 0xb0, 0x2d, 0x49, 0x97, 0xa2, 0x17, 0xe5,
    0x4a, 0x98, 0x9e, 0xb6, 0xda, 0xf9, 0x79, 0xfe, 0x03, 0xe1, 0xc7, 0xff,
    0x96, 0xed, 0xf6, 0x1b, 0x9f, 0x00, 0x00, 0x00, 0x00, 0x49, 0x45, 0x4e,
    0x44, 0xae, 0x42, 0x60, 0x82
};
static int sprocket_large_png_len = 701;

class VideoThumbnailer::Private
{
public:

    Private()
    {
        createStrip = false;
        audioFile   = false;
        thumbSize   = ThumbnailSize::Huge;
        position    = 0;
        duration    = 0;
        extractor   = 0;
    }

    bool                 createStrip;
    bool                 audioFile;
    int                  thumbSize;
    qint64               position;
    qint64               duration;
    QString              file;
    QString              name;
    QImage               strip;
    VideoFrameExtractor* extractor;
};

VideoThumbnailer::VideoThumbnailer(QObject* const parent)
    : QObject(parent),
      d(new Private)
{
    d->extractor = new VideoFrameExtractor();
    d->extractor->setAutoExtract(false);

    connect(d->extractor, SIGNAL(frameExtracted(QtAV::VideoFrame)),
            this, SLOT(slotFrameExtracted(QtAV::VideoFrame)));

#if QTAV_VERSION > QTAV_VERSION_CHK(1, 12, 0)
    connect(d->extractor, SIGNAL(error(QString)),
            this, SLOT(slotFrameError()));
#else
    connect(d->extractor, SIGNAL(error()),
            this, SLOT(slotFrameError()));
#endif

    d->strip = QImage::fromData(sprocket_large_png, sprocket_large_png_len, "PNG");
}

VideoThumbnailer::~VideoThumbnailer()
{
    if (d->extractor)
    {
        d->extractor->deleteLater();
    }

    delete d;
}

void VideoThumbnailer::slotGetThumbnail(const QString& file, int size, bool strip)
{
    d->createStrip = strip;

    if (size < ThumbnailSize::Step || size > ThumbnailSize::HD)
    {
        qCDebug(DIGIKAM_GENERAL_LOG) << "Invalid video thumbnail size : " << size;
        d->thumbSize = ThumbnailSize::Huge;
    }
    else
    {
        d->thumbSize = size;
    }

    if (!QFile::exists(file))
    {
        qCDebug(DIGIKAM_GENERAL_LOG) << "Video file " << file << " does not exist.";
        emit signalThumbnailFailed(file);
        return;
    }

    QMimeDatabase mimeDB;

    d->file      = file;
    d->name      = QUrl::fromLocalFile(d->file).fileName();
    bool video   = mimeDB.mimeTypeForFile(d->file).name().startsWith(QLatin1String("video/"));
    d->audioFile = mimeDB.mimeTypeForFile(d->file).name().startsWith(QLatin1String("audio/mpeg"));

    if (!video && !d->audioFile)
    {
        qCDebug(DIGIKAM_GENERAL_LOG) << "Mime type is not video or audio/mpeg from " << d->name;
        emit signalThumbnailFailed(d->file);
        return;
    }

    AVDemuxer demuxer;
    demuxer.setMedia(d->file);

    if (!demuxer.load())
    {
        qCDebug(DIGIKAM_GENERAL_LOG) << "Video cannot loaded from " << d->name;
        emit signalThumbnailFailed(d->file);
        return;
    }

    d->duration = demuxer.duration();
    demuxer.unload();

    if (d->duration <= 0)
    {
        qCDebug(DIGIKAM_GENERAL_LOG) << "Video has no valid duration for " << d->name;
        emit signalThumbnailFailed(d->file);
        return;
    }

    qCDebug(DIGIKAM_GENERAL_LOG) << "Video duration for " << d->name << "is " << d->duration << " seconds";

    d->position = 0;
    tryExtractVideoFrame();
}

void VideoThumbnailer::tryExtractVideoFrame()
{
    d->position += (qint64)(d->duration * 0.2);

    if (d->position >= d->duration)
    {
        qCDebug(DIGIKAM_GENERAL_LOG) << "Problem while video data extraction from " << d->name;
        emit signalThumbnailFailed(d->file);
        return;
    }

    qCDebug(DIGIKAM_GENERAL_LOG) << "Trying to get thumbnail from " << d->name << " at position " << d->position;

    d->extractor->setSource(d->file);
    d->extractor->setPosition(d->position);
    d->extractor->extract();
}

void VideoThumbnailer::slotFrameError()
{
    if (d->audioFile)
    {
        qCDebug(DIGIKAM_GENERAL_LOG) << "Audio file has no embedded image for " << d->name;
        emit signalThumbnailFailed(d->file);
        return;
    }

    tryExtractVideoFrame();
}

void VideoThumbnailer::slotFrameExtracted(const QtAV::VideoFrame& frame)
{
    QImage img = frame.toImage();

    if (!img.isNull())
    {
        img = img.scaled(d->thumbSize, d->thumbSize, Qt::KeepAspectRatio, Qt::SmoothTransformation);

        if (d->createStrip && img.width() > d->strip.width() && img.height() > d->strip.height())
        {
            // Add a video strip on the left side of video thumb.

            for (int y = 0; y < img.height(); y += d->strip.height())
            {
                for (int ys = 0 ; ys < d->strip.height() ; ys++)
                {
                    int pos = y + ys;

                    if (pos < img.height())
                    {
                        memcpy((void*)img.constScanLine(pos), (void*)d->strip.constScanLine(ys), d->strip.bytesPerLine());
                    }
                }
            }
        }

        qCDebug(DIGIKAM_GENERAL_LOG) << "Video frame extracted with size " << img.size();
        emit signalThumbnailDone(d->file, img.copy());
    }
    else
    {
        qCDebug(DIGIKAM_GENERAL_LOG) << "Video frame is null from " << d->name;
        emit signalThumbnailFailed(d->file);
    }
}

} // namespace Digikam
