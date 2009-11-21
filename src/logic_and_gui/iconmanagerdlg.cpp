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

#include <QSqlQuery>
#include <QListWidgetItem>
#include <QPixmap>
#include <QMessageBox>
#include <QFileDialog>

#include "defsandtools.h"
#include "datamodule.h"
#include "iconmanagerdlg.h"
#include "iconmanager.h"
#include "addupdateicondlg.h"

IconManagerDlg::IconManagerDlg(QCatDataModuleClass *_dm, QWidget *parent)
	: dm(_dm), QDialog(parent), Ui::IconManageDialog()
{
	setupUi(this);
    useDefaultIcon = false;
    iconsListWidget->setIconSize(QSize(32, 32));
    connect(addToolButton, SIGNAL(clicked()), this, SLOT(addClicked()));
    connect(updateToolButton, SIGNAL(clicked()), this, SLOT(updateClicked()));
    connect(removeToolButton, SIGNAL(clicked()), this, SLOT(removeClicked()));
    connect(saveToolButton, SIGNAL(clicked()), this, SLOT(saveClicked()));
    connect(defaultToolButton, SIGNAL(clicked()), this, SLOT(defaultClicked()));
    connect(this, SIGNAL(accepted()), this, SLOT(acceptedFired()));
    connect(iconsListWidget, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(itemChanged(QListWidgetItem *, QListWidgetItem *)));
}


IconManagerDlg::~IconManagerDlg()
{
}

void IconManagerDlg::customInit(int _catId, int _iconId, int _itemType, int _itemAttr)
{
    catId = _catId;
    itemType = _itemType;
    itemAttr = _itemAttr;
    currIcon = _iconId;
    QString sqlCatalogs = "SELECT id, name FROM Catalogs WHERE id = " + QString::number(catId);
    QSqlQuery *q = new QSqlQuery(dm->db);
	if (!q)
		return;
    catalogLineEdit->setEnabled(false);
    
    if (catId)
    {
     	// Regular catalog   
        if (q->exec(sqlCatalogs))
        {
            while (q->next())
            {
                catalogLineEdit->setText(q->value(1).toString());
            }
        }
    }else{
        // Common Icons
        catalogLineEdit->setText(tr("Common icons"));
    }
	reloadIcons();
    if (!catId)
    {
        removeToolButton->setEnabled(false);   
        defaultToolButton->setEnabled(false);   
        addToolButton->setEnabled(false);
    }
}

void IconManagerDlg::addClicked()
{
    AddUpdateIconDlg *dlg1 = new AddUpdateIconDlg(this);
    QString in1 = QString("icon_%1").arg(maxIdNum, 6, 10, QChar('0'));
    QPixmap pix;
    if (catId)
    	dlg1->customInit(false, false, in1, pix);
	else
        dlg1->customInit(false, true, in1, pix);
        
    if (dlg1->exec() == QDialog::Accepted)
    {
        printQS("New Icon added!"); 
        QString iconName = dlg1->iconName;
        QString fileName = dlg1->fileName;
        dm->im->addIconFromFile(catId, iconName, fileName);
        reloadIcons();
        
    }else{
        printQS("Canceled!");
    }
    delete dlg1;
}

void IconManagerDlg::updateClicked()
{
    IconListItem *item = 0;
	item = static_cast<IconListItem *>(iconsListWidget->currentItem());
	if (item)
	{
		QPixmap pix;
		pix = item->icon().pixmap(48, 48);
		AddUpdateIconDlg *dlg1 = new AddUpdateIconDlg(this);
		QString in1 = item->text();
        if (catId)
            dlg1->customInit(true, false, in1, pix);
        else
        	dlg1->customInit(true, true, in1, pix);
        if (dlg1->exec() == QDialog::Accepted)
		{
			printQS("Icon updated!");      
            QString fileName = dlg1->fileName;
            //IconManager im(dbConnName);
            if (fileName.isEmpty())
                dm->im->updateIconName(catId, item->iconId, dlg1->iconName);
            else
                dm->im->updateIconFromFile(catId, item->iconId, dlg1->iconName, fileName);
            reloadIcons();
        }else{
			printQS("Canceled!");
		}
		delete dlg1;
	}
}

void IconManagerDlg::removeClicked()
{
	IconListItem *item = 0;
	item = static_cast<IconListItem *>(iconsListWidget->currentItem());
	if (item)
	{
		if (QMessageBox::warning(this, tr("Delete icon!"), tr("Do you REALLY want to delete this icon?\nThis can not be undone!"), QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::Yes)
		{
            dm->im->deleteIcon(catId, item->iconId);
		}
		reloadIcons();
	}
}

void IconManagerDlg::defaultClicked()
{
    IconListItem *item = 0;
    item = static_cast<IconListItem *>(iconsListWidget->currentItem());
    QPixmap *pix = 0;
    pix = dm->im->getDefaultPixmap(itemType, itemAttr);
    previewLabel->setPixmap(*pix);
    delete pix;
	useDefaultIcon = true;
    currIcon = 0;
}

void IconManagerDlg::saveClicked()
{
    IconListItem *item = 0;
    item = static_cast<IconListItem *>(iconsListWidget->currentItem());
    if (item)
    {
    
        QString fileName = QFileDialog::getSaveFileName(this, tr("Save icon"));
        if (!fileName.isEmpty())
        {
            dm->im->saveIcon(catId, item->iconId, fileName);
        }
    }
}

void IconManagerDlg::reloadIcons()
{
    QString sqlIcons = "SELECT id, name, fileName FROM icons WHERE catId = " + QString::number(catId);
    maxIdNum = 0;
    IconListItem *item = 0;
    while (item = static_cast <IconListItem *>(iconsListWidget->takeItem(0)))
    {
     	delete item;    
    }
    QSqlQuery *q = new QSqlQuery(dm->db);
    if (!q)
        return;
    if (q->exec(sqlIcons))
    {
        while (q->next())
        {
            IconListItem *item = new IconListItem(iconsListWidget);
            QPixmap *pix = 0;
            pix = dm->im->getPixmap(catId, q->value(0).toInt());
            item->setIcon(QIcon(*pix));
            delete pix;
            item->setText(q->value(1).toString());
            item->iconId = q->value(0).toInt();
            item->iconFileName = q->value(2).toString();
            if (maxIdNum < item->iconId)
                maxIdNum = item->iconId;
        }
    }
    delete q;
}

void IconManagerDlg::acceptedFired()
{
/*	currIcon = 0;
	IconListItem *item = 0;
	item = static_cast<IconListItem *>(iconsListWidget->currentItem());
	if (item)
	{
		currIcon = item->iconId; 
	}else{
     	currIcon = 0;   
    }*/
}

int IconManagerDlg::getCurrentIcon()
{
	return currIcon;
}

void IconManagerDlg::itemChanged(QListWidgetItem *currentItem, QListWidgetItem *prevItem)
{
    if (currentItem)
    {
        IconListItem *item = static_cast <IconListItem *>(currentItem);
        previewLabel->setPixmap(item->icon().pixmap(48, 48));
        useDefaultIcon = false;;
        currIcon = item->iconId;
    }
}