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
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QFileInfo>
#include <QDateTime>
#include <QVariant>
#include <QDir>

#include "defsandtools.h"
#include "dbnode.h"

#include "mediascanner.h"

/**
 * MediaScanner class start
 * 
 *  
 * */
MediaScanner::MediaScanner(dbNodeClass *_dbn, int catID, int parent, QString pathToScan)
{
    
	dbn = _dbn;
	startPath = pathToScan;
	catNo = catID;
	this->parent = parent;
	justCountFiles = false;
//	cntAll = 0;
	cntItems = 0;
	dirProgress = NULL;
	filesProgress = NULL;
}

MediaScanner::~MediaScanner()
{
	
}

void MediaScanner::setCatalogAndParent(int catId, int parent)
{
	catNo = catId;
	this->parent = parent;
}

void MediaScanner::startScan(int _currentDiskNo)
{
	cntItems = 0;
	currentDiskNo = _currentDiskNo;
	scan(startPath, parent);
}

void MediaScanner::scan(QString path, int parent)
{
	QDir currDir(path);
    //currDir.setFilter(QDir::AllDirs | QDir::Files | QDir::Hidden | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	currDir.setFilter(QDir::AllDirs | QDir::Files | QDir::NoSymLinks | QDir::NoDotAndDotDot);
	currDir.setSorting(QDir::Name);
	QFileInfoList allFiles = currDir.entryInfoList();
    
	for (int i = 0; i < allFiles.count(); i++){
		QFileInfo currItem = allFiles.at(i);
		cntItems++;
		if (!justCountFiles){
			putNodeInDb(currItem, parent);
		}
		if (dirProgress)
			dirProgress(cntItems);
		if (currItem.isDir())
		{
			QString newDir = path +"/" +  currItem.fileName();
			int newParent = 0;
			if (!justCountFiles)
				newParent = dbn->getNodeId(catNo, parent, currItem.fileName());
			scan(newDir, newParent);
		}
	}

}

void MediaScanner::putNodeInDb(const QFileInfo &fi, int parent)
{

    int filePerm = 0;
	int itemType = (fi.isDir()) ? FOLDER_ELEMENT : FILE_ELEMENT;
    filePerm = fi.permissions();
    filePerm = (filePerm & 0x0000F0FF);
    if (fi.isDir())
        filePerm = (filePerm | 0x0400);
    if (fi.isSymLink())
        filePerm = (filePerm | 0x0200);
    if (fi.isHidden())
        filePerm = (filePerm | 0x0100);
    
	dbNode nodeData;
    dbn->initGivenData(nodeData);
	nodeData.catId = catNo;
	nodeData.parent = parent;
	nodeData.nodeName = fi.fileName();
	//nodeData.nodeExt = fi.suffix();
	nodeData.nodeSize = fi.size();
	nodeData.dateTime = fi.lastModified().toTime_t();
	nodeData.nodeType = itemType;
	nodeData.iconId = 0;
	nodeData.nodeDesc = "";
	nodeData.nodeAttr = filePerm;
	nodeData.nodeOwner = fi.owner();
	nodeData.nodeGrp = fi.group();
	nodeData.nodeFlags = 0;
	nodeData.nodeMimeType = "";
	nodeData.diskNo = currentDiskNo;
    
	dbn->createNode(nodeData);
    
    
}
/* MediaScanner end */



