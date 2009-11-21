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
#include <QStringList>
#include <QTreeWidget>
#include <QSqlQuery>
#include <QIcon>
#include <QPixmap>
#include <QDateTime>
#include <QMessageBox>
#include <QHeaderView>


#include "searchdlg.h"
#include "defsandtools.h"
#include "iconmanager.h"
#include "configmanager.h"
#include "datamodule.h"

SearchDlg::SearchDlg(QCatDataModuleClass *_dm, int _catId, QWidget *parent)
 : dm(_dm), QDialog(parent), Ui::SearchDialog()
{
    //setupUi();
	catId = _catId;
    returnedId = -1;
    //dbConnName = connName;
    //db = QSqlDatabase::database(dbConnName, true);
    searchInDirs = dm->settings->searchDirNames;
    searchInFiles = dm->settings->searchFileNames;
    searchInDescriptions = dm->settings->searchDescriptions;
    caseSensitiveSearch = dm->settings->searchCaseSensitive;
	diskNames.clear();
  
	setupUi(this);
    
    QStringList resultHeader;
    resultHeader << tr("Name") << tr("Ext") << tr("Disk name") << tr("Size") << tr("Date") << tr("Time") << tr("Attr");
    resultsTreeWidget->setHeaderLabels(resultHeader);
	resultsTreeWidget->header()->setResizeMode(QHeaderView::ResizeToContents);
    
    if (searchInFiles)
        sfCheckBox->setCheckState(Qt::Checked);
    else
        sfCheckBox->setCheckState(Qt::Unchecked);
    if (searchInDirs)
        sdiCheckBox->setCheckState(Qt::Checked);
    else
        sdiCheckBox->setCheckState(Qt::Unchecked);
    if (searchInDescriptions)
        sdeCheckBox->setCheckState(Qt::Checked);
    else
        sdeCheckBox->setCheckState(Qt::Unchecked);
    if (caseSensitiveSearch)
        csCheckBox->setCheckState(Qt::Checked);
    else
        csCheckBox->setCheckState(Qt::Unchecked);
    
    
    connect(searchPushButton, SIGNAL(clicked()), this, SLOT(searchClicked()));
    connect(settingsPushButton, SIGNAL(clicked()), this, SLOT(settingsClicked()));
    connect(csCheckBox, SIGNAL(stateChanged(int)), this, SLOT(caseSensitiveStateChanged(int)));
    connect(sfCheckBox, SIGNAL(stateChanged(int)), this, SLOT(searchInFilesStateChanged(int)));
    connect(sdiCheckBox, SIGNAL(stateChanged(int)), this, SLOT(searchInDirsStateChanged(int)));
    connect(sdeCheckBox, SIGNAL(stateChanged(int)), this, SLOT(searchInDescStateChanged(int)));
    connect(resultsTreeWidget, SIGNAL(itemActivated(QTreeWidgetItem*, int)), this, SLOT(itemIsActivated(QTreeWidgetItem*, int)));
    
    
}


SearchDlg::~SearchDlg()
{
    printQS("Search destroyed");
}

void SearchDlg::resizeEvent(QResizeEvent *event)
{
//	printQS("Resized");
//	resultsTreeWidget->
}

QString SearchDlg::getDiskName(int diskId)
{
	QString sql = "SELECT name FROM Catalog_" + QString::number(catId) + " WHERE diskNo = " + QString::number(diskId) + " AND itemType = " + QString::number(DISK_ELEMENT);
	//printQS(sql);
	QSqlQuery *q = new QSqlQuery(dm->db);
	QString result;
	if (!q)
		return result;
	if (q->exec(sql))
	{
		q->first();
		result = q->value(0).toString();
	}
	delete q;
	return result;
}

