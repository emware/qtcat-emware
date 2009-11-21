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
#include <QString>
#include <QMessageBox>
#include <QXmlParseException>
#include <QXmlAttributes>


#include "whereisitxmlimportclass.h"
#include "defsandtools.h"
#include "dbnode.h"
#include "datamodule.h"
#include "progresswidget.h"

ImportTreeItem::ImportTreeItem(ImportTreeItem *parent) : QTreeWidgetItem(parent)
{
    // TODO: Init vars!
}

ImportTreeItem::~ImportTreeItem()
{
    
}

/**
 * Start Parser class
 * 
 * */

WhereIsItXmlImportClass::WhereIsItXmlImportClass()
{
    //currentItem = 0;
    //root = 0;
    _diskGroupsNum = 0;
    _disks = 0;
    _folders = 0;
    _files = 0;
	wc = NULL;
    justCount = true;
	
}


WhereIsItXmlImportClass::~WhereIsItXmlImportClass()
{
    
}

bool WhereIsItXmlImportClass::startElement(const QString &namespaceURI,
                                      const QString &localName,
                                      const QString &qName,
                                      const QXmlAttributes &attributes)
{
    if (qName == "REPORT")
    {
     	//delete root;
        //root = new WhereIsItXmlItem;
    }
    if (qName == "ITEM")
    {
        initGivenData(_data);
        QString itemType = attributes.value("ItemType");
        if (itemType == "DiskGroup")
            _data.itemType = GROUP_ELEMENT;
        else if (itemType == "Disk")
            _data.itemType = DISK_ELEMENT;
        else if (itemType == "Folder")
            _data.itemType = FOLDER_ELEMENT;
        else if (itemType == "File")
            _data.itemType = FILE_ELEMENT;
        
        if (justCount)
		{
			switch (_data.itemType)
			{
				case GROUP_ELEMENT:
					_diskGroupsNum++;
					break;   
				case DISK_ELEMENT:
					_disks++;
					break;   
				case FOLDER_ELEMENT:
					_folders++;
					break;   
				case FILE_ELEMENT:
					_files++;
					break;   
			}
		}
    }else{
     	currentText.clear();   
    }
    return true;
}

bool WhereIsItXmlImportClass::endElement(const QString &namespaceURI,
                						const QString &localName,
                						const QString &qName)
{
    if (justCount)
        return true;
    
    if (qName == "ITEM")
    {
        
        if (!justCount && wc)
        {
            wc->processItem(_data);
        }

    }else if (qName == "NAME")
        _data.name = currentText;
    else if (qName == "EXT")
        _data.ext = currentText;
    else if (qName == "SIZE")
        _data.size = currentText.toLongLong();
    else if (qName == "DATE")
        _data.date = currentText;
    else if (qName == "TIME")
        _data.time = currentText;
    else if (qName == "DISK_NAME")
        _data.diskName = currentText;
    else if (qName == "DISK_TYPE")
        _data.diskType = currentText;
    else if (qName == "PATH")
        _data.path = currentText;
    else if (qName == "CATEGORY")
        _data.category = currentText;
    else if (qName == "DESCRIPTION")
        _data.description = currentText;
    else if (qName == "DISK_NUM")
        _data.diskNum = currentText;
    else if (qName == "CRC")
        _data.crc = currentText.toInt();
    else if (qName == "FLAG")
        _data.flag = currentText;
    else if (qName == "DISK_LOCATION")
        _data.diskLocation = currentText;
    
    //printQS(qName + " => " + currentText);
    return true;
}

bool WhereIsItXmlImportClass::characters(const QString &str)
{
    currentText += str;
    return true;
}

bool WhereIsItXmlImportClass::fatalError(const QXmlParseException &exception)
{
    
    QMessageBox::warning(0, QObject::tr("SAX Handler"),
                         QObject::tr("Parse error at line %1, column "
                                 "%2:\n%3.")
                                 .arg(exception.lineNumber())
                                 .arg(exception.columnNumber())
                                 .arg(exception.message()));
    
    return true;
}

void WhereIsItXmlImportClass::setWorkClass(Work_ImportWhereIsItItem *_wc)
{
    wc = _wc;   
}

