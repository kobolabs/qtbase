/****************************************************************************
**
** Copyright (C) 2012 BogDan Vatra <bogdan@kde.org>
** Contact: http://www.qt-project.org/legal
**
** This file is part of the plugins of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and Digia.  For licensing terms and
** conditions see http://qt.digia.com/licensing.  For further information
** use the contact form at http://qt.digia.com/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 as published by the Free Software
** Foundation and appearing in the file LICENSE.LGPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU Lesser General Public License version 2.1 requirements
** will be met: http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Digia gives you certain additional
** rights.  These rights are described in the Digia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the
** packaging of this file.  Please review the following information to
** ensure the GNU General Public License version 3.0 requirements will be
** met: http://www.gnu.org/copyleft/gpl.html.
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "qandroidplatformscreen.h"
#include "qandroidplatformintegration.h"
#include "androidjnimain.h"
#include "androidjnimenu.h"
#include "qandroidplatformwindow.h"

QAndroidPlatformScreen::QAndroidPlatformScreen():QFbScreen()
{
    mGeometry = QRect(0, 0, QAndroidPlatformIntegration::m_defaultGeometryWidth, QAndroidPlatformIntegration::m_defaultGeometryHeight);
    mFormat = QImage::Format_RGB16;
    mDepth = 16;
    mPhysicalSize.setHeight(QAndroidPlatformIntegration::m_defaultPhysicalSizeHeight);
    mPhysicalSize.setWidth(QAndroidPlatformIntegration::m_defaultPhysicalSizeWidth);
    initializeCompositor();
}

void QAndroidPlatformScreen::topWindowChanged(QWindow *w)
{
    QtAndroidMenu::setActiveTopLevelWindow(w);

    if (w != 0) {
        QAndroidPlatformWindow *platformWindow = static_cast<QAndroidPlatformWindow *>(w->handle());
        if (platformWindow != 0)
            platformWindow->updateStatusBarVisibility();
    }
}

QRegion QAndroidPlatformScreen::doRedraw()
{
    QRegion touched;
    touched = QFbScreen::doRedraw();
    if (touched.isEmpty())
        return touched;

    QtAndroid::flushImage(mGeometry.topLeft(), *mScreenImage, touched.boundingRect());
    return touched;
}

QDpi QAndroidPlatformScreen::logicalDpi() const
{
    qreal lDpi = QtAndroid::scaledDensity() * 72;
    return QDpi(lDpi, lDpi);
}

Qt::ScreenOrientation QAndroidPlatformScreen::orientation() const
{
    return QAndroidPlatformIntegration::m_orientation;
}

Qt::ScreenOrientation QAndroidPlatformScreen::nativeOrientation() const
{
    return QAndroidPlatformIntegration::m_nativeOrientation;
}
