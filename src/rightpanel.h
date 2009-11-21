/***************************************************************************
 *   Copyright (C) 2007 by Victor Nike   *
 *   victor.nike@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef RIGHTPANEL_H
#define RIGHTPANEL_H

#include <QFrame>
#include <QTreeWidgetItem>

#include "dbnode.h"

/**
	@author Victor Nike <victor.nike@gmail.com>
*/

class QSplitter;
class QTreeWidget;
class QTextEdit;

class ConfigManager;
class IconManager;
class dbNodeClass;
class QCatDataModuleClass;
class CatalogTable;

class RightPanel : public QFrame
{
    Q_OBJECT
    public:
        RightPanel(QCatDataModuleClass *_dm, QWidget *parent = 0);
        ~RightPanel();
        
        QTreeWidget *getCatalogTable() const;
        void changeDataModule(QCatDataModuleClass *newDM);
        
        // What to do
    public slots:
        void refreshItems(int newCatalog, int parentId);
        
    private slots:
        void newItemSelectedSlot(int _id);
        void newItemSelectedSlot(QString newDesc);
    // What happen
    signals:
        void newItemSelected(int newId);
        void newItemSelected(QString itemDesc);
        void goUp();
        void goDown(int _id);
		void canceledDrag();

    private:
        // Members
        void init();
        
        // GUI
        CatalogTable *catalogTable;
        QSplitter *vertSplit;
        QTextEdit *showDescription;
        
        //Non GUI
        QCatDataModuleClass *dm;

};

#endif
