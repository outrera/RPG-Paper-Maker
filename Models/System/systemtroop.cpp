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

#include "systemtroop.h"
#include "systemmonstertroop.h"

// -------------------------------------------------------
//
//  CONSTRUCTOR / DESTRUCTOR / GET / SET
//
// -------------------------------------------------------

SystemTroop::SystemTroop() : SuperListItem()
{
    m_monstersList = new QStandardItemModel();
}

SystemTroop::SystemTroop(int i, QString n, QStandardItemModel* monstersList) :
    SuperListItem(i,n),
    m_monstersList(monstersList)
{

}

SystemTroop::~SystemTroop(){
    SuperListItem::deleteModel(m_monstersList);
}

QStandardItemModel* SystemTroop::monstersList() const { return m_monstersList; }

// -------------------------------------------------------
//
//  READ / WRITE
//
// -------------------------------------------------------

SuperListItem * SystemTroop::createCopy() const {
    SystemTroop* super = new SystemTroop;
    super->setCopy(*this);
    return super;
}

// -------------------------------------------------------

void SystemTroop::setCopy(const SystemTroop &troop) {
    SuperListItem::setCopy(troop);

    // Currencies
    QList<QStandardItem *> row;
    QStandardItem *item;
    SystemMonsterTroop *sys;
    m_monstersList->setHorizontalHeaderLabels(QStringList({"Monster","Level"}));
    for (int i = 0, l = troop.m_monstersList->invisibleRootItem()->rowCount();
         i < l - 1; i++)
    {
        sys = reinterpret_cast<SystemMonsterTroop *>(troop.m_monstersList->item(
            i)->data().value<quintptr>());
        row = sys->createCopy()->getModelRow();
        m_monstersList->appendRow(row);
    }
    item = new QStandardItem();
    item->setText(SuperListItem::beginningText);
    m_monstersList->appendRow(item);
}

// -------------------------------------------------------

void SystemTroop::read(const QJsonObject &json){
    SuperListItem::read(json);
    QJsonArray tab;

    // Monsters list
    m_monstersList->setHorizontalHeaderLabels(QStringList({"Monster","Level"}));
    QStandardItem* item;
    SystemMonsterTroop* monsterTroop;
    QList<QStandardItem *> row;
    tab = json["l"].toArray();
    for (int i = 0; i < tab.size(); i++){
        monsterTroop = new SystemMonsterTroop;
        monsterTroop->read(tab[i].toObject());
        row = monsterTroop->getModelRow();
        m_monstersList->appendRow(row);
    }
    item = new QStandardItem();
    item->setText(SuperListItem::beginningText);
    m_monstersList->appendRow(item);
}

// -------------------------------------------------------

void SystemTroop::write(QJsonObject &json) const{
    SuperListItem::write(json);
    QJsonArray tab;
    QJsonObject obj;
    int l;

    // Monsters list
    tab = QJsonArray();
    l = m_monstersList->invisibleRootItem()->rowCount();
    for (int i = 0; i < l - 1; i++){
        obj = QJsonObject();
        SystemMonsterTroop* monsterTroop =
                (SystemMonsterTroop*) m_monstersList->item(i)->data()
                .value<quintptr>();
        monsterTroop->write(obj);
        tab.append(obj);
    }
    json["l"] = tab;
}
