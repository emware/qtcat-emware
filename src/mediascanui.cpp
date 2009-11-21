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

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QPushButton>
#include <QLineEdit>
#include <QComboBox>
#include <QSplitter>
#include <QFileDialog>
#include <QProgressDialog>
#include <QTextEdit>
#include <QSpacerItem>
#include <QToolButton>
#include <QCheckBox>
#include <QMessageBox>


#include "mediascanui.h"
#include "mediascanner.h"
#include "defsandtools.h"
#include "dbnode.h"
#include "configmanager.h"
#include "addscanplaces.h"
#include "datamodule.h"
#include "deviceclass.h"

QProgressDialog *progressGlobalDlg;
void makeProgressGlobal(int i)
{
	if (progressGlobalDlg)
		progressGlobalDlg->setValue(i);
}


MediaScanUI::MediaScanUI(QCatDataModuleClass *_dm, QWidget *parent)
	: dm(_dm), QDialog(parent)
{
	initGUI();
	device = new DeviceClass;
}

MediaScanUI::~MediaScanUI()
{
	delete device;
}

void MediaScanUI::initGUI()
{
	catId = 0;
	parentId = 0;
	loadingItems = true;
	mountByMe = false;
	
	vbLayout1 = new QVBoxLayout;
	vbLayout2 = new QVBoxLayout;
	vbLayout3 = new QVBoxLayout;
	vbLayout4 = new QVBoxLayout;
	
	hbLayout1 = new QHBoxLayout;
	hbLayout2 = new QHBoxLayout;
	//hbLayout3 = new QHBoxLayout;
	hbLayout4 = new QHBoxLayout;
	hbLayout5 = new QHBoxLayout;
	//hbLayout6 = new QHBoxLayout;
	hbLayout7 = new QHBoxLayout;
	
	grLayout1 = new QGridLayout;
	
	favsLabel = new QLabel(tr("Choose place to scan:"));
	pathsTree = new QTreeWidget;
    pathsTree->setRootIsDecorated(false);
    pathsTree->setHeaderLabel(tr("Favourites"));
	
	pathLabel = new QLabel;
	pathLabel->setText(tr("Scan path:"));
	pathLineEdit = new QLineEdit;
	
	vbLayout1->addWidget(favsLabel);
	vbLayout1->addWidget(pathsTree);
	vbLayout1->addWidget(pathLabel);
	vbLayout1->addWidget(pathLineEdit);
	
	parentNameLabel = new QLabel(tr("Save in"));
	parentNameLineEdit = new QLineEdit;
	parentNameLineEdit->setText(parentName);
	hbLayout7->addWidget(parentNameLabel);
	hbLayout7->addWidget(parentNameLineEdit);
	
	mediaNameLabel = new QLabel(tr("Media Name"));
	mediaNameLineEdit = new QLineEdit;
	vbLayout2->addWidget(mediaNameLabel);
	vbLayout2->addWidget(mediaNameLineEdit);
			
	serialLabel = new QLabel(tr("Disk Serial"));
	serialLineEdit = new QLineEdit;
	hbLayout1->addWidget(serialLabel);
	hbLayout1->addWidget(serialLineEdit);
	
	mediaTypeLabel = new QLabel(tr("Media type"));
	mediaTypeCombo = new QComboBox;
	
	// Fill Media Types in ComboBox - defined in defsandtools.h
	for (int i = 0; i < MEDIA_TYPES_COUNT; i++)
	{
		mediaTypeCombo->addItem(QIcon(mediaTypesArray[i].iconFile), tr(mediaTypesArray[i].mediaName));
	}

	hbLayout2->addWidget(mediaTypeLabel);
	hbLayout2->addWidget(mediaTypeCombo);
	
	commentLabel = 	new QLabel(tr("Comment:"));
	commentTextEdit = new QTextEdit;
	spacer1 = new QSpacerItem(20, 60, QSizePolicy::Minimum, QSizePolicy::Expanding);
	grLayout1->addWidget(commentLabel, 0, 0, Qt::AlignTop);
	grLayout1->addWidget(commentTextEdit, 0, 1, 1, 1);
	grLayout1->addItem(spacer1, 1, 0);
	
	vbLayout3->addLayout(hbLayout7);
	vbLayout3->addLayout(vbLayout2);
	vbLayout3->addLayout(hbLayout1);
	vbLayout3->addLayout(hbLayout2);
	vbLayout3->addLayout(grLayout1);
	
	hbLayout4->addLayout(vbLayout1);
	hbLayout4->addLayout(vbLayout3);
	
	spacer2 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	
	manageLocations = new QToolButton;
	manageLocations->setText(tr("Manage Paths"));
	manageLocations->setToolTip(tr("Manage paths."));
	doNotCloseCheckBox = new QCheckBox(tr("Do not close dialog"));
    if (dm->settings->dontCloseAfterScan)
        doNotCloseCheckBox->setCheckState(Qt::Checked);

	okScanButton = new QPushButton(tr("Ok/Scan"));
	cancelButton = new QPushButton(tr("Cancel"));
	hbLayout5->addWidget(manageLocations);
	hbLayout5->addWidget(doNotCloseCheckBox);
	hbLayout5->addItem(spacer2);
	hbLayout5->addWidget(okScanButton);
	hbLayout5->addWidget(cancelButton);
	
	
	vbLayout4->addLayout(hbLayout4);
	vbLayout4->addLayout(hbLayout5);
	this->setLayout(vbLayout4);
	
	resize(500, 300);
	
	//int nextDisk = dn->dbn->getNextDiskNo(
	
	
//	connect(selectDirButton, SIGNAL(clicked()), this, SLOT(selectDir()));
	connect(okScanButton, SIGNAL(clicked()), this, SLOT(startScan()));
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
    connect(manageLocations, SIGNAL(clicked()), this, SLOT(managePaths()));
	connect(pathsTree, SIGNAL(currentItemChanged (QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(itemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));

}

void MediaScanUI::itemChanged(QTreeWidgetItem *newItem, QTreeWidgetItem *prevItem)
{
	if (!loadingItems)
	{
		if (newItem->text(1).toInt() == -1)
		{
			QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
			pathLineEdit->setText(dir);
			setWindowTitle(tr("Scanning in ") + dir);
		}else{
			setWindowTitle(tr("Scanning in ") + newItem->toolTip(0));
			pathLineEdit->setText(newItem->toolTip(0));
		}
		QString dev = newItem->toolTip(0);
		if (dev.startsWith("/dev"))
		{
			device->getInfoForOptical(dev);
			if (device->udf)
			{
				mediaNameLineEdit->setText(device->vdUDF);
			}else{
				if (device->Joliet)
					mediaNameLineEdit->setText(device->vdJoliet);
				else
					mediaNameLineEdit->setText(device->vdIso9660);
					
			}
			pathLineEdit->setText(device->getOptiocalMountMount(dev));
		}
	}
}


void MediaScanUI::managePaths()
{
	AddScanPlaces *asp = new AddScanPlaces(this);
    asp->setConfigManager(dm->settings);
	asp->loadItems();
	asp->exec();
    loadItems();
	delete asp;
}

void MediaScanUI::setCatId(int _catId)
{
	catId = _catId;	
	serialLineEdit->setText(QString::number(dm->dbn->getNextDiskNo(catId)));
}

void MediaScanUI::setParentId(int _parentId)
{
	parentId = _parentId;
}

void MediaScanUI::setParentName(const QString &_parentName)
{
	parentName = _parentName;
	parentNameLineEdit->setText(parentName);
	
}

void MediaScanUI::startScan()
{
	
	QString diskName = mediaNameLineEdit->text();
	QString startDir = pathLineEdit->text();
    if (doNotCloseCheckBox->checkState() == Qt::Checked)
        dm->settings->dontCloseAfterScan = true;
    else
        dm->settings->dontCloseAfterScan = false;
    
    if (diskName.isEmpty())
	{
		// Empty disk name
        QMessageBox::critical(this, tr("Required Field!"), tr("Disk name is empty!\nPlease fill it and try again."), QMessageBox::Ok);
		return;
	}
	bool mount = false;
	if (pathsTree->currentItem()->text(2).toInt() != -1)
	{
		int index = pathsTree->currentItem()->text(2).toInt();
		mount = dm->settings->scanPlaces.at(index).useMount;
	}
	if (mount && (!device->checkIfMounted(startDir)))
	{
		mountByMe = device->mountDev(startDir);
	}
	
	//MediaScanner *ms = new MediaScanner(dbn, data.catId, data.id, startDir);
    MediaScanner *ms = new MediaScanner(dm->dbn, 0, 0, startDir);
	ms->setDirProgressFunc(NULL);
	ms->setJustCount(true);
	//ms->startScan(data.diskNo);
	ms->startScan(0);
	int allFiles = ms->getItemsNum();
	
	progressDlg = new QProgressDialog(tr("Scanning files..."), tr("Cancel"), 0, allFiles);
	progressDlg->setWindowModality(Qt::WindowModal);
	progressGlobalDlg = progressDlg;

	// Create new disk
	dbNode data;
    dm->dbn->initGivenData(data);
	data.nodeName = diskName;
	data.catId = catId;
	data.parent = parentId;
	int currI = mediaTypeCombo->currentIndex();
	data.nodeAttr = getFullDiskTypeFromIndex(currI);
	data.nodeFlags = setBasicDiskType(getBasicDiskTypeFromIndex(currI), data.nodeFlags);
	
	
    dm->dbn->createDisk(data);
    data.id = dm->dbn->isExistDisk(data.catId, data.parent, data.nodeName);
	ms->setCatalogAndParent(data.catId, data.id);	
	ms->setDirProgressFunc(makeProgressGlobal);
	ms->setJustCount(false);
	ms->startScan(data.diskNo);

	delete progressDlg;
	delete ms;
	
	if (mountByMe)
		device->unMountDev(startDir);
	if (doNotCloseCheckBox->checkState() == Qt::Unchecked)
		accept();

	
	
}

void MediaScanUI::loadItems()
{
	loadingItems = true;
    while (pathsTree->topLevelItemCount())
    {
        delete pathsTree->takeTopLevelItem(0);
    }
    if (dm->settings->scanPlaces.size() > 0)
    {
        for (int i = 0; i < dm->settings->scanPlaces.size(); ++i)
        {
            QTreeWidgetItem *item = new QTreeWidgetItem;   
            item->setText(0, dm->settings->scanPlaces.at(i).name);
            item->setToolTip(0, dm->settings->scanPlaces.at(i).path);
            item->setIcon(0, QIcon(getMediaTypeIconName(dm->settings->scanPlaces.at(i).itemType)));
			item->setText(1, QString::number(dm->settings->scanPlaces.at(i).itemType));
			//item->setText(1, dm->settings->scanPlaces.at(i).itemType);
			item->setText(2, QString::number(i));
            pathsTree->addTopLevelItem(item);    
        }
        
    }

	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setText(0, tr("Browse..."));
    item->setIcon(0, QIcon(":icons/interface/add.png"));
	item->setText(1, "-1");
	item->setText(2, "-1");
	pathsTree->addTopLevelItem(item);    
	loadingItems = false;
}
