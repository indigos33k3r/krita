/* This file is part of the KDE project
   Copyright 2009 Vera Lukman <vla24@sfu.ca>

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public
   License version 2 as published by the Free Software Foundation.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public License
   along with this library; see the file COPYING.LIB.  If not, write to
   the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.
*/

#include "kis_favorite_brush_data.h"
#include "kis_popup_palette.h"
#include "ko_favorite_resource_manager.h"
#include <kis_paintop_preset.h>
#include <kis_types.h>
#include <KoID.h>
#include <QDebug>
#include <QIcon>
#include <QToolButton>

KisFavoriteBrushData::KisFavoriteBrushData(KoFavoriteResourceManager* resourceManager, KisPaintOpPresetSP  newdata, QIcon* icon)
        : m_favoriteResourceManager (resourceManager)
        , m_button (0)
        , m_data (newdata)
{
    // without a button, this class doesn't make sense, so always initialize it
    m_button = new QToolButton();
    m_button->setMinimumSize(KisPopupPalette::BUTTON_SIZE, KisPopupPalette::BUTTON_SIZE);
    m_button->setMaximumSize(KisPopupPalette::BUTTON_SIZE, KisPopupPalette::BUTTON_SIZE);
    connect(m_button, SIGNAL(clicked()), this, SLOT(slotBrushButtonClicked()));
    connect(this, SIGNAL(signalPaintOpChanged(KisPaintOpPresetSP)), m_favoriteResourceManager, SLOT(slotChangeCurrentPaintOp(KisPaintOpPresetSP)));
    if (icon) m_button->setIcon(*icon);
}

void KisFavoriteBrushData::slotBrushButtonClicked()
{
    qDebug() << "Brush name:" << m_data->paintOp();
    emit signalPaintOpChanged(m_data);
}

KisPaintOpPresetSP KisFavoriteBrushData::paintopPreset()
{
    return m_data;
}

void KisFavoriteBrushData::setIcon (QIcon* icon)
{
    m_button->setIcon(*icon);
}

QToolButton* KisFavoriteBrushData::paintopButton()
{
    return m_button;
}

KisFavoriteBrushData::~KisFavoriteBrushData()
{
    m_favoriteResourceManager = 0;
    qDebug() << "Brush name:" << m_data->paintOp() << "deleting";
    // don't delete m_data, it's a shared pointer
    // only delete the button if it hasn't got a parent object, otherwise
    // we have a double delete
    if (!m_button->parent()) {
        delete m_button;
    }
}

#include "kis_favorite_brush_data.moc"
