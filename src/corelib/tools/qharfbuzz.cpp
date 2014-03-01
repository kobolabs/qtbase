/****************************************************************************
**
** Copyright (C) 2013 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the QtCore module of the Qt Toolkit.
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

#include "qharfbuzz_p.h"

#include "qunicodetables_p.h"
#include "qlibrary.h"

QT_USE_NAMESPACE

extern "C" {

void HB_GetUnicodeCharProperties(HB_UChar32 ch, HB_CharCategory *category, int *combiningClass)
{
    const QUnicodeTables::Properties *prop = QUnicodeTables::properties(ch);
    *category = (HB_CharCategory)prop->category;
    *combiningClass = prop->combiningClass;
}

HB_CharCategory HB_GetUnicodeCharCategory(HB_UChar32 ch)
{
    return (HB_CharCategory)QChar::category(ch);
}

int HB_GetUnicodeCharCombiningClass(HB_UChar32 ch)
{
    return QChar::combiningClass(ch);
}

HB_UChar16 HB_GetMirroredChar(HB_UChar16 ch)
{
    return QChar::mirroredChar(ch);
}

void (*HB_Library_Resolve(const char *library, int version, const char *symbol))()
{
#ifdef QT_NO_LIBRARY
    return 0;
#else
    return QLibrary::resolve(QLatin1String(library), version, symbol);
#endif
}

} // extern "C"

QT_BEGIN_NAMESPACE

HB_Bool qShapeItem(HB_ShaperItem *item)
{
    return HB_ShapeItem(item);
}

HB_Face qHBNewFace(void *font, HB_GetFontTableFunc tableFunc)
{
    return HB_AllocFace(font, tableFunc);
}

HB_Face qHBLoadFace(HB_Face face)
{
    return HB_LoadFace(face);
}

void qHBFreeFace(HB_Face face)
{
    HB_FreeFace(face);
}

HB_Error qHB_GSUB_Select_Script(HB_GSUB  gsub,
                 HB_UInt         script_tag,
                 HB_UShort*       script_index)
{
    return HB_GSUB_Select_Script(gsub, script_tag, script_index);
}

HB_Error qHB_GSUB_Select_Feature(HB_GSUB  gsub,
                  HB_UInt         feature_tag,
                  HB_UShort        script_index,
                  HB_UShort        language_index,
                  HB_UShort*       feature_index)
{
    return HB_GSUB_Select_Feature(gsub, feature_tag, script_index, language_index, feature_index);
}

HB_Error qHBBufferNew(HB_Buffer *buffer)
{
    return hb_buffer_new(buffer);
}

HB_Error qHBBufferAddGlyph(HB_Buffer buffer,
                           HB_UInt glyph_index,
                           HB_UInt properties,
                           HB_UInt cluster )
{
    return hb_buffer_add_glyph(buffer, glyph_index, properties, cluster);
}

HB_Error qHB_GSUB_Add_Feature(HB_GSUB gsub,
                               HB_UShort feature_index,
                               HB_UInt property)
{
    return HB_GSUB_Add_Feature(gsub, feature_index, property);
}

HB_Error qHB_GSUB_Clear_Features(HB_GSUB gsub)
{
    return HB_GSUB_Clear_Features(gsub);
}

HB_Error qHB_GSUB_Apply_String(HB_GSUB gsub, HB_Buffer buffer)
{
    return HB_GSUB_Apply_String(gsub, buffer);
}

QT_END_NAMESPACE
