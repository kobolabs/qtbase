/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
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

#include "qfbscreen_p.h"
#include "qfbcursor_p.h"
#include "qfbwindow_p.h"
#include "qfbbackingstore_p.h"

#include <QtGui/QPainter>
#include <qpa/qwindowsysteminterface.h>

QT_BEGIN_NAMESPACE

QFbScreen::QFbScreen() : mCursor(0), mGeometry(), mDepth(16), mFormat(QImage::Format_RGB16), mScreenImage(0), mCompositePainter(0), mIsUpToDate(false)
{
    mRedrawTimer.setSingleShot(true);
    mRedrawTimer.setInterval(0);
    connect(&mRedrawTimer, SIGNAL(timeout()), this, SLOT(doRedraw()));
}

QFbScreen::~QFbScreen()
{
    delete mCompositePainter;
    delete mScreenImage;
}

void QFbScreen::initializeScreenImage()
{
    delete mScreenImage;
    mScreenImage = new QImage(mGeometry.size(), mFormat);
}

void QFbScreen::initializeCompositor()
{
    delete mCompositePainter;
    mCompositePainter = new QPainter(mScreenImage);
}

void QFbScreen::addWindow(QFbWindow *window)
{
    mWindowStack.prepend(window);
    if (!mBackingStores.isEmpty()) {
        //check if we have a backing store for this window
        for (int i = 0; i < mBackingStores.size(); ++i) {
            QFbBackingStore *bs = mBackingStores.at(i);
            // this gets called during QWindow::create() at a point where the
            // invariant (window->handle()->window() == window) is broken
            if (bs->window() == window->window()) {
                window->setBackingStore(bs);
                mBackingStores.removeAt(i);
                break;
            }
        }
    }

    connect(window->window(), &QWindow::destroyed, this, [=] {
        mWindowStack.removeAll(window);
    });
    invalidateRectCache();
    setDirty(mapToDevice(window->geometry()));
    QWindow *w = window->window();
    QWindowSystemInterface::handleWindowActivated(w);
    topWindowChanged(w);
}

void QFbScreen::removeWindow(QFbWindow *window)
{
    mWindowStack.removeAll(window);
    invalidateRectCache();
    setDirty(mapToDevice(window->geometry()), window->window()->dirtyFlags());
    QWindow *w = topWindow();
    QWindowSystemInterface::handleWindowActivated(w);
    topWindowChanged(w);
}

void QFbScreen::raise(QFbWindow *window)
{
    int index = mWindowStack.indexOf(window);
    if (index <= 0)
        return;
    mWindowStack.move(index, 0);
    invalidateRectCache();
    setDirty(mapToDevice(window->geometry()));
    QWindow *w = window->window();
    QWindowSystemInterface::handleWindowActivated(w);
    topWindowChanged(w);
}

void QFbScreen::lower(QFbWindow *window)
{
    int index = mWindowStack.indexOf(window);
    if (index == -1 || index == (mWindowStack.size() - 1))
        return;
    mWindowStack.move(index, mWindowStack.size() - 1);
    invalidateRectCache();
    setDirty(mapToDevice(window->geometry()));
    QWindow *w = topWindow();
    QWindowSystemInterface::handleWindowActivated(w);
    topWindowChanged(w);
}

QWindow *QFbScreen::topWindow() const
{
    foreach (QFbWindow *fbw, mWindowStack)
        if (fbw->window()->type() == Qt::Window || fbw->window()->type() == Qt::Dialog)
            return fbw->window();
    return 0;
}

QWindow *QFbScreen::topLevelAt(const QPoint & p) const
{
    foreach (QFbWindow *fbw, mWindowStack) {
        if (fbw->geometry().contains(p, false) && fbw->window()->isVisible())
            return fbw->window();
    }
    return 0;
}

static QTransform map(int angle, const QRect &geometry)
{
	QSize size = geometry.size();
	QTransform t;
	switch (angle) {
		case 90:
			t.translate(size.width(), 0);
			break;
		case 180:
			t.translate(size.width(), size.height());
			break;
		case 270:
			t.translate(0, size.height());
			break;
		default:
			break;
	}
	t.rotate(angle);
	return t;
}

QRect QFbScreen::mapToDevice(const QRect &rect) const
{
    QTransform transform = map(screen()->angleBetween(nativeOrientation(), orientation()), mGeometry);
    return transform.mapRect(rect);
}

void QFbScreen::setDirty(const QRect &rect, uint flags)
{
    Q_UNUSED(flags);
    QRect intersection = rect.intersected(mGeometry);
    QPoint screenOffset = mGeometry.topLeft();
    mRepaintRegion += intersection.translated(-screenOffset);    // global to local translation
    scheduleUpdate();
}

