/*
    RPG Paper Maker Copyright (C) 2017-2018 Marie Laporte

    This file is part of RPG Paper Maker.

    RPG Paper Maker is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    RPG Paper Maker is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef SYSTEMAUTOTILE_H
#define SYSTEMAUTOTILE_H

#include <QMetaType>
#include "systemspecialelement.h"

// -------------------------------------------------------
//
//  CLASS SystemAutotile
//
//  A particulary autotile (special element).
//
// -------------------------------------------------------

class SystemAutotile : public SystemSpecialElement
{
public:
    SystemAutotile();
    SystemAutotile(int i, QString n, int pictureId);
    virtual ~SystemAutotile();
    static int NUMBER_ROWS;
    static int NUMBER_COLUMNS;
    static int getPreviewWidth(QImage& image);
    static int getPreviewHeight(QImage& image);
    static int getPreviewColumns(QImage& image);
    static int getPreviewRows(QImage& image);
    static int getPreviewSize(int size);

    virtual SystemPicture* picture() const;

    virtual SuperListItem* createCopy() const;
    virtual void setCopy(const SystemAutotile &super);

    virtual void read(const QJsonObject &json);
    virtual void write(QJsonObject &json) const;
};

Q_DECLARE_METATYPE(SystemAutotile)

#endif // SYSTEMAUTOTILE_H
