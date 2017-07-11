/** ===========================================================
 * @file
 *
 * This file is a part of digiKam project
 * <a href="http://www.digikam.org">http://www.digikam.org</a>
 *
 * @date    2017-05-22
 * @brief   <a href="http://docs.opencv.org/2.4/modules/contrib/doc/facerec/facerec_tutorial.html#fisherfaces">Face Recognition based on Fisherfaces</a>
 *          Turk, Matthew A and Pentland, Alex P. "Face recognition using fisherfaces." 
 *          Computer Vision and Pattern Recognition, 1991. Proceedings {CVPR'91.},
 *          {IEEE} Computer Society Conference on 1991.
 *
 * @section DESCRIPTION
 *
 * @author Copyright (C) 2017 by Yingjie Liu
 *         <a href="mailto:yingjiewudi at gmail dot come">yingjiewudi at gmail dot come</a>
 *
 * @section LICENSE
 *
 * Released to public domain under terms of the BSD Simplified license.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *   * Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 *   * Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *   * Neither the name of the organization nor the names of its contributors
 *     may be used to endorse or promote products derived from this software
 *     without specific prior written permission.
 *
 *   See <http://www.opensource.org/licenses/bsd-license>
 *
 * ============================================================ */

#ifndef FACESENGINE_FISHERFACESMODEL_H
#define FACESENGINE_FISHERFACESMODEL_H

// OpenCV library

#include "libopencv.h"

// Qt include

#include <QList>

// Local includes

#include "opencvmatdata.h"
#include "facerec_fisherborrowed.h"

namespace Digikam
{

class FisherFaceMatMetadata
{
public:

    enum StorageStatus
    {
        Created,
        InDatabase
    };

public:

    FisherFaceMatMetadata();
    ~FisherFaceMatMetadata();

public:

    int           databaseId;
    int           identity;
    QString       context;

    StorageStatus storageStatus;
};

// -------------------------------------------------------------------------------------------------------------------------------------

class FisherFaceModel : public cv::Ptr<FisherFaceRecognizer>
{
public:

    FisherFaceModel();
    ~FisherFaceModel();

    FisherFaceRecognizer*       ptr();
    const FisherFaceRecognizer* ptr() const;

    //Getter function
    std::vector<cv::Mat> getSrc() const;
    void setSrc(std::vector<cv::Mat> new_src);

    cv::Mat getLabels() const;
    void setLabels(cv::Mat new_labels);

    QList<FisherFaceMatMetadata> matMetadata() const;
    OpenCVMatData                matData(int index) const;

    void setWrittenToDatabase(int index, int databaseId);

    void setMats(const QList<OpenCVMatData>& mats, const QList<FisherFaceMatMetadata>& matMetadata);

    /// Make sure to call this instead of FaceRecognizer::update directly!
    void update(const std::vector<cv::Mat>& images, const std::vector<int>& labels, const QString& context);

//public:

    //int databaseId;

protected:

    QList<FisherFaceMatMetadata> m_matMetadata;
};

} // namespace Digikam

#endif // FACESENGINE_FISHERFACESMODEL_H