void WhereIsItXmlImportClass::initGivenData(xmlItem & data)
{
    data.itemType = 0;
    data.name.clear();
    data.ext.clear();
    data.size = 0;
    data.date.clear();
    data.time.clear();
    data.diskName.clear();
    data.diskType.clear();
    data.path.clear();
    data.category.clear();
    data.description.clear();
    data.diskNum.clear();
    data.crc = 0;
    data.flag.clear();
    data.diskLocation.clear();

}


/**
 * Start Work Class
 * 
 * 
 * */


Work_ImportWhereIsItItem::Work_ImportWhereIsItItem(QCatDataModuleClass *_dm, int _catId) : dm(_dm), catId(_catId)
{
	pw = 0;
    progressBarNo = 0;
    parseMode = 0;
}

Work_ImportWhereIsItItem::~Work_ImportWhereIsItItem()
{
    
}

bool Work_ImportWhereIsItItem::processItem(xmlItem &xmlData)
{

    dbNode data;
    dm->dbn->initGivenData(data);
    if (!xmlData.ext.isEmpty())
    {
     	data.nodeName = xmlData.name;
        data.nodeName.append('.');
        data.nodeName += xmlData.ext;   
    }else{
        data.nodeName = xmlData.name;
    }
    data.nodeType = xmlData.itemType;
    data.nodeDesc = xmlData.description;
    data.catId = catId;
    if (data.nodeType == FILE_ELEMENT)
        data.nodeSize = xmlData.size;
    
	QString combDT = xmlData.date + "*" + xmlData.time;
	QDateTime _dt = QDateTime::fromString(combDT, "yyyy-MM-dd*hh:mm:ss");
    data.dateTime = _dt.toTime_t();
	/*
	if (data.nodeType != DISK_ELEMENT)
	{
		printQS(combDT);
	}*/
	
	if ((parseMode == PROCESS_GROUPS) && (data.nodeType == GROUP_ELEMENT))
	{
		data.parent = 0;
		dm->dbn->createGroup(data);
        if (pw)
            pw->stepProgressBar(progressBarNo);

	}
	if ((parseMode == PROCESS_DISKS) && (data.nodeType == DISK_ELEMENT))
	{
		if (!disksMap.contains(xmlData.diskNum))
		{
			ImportTreeItem *item = new ImportTreeItem;
			item->itemType = data.nodeType;
			item->itemName = data.nodeName;
			item->itemDateTime = data.dateTime;
			item->itemDescription = data.nodeDesc;
			item->itemAttr = getDiskFullTypeFromWhereIsIt(xmlData.diskType);
			item->itemFlags = mediaTypesArray[findIndexFromFullDiskType(item->itemAttr)].basicType;
			disksMap[xmlData.diskNum] = item;
            if (pw)
                pw->stepProgressBar(progressBarNo);

		}
	}
	if ((parseMode == PROCESS_FOLDERS) && (data.nodeType == FOLDER_ELEMENT))
	{
		QStringList pathElements = xmlData.path.split("\\");
		ImportTreeItem *disk = disksMap[xmlData.diskNum];
		
		ImportTreeItem *item = new ImportTreeItem;
		item->itemType = data.nodeType;
		item->itemName = data.nodeName;
		item->itemDateTime = data.dateTime;
		item->itemDescription = data.nodeDesc;
		item->itemAttr = 29781; // Def attributes for dirs. Correct Later
		item->itemFlags = 0;
        if (pw)
            pw->stepProgressBar(progressBarNo);

		
		if (pathElements.count() > 0)
		{
			ImportTreeItem *crFolder = disk;
			for(int i = 0; i < pathElements.count(); i++)
			{
				for (int j = 0; j < crFolder->childCount(); j++)
				{
					ImportTreeItem *_temp = static_cast<ImportTreeItem *>(crFolder->child(j));
					if (_temp->itemName == pathElements.at(i))
					{
						crFolder = _temp;
						break;
					}
				}
			}
			crFolder->addChild(item);
		}else{
			disk->addChild(item);	
		}
	}
	if ((parseMode == PROCESS_FILES) && (data.nodeType == FILE_ELEMENT))
	{
		ImportTreeItem *disk = disksMap[xmlData.diskNum];
		int diskNo = disk->dbDiskNo;
		QStringList pathElements = xmlData.path.split("\\");
		
		int parentId = 0;
		if (pathElements.count() > 0)
		{
			ImportTreeItem *crFolder = disk;
			for(int i = 0; i < pathElements.count(); i++)
			{
				for (int j = 0; j < crFolder->childCount(); j++)
				{
					ImportTreeItem *_temp = static_cast<ImportTreeItem *>(crFolder->child(j));
					if (_temp->itemName == pathElements.at(i))
					{
						crFolder = _temp;
						break;
					}
				}
			}
			parentId = crFolder->id;
		}else{
			parentId = disk->id;	
		}
		
		//dbNode data;
		//dm->dbn->initGivenData(data);
		data.catId = catId;
		data.parent = parentId;

		data.nodeAttr = 24644;
		data.nodeFlags = 0;
		data.diskNo = diskNo;
		if (!dm->dbn->createNode(data) == -1)
			printQS("Fail on " + data.nodeName);
        if (pw)
            pw->stepProgressBar(progressBarNo);

	
		
	}
	return true;
    

    /*
    dm->dbn->initGivenData(data);
    data.catId = catId;
    data.nodeType = item->itemType;
    data.nodeName = item->itemName;
    data.dateTime = item->itemDateTime;
    data.iconId = 0;
    data.nodeDesc = item->itemDescription;
    data.nodeAttr = item->itemAttr;
    data.nodeFlags = item->itemFlags;
    // No attributes exported! :(
    
    switch (data.nodeType)
    {
        case GROUP_ELEMENT:
               data.parent = 0;
               dm->dbn->createGroup(data);
            break;
        case DISK_ELEMENT:
	            data.parent = 0;
				dm->dbn->createDisk(data);
				data.id = dm->dbn->isExistDisk(data.catId, data.parent, data.nodeName);
            break;
        case FOLDER_ELEMENT:
            data.parent = item->parentId;   
            break;
        case FILE_ELEMENT:
            data.parent = item->parentId;   
            data.nodeSize = item->itemSize;
            break;
    }
	*/
    return true;
}

