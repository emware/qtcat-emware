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
#ifndef CATALOGTABLE_H
#define CATALOGTABLE_H

#include <QTreeWidget>
#include <QPoint>

/**
	@author Victor Nike <victor.nike@gmail.com>
*/
struct dbNode;
class QCatDataModuleClass;

class CatalogTable : public QTreeWidget
{
	Q_OBJECT
	public:
        CatalogTable(QCatDataModuleClass *_dm, QWidget *parent = 0);
	    ~CatalogTable();
    
        void changeDataModule(QCatDataModuleClass *_newDM);
        // What to do
    protected:
        void mousePressEvent(QMouseEvent *event);
        void mouseMoveEvent(QMouseEvent *event);
        void contextMenuEvent(QContextMenuEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);
        void dragMoveEvent(QDragMoveEvent *event);
        
    public slots:
        void refreshItems(int newCatalog, int parentId);
        
    private slots:
        void itemChanged(QTreeWidgetItem *newItem, QTreeWidgetItem *prevItem);
        void columnResized(int index, int oldSize, int newSize);
        void vItemActivated(QTreeWidgetItem *activeItem, int column);
        
    // What happen
    signals:
        void newItemSelected(int newId);
        void newItemSelected(QString itemDesc);
        void goUp();
        void goDown(int _id);
		void canceledDrag();
    
    private:
        void clearAllItems();
        void startDrag();
        
        QCatDataModuleClass *dm;
        QList<QTreeWidgetItem *> selectedItemsList;
        QList<dbNode *> items;
        QStringList *folderHeader;
        QStringList *diskHeader;
        
        
        int currId;
        bool clearTree;
        bool bDiskColumns;
        bool canResize;
        QPoint dragStartingPos;

        
        void init();

};

#endif
