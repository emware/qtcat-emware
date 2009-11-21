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
#ifndef CATALOGTREE_H
#define CATALOGTREE_H

#include <QTreeWidget>
#include <QPoint>
#include <QTimer>
#include <QIcon>


#include "catalogclass.h"
/**
	@author Victor Nike <victor.nike@gmail.com>
*/

class CatTreeItem;
class QTreeWidgetItem;
class QAction;
class QMenu;
class CatalogClass;
class ConfigManager;
class IconManager;
class dbNodeClass;
class QCatDataModuleClass;

class CatalogTree : public QTreeWidget
{
	Q_OBJECT
	public:
        CatalogTree(QCatDataModuleClass *_dm, QWidget *paren = 0);

    	~CatalogTree();
        void changeDataModule(QCatDataModuleClass *newDM);
        int getCurrentCatalog();
        
    protected:
		void mousePressEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);
        void contextMenuEvent(QContextMenuEvent *event);
        void dragEnterEvent(QDragEnterEvent *event);
		void dragMoveEvent(QDragMoveEvent *event);
        void dropEvent(QDropEvent *event);
		void dragLeaveEvent();
        
    // What to do
    public slots:
        void loadCatalogs();
        void refreshItems();
        void closeCatalog();
        void openCatalog();
        void renameItem();
		void createCatalog();
		void addNewGroup();
		void addNewDisk();
		void deleteItem();
		void catalogProperties();
        void goUpOneLevel();
        void goDownOneLevel(int _id);
        void findItemByItemId(int catId, int id);
		void stopAnimation();
        
    private slots:
        void catTreeItemChanged(QTreeWidgetItem *newItem, QTreeWidgetItem *prevItem);
		void updateItem(CatTreeItem *item);
        void itemIsExpanded(QTreeWidgetItem *item);
        void itemIsCollapsed(QTreeWidgetItem *item);
        void deleteDisk(bool dontAsk = false);
        void deleteGroup(bool dontAsk = false);
        void deleteCatalog();
        void sayNoCatalogs();
        void clearAllTreeItems();
		void autoOpenTimerFired();
        void animate();
        
        CatTreeItem* _findItemByItemId(CatTreeItem *parent, int id);        
    // What happen
    signals:
        void newItemSelected(int newCatalog, int newId);
        void loadCatalog();
        
    private:
        void takeOffCatalogItems(CatTreeItem *item);
		CatTreeItem* findSuitableParentForDisk(CatTreeItem *item); 
		bool moveCatTreeItem(CatTreeItem *itemForMove, CatTreeItem *newParentItem, bool updateDb);
		
        
		void startDrag();
        
        void startAnimate();
        void stopAnimate();
        
        int currAnimFrame;
        QIcon dropTargetItemIcon;
		
		QPoint dragStartingPos;
		CatTreeItem *draggedItem;
        QList <QTreeWidgetItem *> *draggedItems;
		CatTreeItem *dropTargetItem;
        QTimer *autoOpenTimer;
        QTimer *animTimer;
        
        QCatDataModuleClass *dm;
        
        bool clearing;
        QTreeWidgetItem *header;
        int currId;
		int currCatalog;
		int inter;
		int maxLevel;
        
        //GUI
		QAction *pAddCatalogAct;
        QAction *pAddNewGroupAct;
        QAction *pAddNewDiskAct;
        QAction *pUpdateDiskAct;
        QAction *pRenameItemAct;
        QAction *pDeleteItemAct;
        QAction *pPropertiesAct;
        QAction *pCloseCatalogAct;
        QAction *pOpenCatalogAct;
        QAction *pLoadCatalogAct;
        
        QMenu *pCatalogPopupMenu;
        QMenu *pGroupPopupMenu;
        QMenu *pDiskPopupMenu;
		QMenu *pFolderPopupMenu;
		QMenu *pCreateCatalogMenu;
        QMenu *pLoadCatalogMenu;
        
        void init();
		void addTreeItems(int catId, int parentId, CatTreeItem *parent);
		void addTreeItems(int catId, int parentId, CatTreeItem *parent, int currLevel);
		void createActions();
        void createMenus();
        
    
};

#endif
