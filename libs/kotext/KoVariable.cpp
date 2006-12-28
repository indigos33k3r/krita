/* This file is part of the KDE project
 * Copyright (C) 2006 Thomas Zander <zander@kde.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public License
 * along with this library; see the file COPYING.LIB.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301, USA.
 */
#include "KoVariable.h"

#include <QTextCursor>
#include <QPainter>
#include <QFontMetricsF>
#include <QTextDocument>
#include <QTextInlineObject>

KoVariable::KoVariable(const QString &value )
    : m_value(value),
    m_modified(true)
{
}

void KoVariable::setValue(const QString &value) {
    if(m_value == value)
        return;
    m_value = value;
    m_modified = true;
}

void KoVariable::updatePosition(const QTextDocument &document, QTextInlineObject object, int posInDocument, const QTextCharFormat & format ) {
    Q_UNUSED(document);
    Q_UNUSED(posInDocument);
    Q_UNUSED(object);
    Q_UNUSED(format);
    // Variables are always 'in place' so the position is 100% defined by the text layout.
}

void KoVariable::resize(const QTextDocument &document, QTextInlineObject object, int posInDocument, const QTextCharFormat &format, QPaintDevice *pd) {
    Q_UNUSED(document);
    Q_UNUSED(posInDocument);
    if(m_modified == false)
        return;
    Q_ASSERT(format.isCharFormat());
    QFontMetricsF fm(format.font(), pd);
    object.setWidth( fm.width(m_value ) );
    object.setAscent(fm.ascent());
    object.setDescent(fm.descent());
    m_modified = true;
}

void KoVariable::paint(QPainter &painter, QPaintDevice *pd, const QTextDocument &document, const QRectF &rect, QTextInlineObject object, int posInDocument, const QTextCharFormat &format) {
    Q_UNUSED(document);
    Q_UNUSED(object);
    Q_UNUSED(posInDocument);

    // TODO set all the font properties from the format (color etc)
    QFont font(format.font(), pd);
    QTextLayout layout(m_value, font, pd);
    layout.setCacheEnabled(true);
    QList<QTextLayout::FormatRange> layouts;
    QTextLayout::FormatRange range;
    range.start=0;
    range.length=m_value.length();
    range.format = format;
    layouts.append(range);
    layout.setAdditionalFormats(layouts);

    QTextOption option(Qt::AlignLeft | Qt::AlignAbsolute);
    option.setTextDirection(object.textDirection());
    layout.setTextOption(option);
    layout.beginLayout();
    layout.createLine();
    layout.endLayout();
    layout.draw(&painter, rect.topLeft());
}
