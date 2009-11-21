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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTreeWidgetItem>
#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QContextMenuEvent>
#include <QInputDialog>
#include <QMessageBox>
#include <QComboBox>
#include <QPixmap>
#include <QTime>
#include <QTimer>
#include <QDateTime>


#include "defsandtools.h"
#include "datamodule.h"
#include "catalogtree.h"
#include "dbnode.h"
#include "mediascanui.h"
#include "configmanager.h"
#include "catalogpropertiesdlg.h"
#include "iconmanager.h"

#define AUTO_OPEN_INTERVAL 1000

CatalogTree::CatalogTree(QCatDataModuleClass *_dm, QWidget *parent)
    : dm(_dm), QTreeWidget(parent)
{
    init();
}

CatalogTree::~CatalogTree()
{
    delete header;
	delete autoOpenTimer;
    //delete im;
}

void CatalogTree::init()
{	
    currId = 0;
	currCatalog = 0;
    clearing = false;
	dropTargetItem = NULL;
	draggedItem = NULL;
    currAnimFrame = 1;
	autoOpenTimer = new QTimer();
    header = new QTreeWidgetItem();
	connect(autoOpenTimer, SIGNAL(timeout()), this, SLOT(autoOpenTimerFired()));
    animTimer = new QTimer();
    connect(animTimer, SIGNAL(timeout()), this, SLOT(animate()));
    this->setColumnCount(1);
    this->setIconSize(QSize(22, 22));
	
    header->setText(0, tr("Catalogs"));
    this->setHeaderItem(header);
    
    setEditTriggers(QAbstractItemView::AllEditTriggers);
    
    createActions();
    createMenus();
    //setRootIsDecorated(false);
    connect(this, SIGNAL(currentItemChanged (QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(catTreeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(this, SIGNAL(itemExpanded(QTreeWidgetItem*)), this, SLOT(itemIsExpanded(QTreeWidgetItem*)));
    connect(this, SIGNAL(itemCollapsed(QTreeWidgetItem*)), this, SLOT(itemIsCollapsed(QTreeWidgetItem*)));
    //setDragDropMode(QAbstractItemView::DragDrop);
    setAcceptDrops(true);
	setAutoScroll(true);
}

void CatalogTree::addTreeItems(int catId, int parentId, CatTreeItem *parent, int currLevel)
{
	if ((maxLevel != -1) && (currLevel > maxLevel))
		return;
	currLevel++;
	//QString sql = "SELECT id, parent, name, itemType, iconId, attributes FROM Catalog_" + QString::number(catId) + " Where parent = '" + QString::number(parentId) + "' and (itemType = 1 or itemType = 2 or itemType = 3) ORDER BY name ASC";
    //QString sql = "SELECT id, parent, name, itemType, iconId, attributes FROM Catalog_" + QString::number(catId) + " Where parent = '" + QString::number(parentId) + "' and (itemType <> 4) ORDER BY itemType, name ASC";
    QString sql = "SELECT id, parent, name, itemType, iconId, attributes FROM Catalog_" + QString::number(catId) + " Where parent = '" + QString::number(parentId) + "' ORDER BY itemType, name ASC";
    
	inter++;
	QSqlQuery *q = new QSqlQuery(dm->db);
	q->exec(sql);
	//printQS(q->lastQuery());
	if (q->isActive())
	{
		while (q->next())
		{
			CatTreeItem *it = new CatTreeItem(parent);
			it->indexOrCatId = catId;
			it->id = q->value(0).toInt();
			it->parentId = parentId;
			it->itemType = q->value(3).toInt();
            
			it->setText(0, q->value(2).toString());
			it->iconId = q->value(4).toInt();
			it->fullType = q->value(5).toInt();
			if (maxLevel == -1)
				it->childsLoaded = true;
			else
				it->childsLoaded = false;
			int currId = it->id;
			int iconId = it->iconId;
			switch (it->itemType)
			{
				case GROUP_ELEMENT: 
				{
					QPixmap *pix = 0;
					if (!iconId)
					{
						pix = dm->im->getPixmap(0, DISK_GROUP_ICON);
					}else{
						pix = dm->im->getPixmap(catId, iconId);
					}
					if (pix)
						it->setIcon(0, QIcon(*pix));
					delete pix;
				}
				break;
				case DISK_ELEMENT: 
				{
					QPixmap *pix = 0;
					if (!iconId)
					{
						pix = dm->im->getDefaultPixmap(DISK_ELEMENT, it->fullType);
                            //pix = im->getPixmap(0, DISK_CD_ICON);
					}else{
						pix = dm->im->getPixmap(catId, iconId);
					}
					if (pix)
						it->setIcon(0, QIcon(*pix));
					delete pix;
				}
				break;
				case FOLDER_ELEMENT: 
				{
					QPixmap *pix = 0;
					if (!iconId)
					{
						pix = dm->im->getPixmap(0, FOLDER_CLOSED_ICON);
					}else{
						pix = dm->im->getPixmap(catId, iconId);
					}
					if (pix)
						it->setIcon(0, QIcon(*pix));
					delete pix;
				}
				break;
			}
			addTreeItems(catId, currId, it, currLevel);
		}
	}
}

void CatalogTree::addTreeItems(int catId, int parentId, CatTreeItem *parent)
{
	//QString sql = "SELECT id, parent, name, itemType, iconId, attributes FROM Catalog_" + QString::number(catId) + " Where parent = '" + QString::number(parentId) + "' and (itemType = 1 or itemType = 2 or itemType = 3) ORDER BY name ASC";
    //QString sql = "SELECT id, parent, name, itemType, iconId, attributes FROM Catalog_" + QString::number(catId) + " Where parent = '" + QString::number(parentId) + "' and (itemType <> 4) ORDER BY itemType, name ASC";
    QString sql = "SELECT id, parent, name, itemType, iconId, attributes FROM Catalog_" + QString::number(catId) + " Where parent = '" + QString::number(parentId) + "' ORDER BY itemType, name ASC";
    
	inter++;
    QSqlQuery *q = new QSqlQuery(dm->db);
    q->exec(sql);
    if (q->isActive())
    {
        while (q->next())
        {
            CatTreeItem *it = new CatTreeItem(parent);
            it->indexOrCatId = catId;
            it->id = q->value(0).toInt();
            it->parentId = parentId;
            it->itemType = q->value(3).toInt();
            
            it->setText(0, q->value(2).toString());
            it->iconId = q->value(4).toInt();
            it->fullType = q->value(5).toInt();
            int currId = it->id;
            int iconId = it->iconId;
            switch (it->itemType)
            {
                case GROUP_ELEMENT: 
                    {
                        QPixmap *pix = 0;
                        if (!iconId)
                        {
                            pix = dm->im->getPixmap(0, DISK_GROUP_ICON);
                        }else{
                            pix = dm->im->getPixmap(catId, iconId);
                        }
                        if (pix)
                            it->setIcon(0, QIcon(*pix));
                        delete pix;
                    }
                    break;
                case DISK_ELEMENT: 
                    {
                        QPixmap *pix = 0;
                        if (!iconId)
                        {
                            pix = dm->im->getDefaultPixmap(DISK_ELEMENT, it->fullType);
                            //pix = im->getPixmap(0, DISK_CD_ICON);
                        }else{
                            pix = dm->im->getPixmap(catId, iconId);
                        }
                        if (pix)
                            it->setIcon(0, QIcon(*pix));
                        delete pix;
                    }
                    break;
                case FOLDER_ELEMENT: 
                    {
                        QPixmap *pix = 0;
                        if (!iconId)
                        {
                            pix = dm->im->getPixmap(0, FOLDER_CLOSED_ICON);
                        }else{
                            pix = dm->im->getPixmap(catId, iconId);
                        }
                        if (pix)
                            it->setIcon(0, QIcon(*pix));
                        delete pix;
                    }
                    break;
            }
            addTreeItems(catId, currId, it);
        }
    }
}

// Public Slots

void CatalogTree::refreshItems()
{
    clearing = true;
    clear();
    clearing = false;
	dm->cat->readAllCatalogs();
	maxLevel = -1;
    if (dm->cat->items.size() > 0)
	{
        for (int i = 0; i < dm->cat->items.size(); ++i)
		{
            catalogNode *item = dm->cat->items.at(i);
		
			CatTreeItem *treeItem = new CatTreeItem;
			treeItem->indexOrCatId = i;						// If type is ROOT_ELEMENT -> id in the QList
			treeItem->itemType = CATALOG_ELEMENT;			// type ROOT_ELEMENT - Means item is Catalog
			treeItem->id = item->catId;						// Catalog Id in Catalogs Table
			treeItem->parentId = 0;							// No parent for Root Catalog Items - Catalog Names
            treeItem->iconId = item->iconId;
            
			treeItem->setText(0, item->catName);
            
            QPixmap *pix = 0;
			if ((!item->isReadProtected) && (item->isOpened))
			{
                if (item->iconId)
                    pix = dm->im->getPixmap(item->catId, item->iconId);
            	else
                    pix = dm->im->getPixmap(0, CATALOG_ICON_ENABLED);
				treeItem->setIcon(0, QIcon(*pix));
                delete pix;
				//addTreeItems(item->catId, 0, treeItem);
				
				addTreeItems(item->catId, 0, treeItem, 0);
			}else{
                pix = dm->im->getPixmap(0, CATALOG_ICON_DISABLED);
				treeItem->setIcon(0, QIcon(*pix));
                delete pix;
			}
            addTopLevelItem(treeItem);
		}
		
	}else{
        sayNoCatalogs();
	}
}

void CatalogTree::sayNoCatalogs()
{
    CatTreeItem *treeItem = new CatTreeItem;
    treeItem->id = 0;
    treeItem->parentId = 0;
    if (dm)
    {
        treeItem->indexOrCatId = -1; 		// No Catalogs Available
        treeItem->itemType = -1;	// No item available
        treeItem->setText(0, tr("Empty Catalog!"));
    }
    else{
        treeItem->indexOrCatId = -2; 		// DB not available
        treeItem->itemType = -2;	
        treeItem->setText(0, tr("No open catalog!"));
    }
    treeItem->setIcon(0, QIcon(":icons/interface/critical.png"));
        
    addTopLevelItem(treeItem);
}

void CatalogTree::loadCatalogs()
{
    if (dm)
    {
    	refreshItems();    
    }else{
     	sayNoCatalogs();   
    }
    
}


// Private Slots
void CatalogTree::catTreeItemChanged(QTreeWidgetItem *newItem, QTreeWidgetItem *prevItem)
{
    int currCatalog = -1;
    int currId = 0;
    if (!clearing)
    {
        CatTreeItem *treeItem = static_cast<CatTreeItem *>(newItem);
        if (treeItem)
        {
			if (treeItem->itemType == -1)
				return;
			if (treeItem->itemType == -2)
				return;
            if (treeItem->itemType == CATALOG_ELEMENT)
            {
             	// Do non root element doings      
                catalogNode *node = dm->cat->items.at(treeItem->indexOrCatId);
                if (node->isOpened)
                {
                    currCatalog = node->catId;
                }else{
                    currCatalog = -1; //No catalog or is not opened 
                }
            }else{
                currId = treeItem->id;  
                currCatalog = treeItem->indexOrCatId;
				
				if (!treeItem->childsLoaded){
					QList<QTreeWidgetItem *> children = treeItem->takeChildren();
					children.clear();
					addTreeItems(currCatalog, currId, treeItem, 0);
					treeItem->childsLoaded = true;
				}
				
				/*
				if (!treeItem->childsLoaded){
					CatTreeItem *parentItem = static_cast <CatTreeItem *>(treeItem->parent());
					if (parentItem && (parentItem->itemType != CATALOG_ELEMENT))
					{
						addTreeItems(currCatalog, currId, parentItem, 0);
						treeItem->childsLoaded = true;
					}
				}
				*/
            }
            //printQS("Item Index = " + QString::number(treeItem->indexOfChild(treeItem)));
        }
        emit newItemSelected(currCatalog, currId);
    }
}

void CatalogTree::itemIsExpanded(QTreeWidgetItem *item)
{
	CatTreeItem *it = static_cast <CatTreeItem *>(item);
    if (it)
    {
        if ((it->itemType == FOLDER_ELEMENT) && !it->iconId)
        {
            QPixmap *pix;
            pix = dm->im->getPixmap(0, FOLDER_OPENED_ICON);
            item->setIcon(0, QIcon(*pix));
            delete pix;
        }
    }
}

void CatalogTree::itemIsCollapsed(QTreeWidgetItem *item)
{
    CatTreeItem *it = static_cast <CatTreeItem *>(item);
    if (it)
    {
        if ((it->itemType == FOLDER_ELEMENT) && !it->iconId)
        {
           QPixmap *pix;
           pix = dm->im->getPixmap(0, FOLDER_CLOSED_ICON);
        	item->setIcon(0, QIcon(*pix));
        	delete pix;
        }
    }
}


void CatalogTree::takeOffCatalogItems(CatTreeItem *item)
{
	QList<QTreeWidgetItem *> items = item->takeChildren();   
    while (!items.isEmpty())
        delete items.takeFirst();
}
        
void CatalogTree::closeCatalog()
{
    CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
    int index = item->indexOrCatId;
    catalogNode *node = dm->cat->items.at(index);
    
    //clear();    
    takeOffCatalogItems(item);
    QPixmap *pix = 0;
    pix = dm->im->getDefaultPixmap(CATALOG_ELEMENT, 0);
    item->setIcon(0, QIcon(*pix));
    delete pix;
    node->isOpened = false;
    if (node->isReadProtected)
        node->isReadUnlocked = false;
	if (node->isWriteProtected)
        node->isWriteUnlocked = false;
    emit newItemSelected(-1, ROOT_ELEMENT);
}

void CatalogTree::openCatalog()
{
	inter = 0;
	printQS(QTime::currentTime().toString());
    dm->db.open();
    CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
    int index = item->indexOrCatId;
    catalogNode *node = dm->cat->items.at(index);
    /*if (node->isOpened)
		return;
	*/
    if (node->isReadProtected)
    {
        // TODO: Use self made password dialog with Icon :)
        bool ok;
     	QString userPass = QInputDialog::getText(this, tr("Password protected item"), tr("This catalog is protected with password.\nThis prevents unauthorized person to view its content.\nPlease enter your password to unlock it."), QLineEdit::Password, "", &ok);
        if (ok && !userPass.isEmpty())
        {
            if (!dm->cat->authRead(*node, userPass)){
                    QMessageBox::critical(this, tr("Wrong Password"), tr("The password is wrong!"));
            	return;
            }
        }
    }
    node->isOpened = true;
	maxLevel = -1;
    addTreeItems(item->id, 0, item, 0);
    
    QPixmap *pix = 0;
    if (node->iconId)
        pix = dm->im->getPixmap(node->catId, node->iconId);
    else
        pix = dm->im->getDefaultPixmap(CATALOG_ELEMENT, 1);
    
    item->setIcon(0, QIcon(*pix));
    delete pix;

    emit newItemSelected(item->id, CATALOG_ELEMENT);
	printQS("Inter = " + QString::number(inter));
	printQS(QTime::currentTime().toString());
}

void CatalogTree::renameItem()
{
    CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
    bool ok;
    QString userInput = QInputDialog::getText(this, tr("Dummy rename dialog!"), tr("Please enter new name for this item"), QLineEdit::Normal, item->text(0), &ok);
    if (ok && !userInput.isEmpty())
    {
		if (item->itemType != CATALOG_ELEMENT)
		{
         	// Rename ordinary element - dbNode   
            if (userInput == item->text(0))
                return;
            if (dm->dbn->renameNode(item->indexOrCatId, item->id, userInput))
            {
                item->setText(0, userInput);
            }
			
		}else{
            // Rename Catalog element - catalogClass
            catalogNode *node = dm->cat->items.at(item->indexOrCatId);
			if (userInput == node->catName)
				return;
            if (dm->cat->renameCatalog(node->catId, userInput))
			{
				item->setText(0, userInput);
				node->catName = userInput;
			}
			
		}
    }
    
}

void CatalogTree::createCatalog()
{
	CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
	bool ok;
	QString userInput = QInputDialog::getText(this, tr("Add New Catalog"), tr("Please enter catalog name.\nIt must be unique among the other catalogs."), QLineEdit::Normal, "", &ok);
	
	if (ok && !userInput.isEmpty())
	{
		//TODO: this way is rude. if there are catalogs with pass and are opened will be closed. 
		// Make the hard way
		catalogNode data;
        dm->cat->initGivenData(data);
		data.catName = userInput;
        data.dateTime = QDateTime::currentDateTime().toTime_t();
        dm->cat->createCatalog(data);
		refreshItems();
	}
    
}

void CatalogTree::addNewGroup()
{
    CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
    if (item->itemType == -1)
    {
        QMessageBox::critical(this, tr("No available catalog"), tr("There is no catalog available!\nPlease create one first."));   
        return;
    }

    
    int catId = 0;
	int parentId = 0;
	int catIndex = 0;
	if (item->itemType == CATALOG_ELEMENT)
	{
        catId = item->id;
		catIndex = item->indexOrCatId;
	}else{
        catId = item->indexOrCatId;
		parentId = item->id;
	}
    catalogNode *node = dm->cat->items.at(catIndex);
    if (node->isWriteProtected && !node->isWriteUnlocked)
    {
     	// Write Unlock
        bool passOk;
        QString userPass = QInputDialog::getText(this, tr("Enter Password"), tr("This catalog is writeprotected.\nPlease enter correct password to unlock it.\n"), QLineEdit::Normal, "", &passOk);
        if (passOk && !userPass.isEmpty())
        {
            if (!dm->cat->authWrite(*node, userPass))
            {
                QMessageBox::critical(this, tr("Wrong Password"), tr("The password is wrong!"));
                return;
        	}
        	node->isWriteUnlocked = true;
        }else{
            return;
        }
    }
    bool ok;
    QString userInput = QInputDialog::getText(this, tr("Add New Disk Group"), tr("Please enter name for the new group"), QLineEdit::Normal, "Disk Group", &ok);
    if (ok && !userInput.isEmpty())
    {
         
        dbNode data;
        dm->dbn->initGivenData(data);
		data.nodeName = userInput;
		data.catId = catId;
		data.parent = parentId;
        dm->dbn->createGroup(data);
		takeOffCatalogItems(item);
		addTreeItems(catId, parentId, item);

        //refreshItems();
    }

}

void CatalogTree::addNewDisk()
{
    CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
    if (item->itemType == -1)
    {
     	QMessageBox::critical(this, tr("No available catalog"), tr("There is no catalog available!\nPlease create one first."));   
        return;
    }
    
    int catId = 0;
    int parentId = 0;
	int catIndex = 0;
	QString parentName = item->text(0);
    if (item->itemType == CATALOG_ELEMENT)
	{
        catId = item->id;
		catIndex = item->indexOrCatId;
	}else{
        catId = item->indexOrCatId;
        parentId = item->id;
    }
	
	//TODO: Check is WriteProtected!!!
	if ((item->itemType != CATALOG_ELEMENT) && (item->itemType != GROUP_ELEMENT))
	{
		CatTreeItem *suitableItem = findSuitableParentForDisk(item);
		if (!suitableItem)
			return; // No suitable item for scan. Aborting
		this->setCurrentItem(suitableItem);
		// Very bad way of realisation this check, but I'm sleepy and can not think:(
		// Now we are on suitable item which will become parent for this scan, so we can 
		// execute addNewDisk again
		addNewDisk();
		return;
	}
	
    MediaScanUI *ms = new MediaScanUI(dm, this);
    ms->setCatId(catId);
    ms->setParentId(parentId);
	ms->setParentName(parentName);
    ms->loadItems();
    ms->exec();
    delete ms;	
    takeOffCatalogItems(item);
    addTreeItems(catId, parentId, item);
    
}

CatTreeItem* CatalogTree::findSuitableParentForDisk(CatTreeItem *item)
{
	if (item)
	{
		CatTreeItem *parentItem = static_cast <CatTreeItem *> (item->parent());
		if ((parentItem->itemType != CATALOG_ELEMENT) && (parentItem->itemType != GROUP_ELEMENT))
			return findSuitableParentForDisk(parentItem);
		else
			return parentItem;
	}else{
		return NULL;
	}
}

void CatalogTree::deleteCatalog()
{
	CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
    
	int catId = item->id;
	int index = item->indexOrCatId;
	
	if (QMessageBox::warning(this, tr("Delete disk!"), tr("Do you REALLY want to delete this catalog?\nThis can not be undone!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
	{
        dm->cat->deleteCatalog(catId);
		takeOffCatalogItems(item);
		//items.removeAt(index);
		delete item;
	}
	if (!this->topLevelItemCount() > 0)
		refreshItems();
}

void CatalogTree::deleteDisk(bool dontAsk)
{
    CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
    
    int catId = item->indexOrCatId;
    int itemId = item->id;
	if (dontAsk || (QMessageBox::warning(this, tr("Delete disk!"), tr("Do you REALLY want to delete this disk?\nThis can not be undone!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes))
	{
		printQS("Delete Disk");
		dbNode data;
        dm->dbn->readNode(catId, itemId, data);
        dm->dbn->deleteDisk(data);
		delete item;
	}
}

void CatalogTree::deleteGroup(bool dontAsk)
{
	CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
	int catId = item->indexOrCatId;
	int itemId = item->id;
	
	if (dontAsk || (QMessageBox::warning(this, tr("Delete disk!"), tr("Do you REALLY want to delete this Group?\nThis can not be undone!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes))
	{
		printQS("Delete Group");
		CatTreeItem *firstChildItem = 0;
		while (item->childCount() > 0)
		{
			firstChildItem = static_cast <CatTreeItem *> (item->child(0));
			if (firstChildItem->itemType == DISK_ELEMENT)
			{
				this->setCurrentItem(firstChildItem);
				deleteDisk(true);
			}
			if (firstChildItem->itemType == GROUP_ELEMENT)
			{
				this->setCurrentItem(firstChildItem);
				deleteGroup(true);
			}
			firstChildItem = static_cast <CatTreeItem *> (item->child(0));
		}
		dbNode data;
		data.catId = item->indexOrCatId;
		data.id = item->id;
        dm->dbn->deleteNode(data);
		delete item;
	}
}

void CatalogTree::deleteItem()
{
	CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
	int itemType = item->itemType;
    
	if (item->itemType == CATALOG_ELEMENT)
		deleteCatalog();
	if (itemType == GROUP_ELEMENT)
		deleteGroup();
	if (itemType == DISK_ELEMENT)
		deleteDisk();
}

void CatalogTree::updateItem(CatTreeItem *item)
{
	int catId = 0;
	int itemId = 0;
	//int itemType = item->itemType;
	QString itemName = item->text(0);
	if (item->itemType == CATALOG_ELEMENT)
	{
        // 
		catId = item->id;
		int catIndex = item->indexOrCatId;
        catalogNode *node = dm->cat->items.at(catIndex);
		item->setText(0, node->catName);
		QPixmap *pix;
		if (node->iconId)
            pix = dm->im->getPixmap(node->catId, node->iconId);
		else
            pix = dm->im->getDefaultPixmap(CATALOG_ELEMENT, 1);
		item->setIcon(0, QIcon(*pix));
		item->iconId = node->iconId;
		delete pix;
	}else{
		catId = item->indexOrCatId;
		itemId = item->id;
		dbNode data;
        dm->dbn->readNode(catId, itemId, data);
		item->setText(0, data.nodeName);
        item->fullType = data.nodeAttr;
		QPixmap *pix = 0;
		int iconId = data.iconId;
		switch (data.nodeType)
		{
			case GROUP_ELEMENT: 
			{
				if (!iconId)
				{
                    pix = dm->im->getDefaultPixmap(GROUP_ELEMENT, 0);
				}else{
                    pix = dm->im->getPixmap(catId, iconId);
				}
				if (pix)
					item->setIcon(0, QIcon(*pix));
			}
			break;
			case DISK_ELEMENT: 
			{
				if (!iconId)
				{
                    pix = dm->im->getDefaultPixmap(DISK_ELEMENT, item->fullType);
				}else{
                    pix = dm->im->getPixmap(catId, iconId);
				}
				if (pix)
					item->setIcon(0, QIcon(*pix));
			}
			break;
			case FOLDER_ELEMENT: 
			{
				if (!iconId)
				{
                    if (item->isExpanded())
                        pix = dm->im->getDefaultPixmap(FOLDER_ELEMENT, 1);
                	else
                        pix = dm->im->getDefaultPixmap(FOLDER_ELEMENT, 0);
                }else{
                    pix = dm->im->getPixmap(catId, iconId);
				}
				if (pix)
					item->setIcon(0, QIcon(*pix));
			}
			break;
		}
		item->iconId = iconId;
		delete pix;
	}
	
}

void CatalogTree::catalogProperties()
{
	CatTreeItem *item = static_cast<CatTreeItem *>(this->currentItem());
    
	int catId = 0;
	int itemId = 0;
	int itemType = item->itemType;
	int catIndex = 0;
	QString parentName = item->text(0);
	if (item->itemType == ROOT_ELEMENT)
	{
		catId = item->id;
		catIndex = item->indexOrCatId;
		
	}else{
		catId = item->indexOrCatId;
		itemId = item->id;
	}
	
	dbNode data;
    dm->dbn->initGivenData(data);
	
	// TODO: See logic. Make use default icon
    CatalogPropertiesDlg dlg(dm, this);
    if (itemType == CATALOG_ELEMENT)
    {
        //dlg.setCatalogClass(dm->cat);
        dlg.setCatalogNode(dm->cat->items.at(catIndex));
        dlg.setFlagsAndAttr(0, 1); // Use default enabled icon
    }else{
		//dlg.setCatalogClass(NULL);
		dlg.setCatalogNode(NULL);
        dm->dbn->readNode(catId, itemId, data);
        if (itemType == FOLDER_ELEMENT)
        {
            // We check if current Folder is expanded so propeirly to set nodeAttributes
            // Because it is used to determine if folder is open or closed
            // Stupid comment! :)
            if (item->isExpanded())
            {
                dlg.setFlagsAndAttr(data.nodeFlags, 1); // User opened folder
            }else{
                dlg.setFlagsAndAttr(data.nodeFlags, 0); // Use closed folder
            }
            
        }else{
            // The item is Disk or Disk_Group - No open and close images
            dlg.setFlagsAndAttr(data.nodeFlags, data.nodeAttr);
        }
		
	}
    
	dlg.customInit(itemType, catId, itemId, item->iconId);
	if (dlg.exec() == QDialog::Accepted)
	{
		if (dlg.isChanged())
		{
			updateItem(item);
		}
	}
    
}

void CatalogTree::goUpOneLevel()
{
    QTreeWidgetItem *newItem = 0;
    QTreeWidgetItem *oldItem = 0;
    oldItem = this->currentItem();
    newItem = oldItem->parent();
    if (newItem)
    {
        this->setCurrentItem(newItem);
    	//emit catTreeItemChanged(newItem, oldItem);
    }
    
}

void CatalogTree::goDownOneLevel(int _id)
{
    CatTreeItem *currItem = static_cast <CatTreeItem *>(this->currentItem());
    CatTreeItem *level = 0;
    
    for (int i = 0; i < currItem->childCount(); i++)
    {
     	CatTreeItem *child = static_cast<CatTreeItem *>(currItem->child(i));
        if (child->id == _id)
        {
            level = child;
            break;
        }
    }
    if (level)
    {
     	this->setCurrentItem(level);   
    }
    
}

CatTreeItem* CatalogTree::_findItemByItemId(CatTreeItem *parent, int id)
{
    CatTreeItem *item = 0;
    for (int i = 0; i < parent->childCount(); i++)
    {
        CatTreeItem *ci = static_cast <CatTreeItem *>(parent->child(i));
        //printQS(ci->text(0));
     	if (ci->id == id)
        {
         	item = ci;
            break;   
        }
        if (ci->childCount() > 0)
		{
			CatTreeItem *tempItem = _findItemByItemId(ci, id);
			if (tempItem)
            	return tempItem;
		}
    }
    return item;
}

void CatalogTree::findItemByItemId(int catId, int id)
{
    CatTreeItem *item = 0;
    for(int i = 0; i < this->topLevelItemCount(); i++)
    {
        item = static_cast<CatTreeItem *>(topLevelItem(i));
        if (item->id == catId){
            
         	setCurrentItem(_findItemByItemId(item, id));
            break;
        }
    }
}

int CatalogTree::getCurrentCatalog()
{
    CatTreeItem *item = static_cast<CatTreeItem *>(currentItem());
    if (item->itemType == CATALOG_ELEMENT)
        return item->id;
    else
        return item->indexOrCatId;
}

void CatalogTree::createActions()
{
    
    pOpenCatalogAct = new QAction(QIcon(":icons/interface/open.png"), tr("&Open catalog"), this);
    pOpenCatalogAct->setStatusTip(tr("Opens this catalog."));
    connect(pOpenCatalogAct, SIGNAL(triggered()), this, SLOT(openCatalog()));
    
    pCloseCatalogAct = new QAction( tr("&Close catalog"), this);
    pCloseCatalogAct->setStatusTip(tr("Close this catalog."));
    connect(pCloseCatalogAct, SIGNAL(triggered()), this, SLOT(closeCatalog()));
 	   
    pRenameItemAct = new QAction(tr("Rename"), this);
    connect(pRenameItemAct, SIGNAL(triggered()), this, SLOT(renameItem()));
    
    pDeleteItemAct = new QAction(QIcon(":icons/interface/remove.png"), tr("Delete"), this);
    connect(pDeleteItemAct, SIGNAL(triggered()), this, SLOT(deleteItem()));
    
    pAddNewGroupAct = new QAction(QIcon(":icons/interface/add.png"), tr("Add new Group"), this);
    connect(pAddNewGroupAct, SIGNAL(triggered()), this, SLOT(addNewGroup()));
    
    pAddNewDiskAct = new QAction(QIcon(":icons/interface/add.png"), tr("Add new Disk Here"), this);
	connect(pAddNewDiskAct, SIGNAL(triggered()), this, SLOT(addNewDisk()));
    
    pUpdateDiskAct = new QAction(tr("Update this Disk"), this);
    
    pPropertiesAct = new QAction(tr("Properties"), this);
	connect(pPropertiesAct, SIGNAL(triggered()), this, SLOT(catalogProperties()));
	
	
	pAddCatalogAct = new QAction(QIcon(":/icons/db_icons/catalog_enabled.png"), tr("Create New Catalog"), this);
	connect(pAddCatalogAct, SIGNAL(triggered()), this, SLOT(createCatalog()));
	/*
	QPixmap *pix = dm->im->getDefaultPixmap(CATALOG_ELEMENT, 1);
	pAddCatalogAct = new QAction(QIcon(*pix), tr("Create New Catalog"), this);
	connect(pAddCatalogAct, SIGNAL(triggered()), this, SLOT(createCatalog()));
	delete pix;
    */
    pLoadCatalogAct = new QAction(QIcon(":icons/interface/open.png"), tr("Load Catalog"), this);
    connect(pLoadCatalogAct, SIGNAL(triggered()), this, SIGNAL(loadCatalog()));
    
}


void CatalogTree::createMenus()
{
    pCatalogPopupMenu = new QMenu(this); 
	pCatalogPopupMenu->addAction(pAddCatalogAct);
    pCatalogPopupMenu->addAction(pAddNewGroupAct);
    pCatalogPopupMenu->addAction(pAddNewDiskAct);
    pCatalogPopupMenu->addAction(pRenameItemAct);
    pCatalogPopupMenu->addAction(pDeleteItemAct);
    pCatalogPopupMenu->addSeparator();
    pCatalogPopupMenu->addAction(pOpenCatalogAct);
    pCatalogPopupMenu->addAction(pCloseCatalogAct);
    pCatalogPopupMenu->addSeparator();
    pCatalogPopupMenu->addAction(pPropertiesAct);
    
    pGroupPopupMenu = new QMenu(this);
    pGroupPopupMenu->addAction(pAddNewGroupAct);
    pGroupPopupMenu->addAction(pAddNewDiskAct);
    pGroupPopupMenu->addAction(pRenameItemAct);
    pGroupPopupMenu->addAction(pDeleteItemAct);
    pGroupPopupMenu->addSeparator();
    pGroupPopupMenu->addAction(pPropertiesAct);
    
    pDiskPopupMenu = new QMenu(this);
    pDiskPopupMenu->addAction(pAddNewDiskAct);
    pDiskPopupMenu->addAction(pUpdateDiskAct);
    pDiskPopupMenu->addAction(pRenameItemAct);
    pDiskPopupMenu->addAction(pDeleteItemAct);
    pDiskPopupMenu->addSeparator();
    pDiskPopupMenu->addAction(pPropertiesAct);
    
    pFolderPopupMenu = new QMenu(this);
    pFolderPopupMenu->addAction(pAddNewDiskAct);
    //pFolderPopupMenu->addAction(pRenameItemAct);
    pFolderPopupMenu->addAction(pDeleteItemAct);
    pFolderPopupMenu->addSeparator();
    pFolderPopupMenu->addAction(pPropertiesAct);
   
    pCreateCatalogMenu = new QMenu(this);
    pCreateCatalogMenu->addAction(pAddCatalogAct);
    
    pLoadCatalogMenu = new QMenu(this);
    pLoadCatalogMenu->addAction(pLoadCatalogAct);
}

void CatalogTree::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu *popupMenu = NULL;
    
    CatTreeItem *item = static_cast<CatTreeItem *>(itemAt(event->pos()));
	if (!item)
	{
		return;
	}
    int catId = 0;
	if (item->itemType == ROOT_ELEMENT)
	{
		catId = item->id;
		int catIndex = item->indexOrCatId;
        catalogNode *node = dm->cat->items.at(catIndex);
		if (!node->isOpened)
		{
			pAddCatalogAct->setEnabled(true);
			pAddNewGroupAct->setEnabled(false);
			pAddNewDiskAct->setEnabled(false);
			pRenameItemAct->setEnabled(false);
			pDeleteItemAct->setEnabled(false);
			pOpenCatalogAct->setEnabled(true);
			pCloseCatalogAct->setEnabled(false);
			pPropertiesAct->setEnabled(false);
			
		}else{
			pAddCatalogAct->setEnabled(true);
			pAddNewGroupAct->setEnabled(true);
			pAddNewDiskAct->setEnabled(true);
			pRenameItemAct->setEnabled(true);
			pDeleteItemAct->setEnabled(true);
			pOpenCatalogAct->setEnabled(false);
			pCloseCatalogAct->setEnabled(true);
			pPropertiesAct->setEnabled(true);
		}
		
	}else{
		catId = item->indexOrCatId;
		pAddNewGroupAct->setEnabled(true);
		pAddNewDiskAct->setEnabled(true);
		pRenameItemAct->setEnabled(true);
		pDeleteItemAct->setEnabled(true);
		pPropertiesAct->setEnabled(true);
		pUpdateDiskAct->setEnabled(true);
		
	}
	
	
	
    //printQS(QString::number(item->itemType));
    switch (item->itemType)
    {
        case -2:
            popupMenu = pLoadCatalogMenu;
            break;
        case -1:
            popupMenu = pCreateCatalogMenu;
            break;
        case ROOT_ELEMENT:
            popupMenu = pCatalogPopupMenu;
            
            break;
        case GROUP_ELEMENT:
            popupMenu = pGroupPopupMenu;
            break;
        case DISK_ELEMENT:
            popupMenu = pDiskPopupMenu;
            break;
        case FOLDER_ELEMENT:
            popupMenu = pFolderPopupMenu;
            break;
        default:
            popupMenu = NULL;
    }
    
    if (popupMenu)
    	popupMenu->exec(event->globalPos());
    
}

void CatalogTree::changeDataModule(QCatDataModuleClass *newDM)
{
	clear();
    dm = newDM;
    loadCatalogs();
}

void CatalogTree::clearAllTreeItems()
{
    CatTreeItem *item = 0;
    while (item = static_cast <CatTreeItem *>(takeTopLevelItem(0)))
    {
        delete item;
    }
}


void CatalogTree::dragEnterEvent(QDragEnterEvent * event)
{
	printQS("Entering dragEnterEvent()");
    if (event->mimeData()->hasText() 
        && (
            (event->mimeData()->text() == "LocalDrag") || (event->mimeData()->text() == "RightPanelDrag")
           ))
	{
		event->setDropAction(Qt::MoveAction);
		event->accept();
        
	}else{
     	event->ignore();   
    }
}

void CatalogTree::dragMoveEvent(QDragMoveEvent * event)
{
    bool localDrag;
	if (event)
	{
		if (event->mimeData()->hasText())
		{
			if (event->mimeData()->text() == "RightPanelDrag")
				localDrag = false;
			if (event->mimeData()->text() == "LocalDrag")
				localDrag = true;
		}
		event->ignore();
		CatTreeItem *dropPlaceItem = static_cast <CatTreeItem *>(itemAt(event->pos()));
		if (!dropPlaceItem)
			return;
		
			// Check dropTarget Catalog
		int dropCatalog = -1;
		if (dropPlaceItem->itemType == CATALOG_ELEMENT)
			dropCatalog = dropPlaceItem->id;
		else
			dropCatalog = dropPlaceItem->indexOrCatId;
		
		int dragCatalog = -1;
		CatTreeItem *tmpItem = static_cast <CatTreeItem *>(this->currentItem());
		if (tmpItem->itemType == CATALOG_ELEMENT)
			dragCatalog = tmpItem->id;
		else
			dragCatalog = tmpItem->indexOrCatId;
		
		if (dragCatalog != dropCatalog)
			return;
		
		if (dropPlaceItem == dropTargetItem)
		{
			autoOpenTimer->start(AUTO_OPEN_INTERVAL);
			//event->accept();
			//return;
		}
		else
		{
			stopAnimate();
			if (!dropTargetItemIcon.isNull())
				dropTargetItem->setIcon(0, dropTargetItemIcon);
			
			dropTargetItem = dropPlaceItem;
			autoOpenTimer->start(AUTO_OPEN_INTERVAL);
            startAnimate();
		}

        if (localDrag) // Doing drag'n'drop from current widget
        {
			if (dropPlaceItem && draggedItem)
            {
				if ((dropPlaceItem->itemType == GROUP_ELEMENT) || (dropPlaceItem->itemType == CATALOG_ELEMENT))
				{
					if (!draggedItem->contains(dropPlaceItem))
                    {
                        event->setDropAction(Qt::MoveAction);
                        event->accept();
                    }else{
                        event->ignore();
                    }
                }else{
                    event->ignore();
                }
        
            }
        }else{	//Doing drag'n'drop from CatalogTable widget - right panel

			if ((dropPlaceItem->itemType == CATALOG_ELEMENT) || (dropPlaceItem->itemType == GROUP_ELEMENT))
			{
				event->accept();
			}else{
				event->ignore();
			}
        }
		startAutoScroll();
        
	}
}

void CatalogTree::dragLeaveEvent()
{
	printQS("Entering dragLeaveEvent()");
	stopAnimate();
}


void CatalogTree::dropEvent(QDropEvent * event)
{
    if (event->source() == this)
    {
        CatTreeItem *_currentItem = static_cast <CatTreeItem *>(itemAt(event->pos()));
		if (_currentItem == draggedItem)
			return;
        
        //printQS("Drop site = " + _currentItem->text(0));

		if (moveCatTreeItem(draggedItem, _currentItem, true))
			printQS("Moved!");
		else
			printQS("Failed move!");
    }else{
        printQS("Using remote drag'n'Drop");
		QString csvData = QString(event->mimeData()->data("CSVList"));
		printQS(csvData);
		QStringList strList = csvData.split(":", QString::SkipEmptyParts);
		printQS(QString("Moving %1 items").arg(strList.count()));
		
		CatTreeItem *currentItem = static_cast <CatTreeItem *>(this->currentItem());
		CatTreeItem *dropPlace = static_cast <CatTreeItem *>(this->itemAt(event->pos()));
		
		for (int i = 0; i < strList.count(); i++)
		{
			int idForMove = strList.at(i).toInt();
			printQS(QString("moving item %1").arg(idForMove));
			CatTreeItem *it = NULL;
			int childNo = -1;
			for (int j = 0; j < currentItem->childCount(); j++)
			{
				it = static_cast <CatTreeItem *>(currentItem->child(j));
				if (it->id == idForMove)
				{
					printQS("moving item " + it->text(0));
					childNo = j;
					break;
				}
			}
			if (childNo != -1)
			{
				if (moveCatTreeItem(it, dropPlace, true))
					printQS("Moved!");
				else
					printQS(QString("Failed to move item %1").arg(it->text(0)));

			}
		}
		
		
		
    }
	event->setDropAction(Qt::MoveAction);
	event->accept();
    stopAnimate();
   
}

void CatalogTree::mousePressEvent(QMouseEvent * event)
{
	//printQS("Entering mousePressEvent()");
	if (event->button() == Qt::LeftButton)
		dragStartingPos = event->pos();
	QTreeWidget::mousePressEvent(event);
}

void CatalogTree::mouseMoveEvent(QMouseEvent * event)
{
	//printQS("Entering mouseMoveEvent()");
	if (event->buttons() & Qt::LeftButton)
	{
		int distance = (event->pos() - dragStartingPos).manhattanLength();
		if (distance >= QApplication::startDragDistance())
			startDrag();
	}
	QTreeWidget::mouseMoveEvent(event);
}

void CatalogTree::startDrag()
{
	//printQS("Entering startDrag()");
	draggedItem = static_cast <CatTreeItem *>(currentItem());
	if ((draggedItem->itemType == GROUP_ELEMENT) || (draggedItem->itemType == DISK_ELEMENT))
	{
		QMimeData *mimeData = new QMimeData;
        QString strData = "LocalDrag";
		mimeData->setText(strData);
		QDrag *drag = new QDrag(this);
		drag->setMimeData(mimeData);
		drag->setPixmap(draggedItem->icon(0).pixmap(22, 22));
		if (drag->start(Qt::MoveAction) == Qt::MoveAction)
			printQS("Drage Moved");
        autoOpenTimer->stop();
        stopAnimate();
        if (!dropTargetItemIcon.isNull() && dropTargetItem)
            dropTargetItem->setIcon(0, dropTargetItemIcon);
    }
}

void CatalogTree::autoOpenTimerFired()
{
	if (dropTargetItem)
		dropTargetItem->setExpanded(true);
	autoOpenTimer->stop();
}

void CatalogTree::animate()
{
    if (dropTargetItem)
    {
        dropTargetItem->setIcon(0, QIcon(":icons/openfolder/openfolder-act" + QString::number(currAnimFrame)));
        currAnimFrame++;
        if (currAnimFrame > 8)
            currAnimFrame = 1;
    }
}

void CatalogTree::startAnimate()
{
	dropTargetItemIcon = dropTargetItem->icon(0);
    animTimer->start(50);
}

void CatalogTree::stopAnimate()
{
    animTimer->stop();
	if (!dropTargetItemIcon.isNull())
		dropTargetItem->setIcon(0, dropTargetItemIcon);
}

void CatalogTree::stopAnimation()
{
	if (!dropTargetItemIcon.isNull())
		dropTargetItem->setIcon(0, dropTargetItemIcon);
	stopAnimate();
}




bool CatalogTree::moveCatTreeItem(CatTreeItem * itemForMove, CatTreeItem * newParentItem, bool updateDb)
{
	if (itemForMove && newParentItem && !itemForMove->contains(newParentItem))
	{
        if (itemForMove == newParentItem)
            return true;
		QTreeWidgetItem *parent = itemForMove->parent();
		int childIndex = parent->indexOfChild(itemForMove);
		CatTreeItem *orphonChild = static_cast <CatTreeItem *>(parent->takeChild(childIndex));
		newParentItem->addChild(orphonChild);
		setCurrentItem(orphonChild);
		if (updateDb)
		{
			int newParentId = 0;
			if (newParentItem->itemType != CATALOG_ELEMENT)
				newParentId = newParentItem->id;
			dm->dbn->updateNodeParent(orphonChild->indexOrCatId, orphonChild->id, newParentId);
		}
		return true;
	}
	return false;
}