void QFbScreen::scheduleUpdate()
{
    if (!mRedrawTimer.isActive())
        mRedrawTimer.start();
}

void QFbScreen::setPhysicalSize(const QSize &size)
{
    mPhysicalSize = size;
}

void QFbScreen::setGeometry(const QRect &rect)
{
    mGeometry = rect;
    initializeScreenImage();
    initializeCompositor();

    invalidateRectCache();
    QWindowSystemInterface::handleScreenGeometryChange(QPlatformScreen::screen(), geometry());
    QWindowSystemInterface::handleScreenAvailableGeometryChange(QPlatformScreen::screen(), availableGeometry());
    resizeMaximizedWindows();
}

void QFbScreen::generateRects()
{
    mCachedRects.clear();
    QPoint screenOffset = mGeometry.topLeft();
    QRegion remainingScreen(mGeometry.translated(-screenOffset)); // global to local translation

    for (int i = 0; i < mWindowStack.length(); i++) {
        if (remainingScreen.isEmpty())
            break;
#if 0
        if (!mWindowStack[i]->isVisible())
            continue;
        if (mWindowStack[i]->isMinimized())
            continue;

        if (!mWindowStack[i]->testAttribute(Qt::WA_TranslucentBackground)) {
            QRect localGeometry = mWindowStack.at(i)->geometry().translated(-screenOffset); // global to local translation
            remainingScreen -= localGeometry;
            QRegion windowRegion(localGeometry);
            windowRegion -= remainingScreen;
            foreach (QRect rect, windowRegion.rects()) {
                mCachedRects += QPair<QRect, int>(rect, i);
            }
        }
#endif
    }
    foreach (QRect rect, remainingScreen.rects())
        mCachedRects += QPair<QRect, int>(rect, -1);
    mIsUpToDate = true;
    return;
}

QRegion QFbScreen::doRedraw()
{
    QPoint screenOffset = mGeometry.topLeft();

    QRegion touchedRegion;
    if (mCursor && mCursor->isDirty() && mCursor->isOnScreen()) {
        QRect lastCursor = mCursor->dirtyRect();
        mRepaintRegion += lastCursor;
    }
    if (mRepaintRegion.isEmpty() && (!mCursor || !mCursor->isDirty())) {
        return touchedRegion;
    }

    QVector<QRect> rects = mRepaintRegion.rects();

    if (!mIsUpToDate)
        generateRects();

    if (!mCompositePainter)
        initializeCompositor();

    for (auto rectIndex = rects.constBegin(); rectIndex != rects.constEnd(); rectIndex++) {
        QRegion rectRegion = *rectIndex;

        for (auto i = mCachedRects.constBegin(); i != mCachedRects.constEnd(); i++) {
            const QRect &screenSubRect = i->first;
            QRegion intersect = rectRegion.intersected(screenSubRect);

            if (intersect.isEmpty())
                continue;

            rectRegion -= intersect;

            QRect rect = intersect.boundingRect();
            int z = 0;
            // find the highest window to fully contain this rect
            while (z < mWindowStack.size()) {
                if (!mWindowStack[z]->window()->isVisible() || mWindowStack[z]->window()->flags() & Qt::KoboDialogTranslucentHint) {
                    z++;
                    continue;
                }
                QRect windowRect = mWindowStack[z]->geometry().translated(-screenOffset);
                if (windowRect.contains(rect)) {
                    break;
                }
                z++;
            }

            for (int layerIndex = qMin(z, mWindowStack.size() - 1); layerIndex != -1; layerIndex--) {
                if (!mWindowStack[layerIndex]->window()->isVisible())
                    continue;
                QRect windowRect = mWindowStack[layerIndex]->geometry().translated(-screenOffset);
                QRect windowIntersect = rect.translated(-windowRect.left(), -windowRect.top());
                if (!hasAcceleratedRotation() && nativeOrientation() != orientation()) {
                    QTransform transform = map(screen()->angleBetween(nativeOrientation(), orientation()), mGeometry);
                    mCompositePainter->setTransform(transform, false);
                }
                mCompositePainter->drawImage(rect, mWindowStack[layerIndex]->backingStore()->image(), windowIntersect);
                touchedRegion += rect;
            }
        }
    }

    QRect cursorRect;
    if (mCursor && (mCursor->isDirty() || mRepaintRegion.intersects(mCursor->lastPainted()))) {
        cursorRect = mCursor->drawCursor(*mCompositePainter);
        touchedRegion += cursorRect;
    }

    mRepaintRegion = QRegion();



//    qDebug() << "QFbScreen::doRedraw"  << mWindowStack.size() << mScreenImage->size() << touchedRegion;

    return touchedRegion;
}

QT_END_NAMESPACE