void Work_ImportWhereIsItItem::saveDisks()
{
	foreach (ImportTreeItem *item, disksMap)
	{
		
		
		dbNode data;
		dm->dbn->initGivenData(data);
		data.catId = catId;
		data.nodeName = item->itemName;
		data.dateTime = item->itemDateTime;
		data.nodeDesc = item->itemDescription;
		data.nodeType = item->itemType;
		data.nodeAttr = item->itemAttr;
		data.nodeFlags = item->itemFlags;
		dm->dbn->createDisk(data);
		data.id = dm->dbn->isExistDisk(data.catId, data.parent, data.nodeName);
		item->id = data.id;
		item->dbDiskNo = data.diskNo;
        if (pw)
            pw->stepProgressBar(progressBarNo);
		//printQS(QString("Disk %1 has id = %2").arg(data.nodeName).arg(data.id));
	}
}

void Work_ImportWhereIsItItem::saveAllFolders()
{
	foreach (ImportTreeItem *item, disksMap)
	{
		saveFolders(item->dbDiskNo, item);
	}
	
}

void Work_ImportWhereIsItItem::saveFolders(int diskNo, ImportTreeItem * root)
{
	for (int i = 0; i < root->childCount(); i++)
	{
		ImportTreeItem *item = static_cast <ImportTreeItem *>(root->child(i));
		dbNode data;
		dm->dbn->initGivenData(data);
		data.catId = catId;
		data.parent = root->id;
		data.nodeName = item->itemName;
		data.dateTime = item->itemDateTime;
		data.nodeDesc = item->itemDescription;
		data.nodeType = item->itemType;
		data.nodeAttr = item->itemAttr;
		data.nodeFlags = item->itemFlags;
		data.diskNo = diskNo;
		dm->dbn->createNode(data);
        if (pw)
            pw->stepProgressBar(progressBarNo);
		data.id = dm->dbn->getNodeId(data.catId, data.parent, data.nodeName);
		item->id = data.id;
		item->dbDiskNo = data.diskNo;
		//printQS(QString("Folder %1 has id = %2").arg(data.nodeName).arg(data.id));
		saveFolders(diskNo, item);

	}
	
}

void Work_ImportWhereIsItItem::setProgressObject(ProgressWidget * _pw, int progressNo)
{
    pw = _pw;
    progressBarNo = progressNo;
}