void SearchDlg::searchClicked()
{
    if (searchInDirs || searchInFiles || searchInDescriptions)
    {
        QString searchCriteria = searchLineEdit->text();
        if (searchCriteria.isEmpty())
        {
            QMessageBox::critical(this, tr("Missing Criteria"), tr("Missing Search Criteria.\nPlease fill some pattern to search for. "));   
            return;
  
        }
        dm->settings->searchDirNames = searchInDirs;
        dm->settings->searchFileNames = searchInFiles;
        dm->settings->searchDescriptions = searchInDescriptions;
        dm->settings->searchCaseSensitive = caseSensitiveSearch;

        QString sqlC = "SELECT id, parent, itemType, name, size, dateTime, iconId, attributes, diskNo FROM Catalog_" + QString::number(catId) + " WHERE ";
        QString sqlF = "SELECT id, parent, itemType, name, size, dateTime, iconId, attributes, diskNo FROM Files_" + QString::number(catId) + " WHERE ";
        
        QString sqlName(NULL);
        QString sqlDesc(NULL);
        if (searchInDescriptions)
        {
         	sqlDesc = " (description LIKE '%" + searchCriteria + "%') ";   
        }
        
        if (searchInFiles)
        {
            sqlName = " (name LIKE '%" + searchCriteria + "%') ";
            if (searchInDescriptions)
                sqlName += "OR " + sqlDesc;
        }
        if (searchInDirs)
        {
            sqlName = " (name LIKE '%" + searchCriteria + "%') ";
            if (searchInDescriptions)
                sqlName += "OR " + sqlDesc;
            
        }
        /*
        if (searchInDirs && searchInFiles)
        {
            sqlName = " (name LIKE '%" + searchCriteria + "%') ";
            if (searchInDescriptions)
                sqlName += "OR " + sqlDesc;
        }else{
            if (searchInFiles)
            {
                sqlName = " (itemType = " + QString::number(FILE_ELEMENT) + ") AND ((name LIKE '%" + searchCriteria + "%') ";
                if (searchInDescriptions)
                {
                    sqlName += "OR " + sqlDesc;   
                }
                sqlName += ")";
            }
            if (searchInDirs)
            {
                sqlName = " (itemType = " + QString::number(FOLDER_ELEMENT) + ") AND ((name LIKE '%" + searchCriteria + "%') ";
                if (searchInDescriptions)
                {
                    sqlName += "OR " + sqlDesc;   
                }
                sqlName += ")";
            }
        }
        */
        if (searchInFiles || searchInDirs)
        {
        	sqlC += sqlName;
            sqlF += sqlName;
        }else{
         	sqlC += sqlDesc;
            sqlF += sqlDesc;   
        }
    
        QSqlQuery *q = new QSqlQuery(dm->db);
        if (!q)
            return;
        //printQS(sqlC);
        //printQS(sqlF);
    // Delete previous items
        QTreeWidgetItem *qtwi = 0;
        while (qtwi = resultsTreeWidget->takeTopLevelItem(0))
        {
            delete qtwi;   
        }
    // Run current search
        /*
        if (q->exec(sqlC))
        {
            fillRes(q);
/*            
            QDateTime dt;
            while (q->next())
            {
            
				qlonglong _size = 0;
				int diskNo = q->value(8).toInt();
				if (!diskNames.contains(diskNo))
					diskNames[diskNo] = getDiskName(diskNo);
                CatTreeItem *item = new CatTreeItem;
//            item->setText(0, q->value(1).toString());
                item->iconId = q->value(6).toInt();
                item->itemType = q->value(2).toInt();
                item->id = q->value(0).toInt();
                item->parentId = q->value(1).toInt();
                dt.setTime_t(q->value(5).toInt());
                vFileName data;
                if (item->itemType == FILE_ELEMENT)
                {
                    parseFileName(q->value(3).toString(), data);
                    item->setText(0, data.fileName);
                }else{
                    item->setText(0, q->value(3).toString());
                }
                if (data.hasExt)
                    item->setText(1, data.fileExt);
				_size = q->value(4).toLongLong();
				item->setText(2, diskNames[diskNo]);
				item->setText(3, QString::number(_size));
                item->setText(4, dt.date().toString(Qt::LocalDate));
                item->setText(5, dt.time().toString(Qt::LocalDate));
                item->setText(6, permsToQString(q->value(7).toInt()));
                QPixmap *pix = 0;
                if (item->iconId)
                {
                    pix = dm->im->getPixmap(1, item->iconId);
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
            
            
            
                resultsTreeWidget->addTopLevelItem(item);
            
            }
            
        }
        */
        if (searchInDirs)
        {
            if (q->exec(sqlC))
            {
                fillRes(q);
            }
        }
        if (searchInFiles)
        {
            if (q->exec(sqlF))
            {
                fillRes(q);
            }
        }
        delete q;
        //printQS("Search!");
        
    }else{
        QMessageBox::critical(this, tr("Missing Criteria"), tr("Missing Search Criteria.\nPlease check at least one in the search options. "));   
    }
    
}

void SearchDlg::settingsClicked()
{
    printQS("Settings!");
}

void SearchDlg::caseSensitiveStateChanged(int state)
{
    if (csCheckBox->checkState() == Qt::Checked)
        caseSensitiveSearch = true;
    else
        caseSensitiveSearch = false;
}

void SearchDlg::searchInFilesStateChanged(int state)
{
    if (sfCheckBox->checkState() == Qt::Checked)
        searchInFiles = true;
    else
        searchInFiles = false;
    
}

void SearchDlg::searchInDirsStateChanged(int state)
{
    if (sdiCheckBox->checkState() == Qt::Checked)
        searchInDirs = true;
    else
        searchInDirs = false;
    
}

void SearchDlg::searchInDescStateChanged(int state)
{
    if (sdeCheckBox->checkState() == Qt::Checked)
        searchInDescriptions = true;
    else
        searchInDescriptions = false;
}

void SearchDlg::itemIsActivated(QTreeWidgetItem *_item, int column)
{
    CatTreeItem *item = static_cast<CatTreeItem *>(_item);
    if (item->itemType == FILE_ELEMENT)
        returnedId = item->parentId;
    else
        returnedId = item->id;
    printQS(QString::number(returnedId));
    done(QDialog::Accepted);
}

void SearchDlg::fillRes(QSqlQuery * q)
{
    QDateTime dt;
    while (q->next())
    {
            
        qlonglong _size = 0;
        int diskNo = q->value(8).toInt();
        if (!diskNames.contains(diskNo))
            diskNames[diskNo] = getDiskName(diskNo);
        CatTreeItem *item = new CatTreeItem;
//            item->setText(0, q->value(1).toString());
        item->iconId = q->value(6).toInt();
        item->itemType = q->value(2).toInt();
        item->id = q->value(0).toInt();
        item->parentId = q->value(1).toInt();
        dt.setTime_t(q->value(5).toInt());
        vFileName data;
        if (item->itemType == FILE_ELEMENT)
        {
            parseFileName(q->value(3).toString(), data);
            item->setText(0, data.fileName);
        }else{
            item->setText(0, q->value(3).toString());
        }
        if (data.hasExt)
            item->setText(1, data.fileExt);
        _size = q->value(4).toLongLong();
        item->setText(2, diskNames[diskNo]);
        item->setText(3, QString::number(_size));
        item->setText(4, dt.date().toString(Qt::LocalDate));
        item->setText(5, dt.time().toString(Qt::LocalDate));
        item->setText(6, permsToQString(q->value(7).toInt()));
        QPixmap *pix = 0;
        if (item->iconId)
        {
            pix = dm->im->getPixmap(1, item->iconId);
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
            
            
            
        resultsTreeWidget->addTopLevelItem(item);
            
    }

}

