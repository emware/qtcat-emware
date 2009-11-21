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
#include <QHeaderView>
#include <QDateTime>
#include <QMouseEvent>
#include <QApplication>

#include "defsandtools.h"
#include "datamodule.h"
#include "catalogtable.h"
#include "dbnode.h"
#include "configmanager.h"
#include "iconmanager.h"

CatalogTable::CatalogTable(QCatDataModuleClass *_dm, QWidget *parent)
	: dm(_dm), QTreeWidget(parent)
{
	init();
}

CatalogTable::~CatalogTable()
{
    
}

void CatalogTable::init()
{	
    currId = 0;
    clearTree = false;
    items.clear();
    
    folderHeader = new QStringList;
    *folderHeader << tr("Name") << tr("Ext") << tr("Size") << tr("Date") << tr("Time") << tr("Attr") << tr("Description") << tr("Owner") << tr("Group") << tr("Flags") << tr("Disk No");
    diskHeader = new QStringList;
    *diskHeader << tr("Disk Name") << tr("Media Type") << tr("Capacity") << tr("Last update") << tr("Description") << tr("Disk No") << tr("Disk Location") << tr("Location");

    this->setHeaderLabels(*diskHeader);
    this->setSortingEnabled(true);
    this->setRootIsDecorated(false);
    this->setAlternatingRowColors(true);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->header()->setResizeMode(QHeaderView::Interactive);
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setIconSize(QSize(22, 22));
    this->sortByColumn(1, Qt::AscendingOrder);

    connect(this, SIGNAL(currentItemChanged (QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(itemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(this->header(), SIGNAL(sectionResized(int, int, int)), this, SLOT(columnResized(int, int, int)));
    connect(this, SIGNAL(itemActivated(QTreeWidgetItem *, int)), this, SLOT(vItemActivated(QTreeWidgetItem *, int)));

}

void CatalogTable::changeDataModule(QCatDataModuleClass *_newDM)
{
    clearAllItems();
    dm = _newDM;
}

void CatalogTable::itemChanged(QTreeWidgetItem *newItem, QTreeWidgetItem *prevItem)
{
    if (!clearTree)
    {
        int no = 0;
        if (newItem)
            currId = static_cast<CatTreeItem *>(newItem)->id;
        if (newItem)
            no = static_cast<CatTreeItem *>(newItem)->indexOrCatId;
        
        if ((currId >= 0) && (no >= 0))
        {
            emit newItemSelected(currId);
            emit newItemSelected(items.at(no)->nodeDesc);
        }
    }
}

void CatalogTable::refreshItems(int newCatalog, int parentId)
{
    clearAllItems();
    if (newCatalog == -1)
        return;
    int parentType = dm->dbn->getNodeType(newCatalog, parentId, false);
    
    canResize = false;
    switch(parentType)
    {
        case ROOT_ELEMENT:
        case GROUP_ELEMENT:
            bDiskColumns = false;
            this->setHeaderLabels(*diskHeader);
            break;
        case DISK_ELEMENT:
        case FOLDER_ELEMENT:
            bDiskColumns = true;
            this->setHeaderLabels(*folderHeader);
            break;
        default:
            this->setHeaderItem(NULL);
            break;
    }
    
    for (int i = 0; i < this->header()->count(); ++i)
    {
        switch(parentType)
        {
            case ROOT_ELEMENT:
            case GROUP_ELEMENT:
                if (dm->settings->isCatalogColumnVisible(i))
                    this->setColumnHidden(i, false);
                else
                {
					//printQS(QString("Columt %1 is hidden").arg(i));
                    this->setColumnHidden(i, true);
                }
                this->header()->resizeSection(i, dm->settings->catColumnSizes[i]);
                break;
            case DISK_ELEMENT:
            case FOLDER_ELEMENT:
                if (dm->settings->isDiskColumnVisible(i))
                    this->setColumnHidden(i, false);
                else
                {
					//printQS(QString("Columt %1 is hidden").arg(i));
                    this->setColumnHidden(i, true);
                }
                this->header()->resizeSection(i, dm->settings->diskColumnSizes[i]);
                break;
        }
    }
    canResize = true;

    //if (!items)
    //    delete items;
	//items = dbn->readAllChildsForParent(newCatalog, parentId);
    dm->dbn->readAllChildsForParent(newCatalog, parentId, items);
    QDateTime dt;
    
    if (parentType != CATALOG_ELEMENT)
    {
        CatTreeItem *upDirItem = new CatTreeItem;
        upDirItem->itemType = UPDIR_ELEMENT;
        upDirItem->id = -1;
        upDirItem->indexOrCatId = -1;
        upDirItem->fullType = 0;
        upDirItem->iconId = 0;
        upDirItem->setText(0, "..");
        QPixmap *pix = dm->im->getDefaultPixmap(upDirItem->itemType, 0);
        upDirItem->setIcon(0, QIcon(*pix));
        delete pix;
        this->addTopLevelItem(upDirItem);
        
        //tableItems.append(upDirItem);
        
    }
    
    for (int i = 0; i < items.size(); ++i)
    {
        dbNode *pNode = items.at(i);
        dt.setTime_t(pNode->dateTime);
        CatTreeItem *item  = new CatTreeItem;
        item->itemType = pNode->nodeType;
        item->id = pNode->id;
        item->indexOrCatId = i;
        item->parentId = parentId;
        item->fullType = pNode->nodeAttr;
        item->iconId = pNode->iconId;
        vFileName data;
        parseFileName(pNode->nodeName, data);
        item->setText(0, data.fileName);
        if (data.hasExt)
            item->setText(1, data.fileExt);
        
        switch (pNode->nodeType)
        {
            case GROUP_ELEMENT: 
                //item->setIcon(0, QIcon(":/images/blockdevice.png"));
                item->setText(3, dt.toString(Qt::LocalDate));
                item->setText(4, pNode->nodeDesc);
                item->setText(5, "");
                break;
            case DISK_ELEMENT: 
                //item->setIcon(0, QIcon(":/images/dvd_unmount.png"));
                item->setText(2, QString::number(pNode->nodeSize));
                item->setText(3, dt.toString(Qt::LocalDate));
                item->setText(4, pNode->nodeDesc);
                item->setText(5, QString::number(pNode->diskNo));
                break;
            case FOLDER_ELEMENT: 
                //item->setIcon(0, QIcon(":/images/folder_green.png"));
                item->setText(3, dt.date().toString(Qt::LocalDate));
                item->setText(4, dt.time().toString(Qt::LocalDate));
                item->setText(5, permsToQString(pNode->nodeAttr));
                break;
            case FILE_ELEMENT: 
                //item->setIcon(0, QIcon(":/images/binary.png"));
                item->setText(2, QString::number(pNode->nodeSize));
                item->setText(3, dt.date().toString(Qt::LocalDate));
                item->setText(4, dt.time().toString(Qt::LocalDate));
                item->setText(5, permsToQString(pNode->nodeAttr));
                break;
        }
        
        QPixmap *pix = 0;
        if (item->iconId)
        {
            pix = dm->im->getPixmap(newCatalog, item->iconId);
            item->setIcon(0, QIcon(*pix));
        }else{
            if (item->itemType == FOLDER_ELEMENT)
                pix = dm->im->getDefaultPixmap(item->itemType, 0);
            else
                if (item->itemType == FILE_ELEMENT)
                    pix = dm->im->getPixmap(0, data.defIcon);
            else
                pix = dm->im->getDefaultPixmap(item->itemType, item->fullType);
            
            item->setIcon(0, QIcon(*pix));
        }
        delete pix;
        
        this->addTopLevelItem(item);
    }
    

}

void CatalogTable::clearAllItems()
{
    clearTree = true;
    this->clear();
    if (dm)
    {
        dm->dbn->clearNodeList(items);
    }
    clearTree = false;
}

void CatalogTable::columnResized(int index, int oldSize, int newSize)
{
    if (canResize)
    {
        if (bDiskColumns)
        {
            dm->settings->diskColumnSizes[index] = newSize;
        }else{
            dm->settings->catColumnSizes[index] = newSize;
        }
    }
}

void CatalogTable::vItemActivated(QTreeWidgetItem * activeItem, int column)
{
    CatTreeItem *item = static_cast<CatTreeItem *>(activeItem);
    if (item)
    {
        int itemType = item->itemType;
        if (itemType == UPDIR_ELEMENT)
        {
            emit goUp();   
            this->setCurrentItem(this->topLevelItem(0));
            return;
        }
        if (itemType == FILE_ELEMENT)
        {
            printQS("File activated!");   
        }else{
            emit goDown(item->id);   
            this->setCurrentItem(this->topLevelItem(0));
        }
        
    }
}

void CatalogTable::mousePressEvent(QMouseEvent * event)
{
    
    if (event->button() == Qt::LeftButton)
        dragStartingPos = event->pos();
    
    QTreeWidget::mousePressEvent(event);
}

void CatalogTable::mouseMoveEvent(QMouseEvent * event)
{
    if (!bDiskColumns)
    {
        if (event->buttons() & Qt::LeftButton)
        {
            int distance = (event->pos() - dragStartingPos).manhattanLength();
            if (distance >= QApplication::startDragDistance())
                startDrag();
        }
    }
    QTreeWidget::mouseMoveEvent(event);
}

void CatalogTable::contextMenuEvent(QContextMenuEvent * event)
{
}

void CatalogTable::dragMoveEvent(QDragMoveEvent * event)
{
}

void CatalogTable::dragEnterEvent(QDragEnterEvent * event)
{
}

void CatalogTable::startDrag()
{
    QMimeData *mimeData = new QMimeData;
    QString strData = "RightPanelDrag";
    selectedItemsList.clear();
    selectedItemsList << (this->selectedItems());
	QString selItems(NULL);
	for (int i = 0; i < selectedItemsList.count(); i++)
	{
		//CatTreeItem *item = static_cast <CatTreeItem *>(selectedItemsList.at(i));
		selItems += QString::number(static_cast <CatTreeItem *>(selectedItemsList.at(i))->id);
		selItems +=":";
	}
    char ptrCh[5];
    int listAddr = reinterpret_cast <int> (&selectedItemsList);
    memcpy(&ptrCh, &listAddr, 4);
    ptrCh[4] = 0x0;
    QByteArray ptr(ptrCh);
    
    mimeData->setText(strData);
    mimeData->setData("ListPtr", ptr);
	mimeData->setData("CSVList", selItems.toAscii());
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    if (drag->start(Qt::MoveAction) == Qt::MoveAction)
        printQS("Drage Moved");
	else
		emit canceledDrag();

}


