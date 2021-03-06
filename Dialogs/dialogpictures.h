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

#ifndef DIALOGPICTURES_H
#define DIALOGPICTURES_H

#include <QDialog>
#include <QStandardItemModel>
#include "picturekind.h"

// -------------------------------------------------------
//
//  CLASS DialogPictures
//
//  A dialog used for editing pictures in the game.
//
// -------------------------------------------------------

namespace Ui {
class DialogPictures;
}

class DialogPictures : public QDialog
{
    Q_OBJECT

public:
    explicit DialogPictures(QWidget *parent = 0);
    ~DialogPictures();
    QStandardItemModel* createFoldersModel() const;
    void addfolders(QIcon& icon,
                    QStandardItem* root,
                    QList<QString>& names,
                    QList<PictureKind>& enums) const;

private:
    Ui::DialogPictures *ui;

private slots:
    void on_folderSelected(const QModelIndex& current, const QModelIndex&);
};

#endif // DIALOGPICTURES_H
