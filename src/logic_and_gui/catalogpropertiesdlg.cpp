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
#include <QPixmap>
#include <QDialog>
#include <QCheckBox>
#include <QComboBox>
#include <QDateTime>
#include <QDate>
#include <QTime>

#include "defsandtools.h"
#include "catalogpropertiesdlg.h"
#include "catalogreports.h"
#include "catalogpassworddlg.h"
#include "iconmanagerdlg.h"
#include "iconmanager.h"
#include "dbnode.h"
#include "catalogclass.h"
#include "datamodule.h"

CatalogPropertiesDlg::CatalogPropertiesDlg(QCatDataModuleClass *_dm, QWidget *parent)
 : dm(_dm), QDialog(parent), Ui::CatalogPropertiesDialog()
{
	setupUi(this);
	descChanged = false;
	iconChanged = false;
	autoOpenChanged = false;
	diskTypeChanged = false;
	nameChanged = false;
	thisChanged = false;
    defaultIconUsed = true;
    itemAttr = 0;
    itemFlags = 0;
    itemType = 0;
    itemId = 0;
    catId = 0;
    iconId = 0;
	
	toolButtonLock->setIconSize(QSize(32, 32));
	toolButtonLock->setIcon(QIcon(":icons/interface/lock.png"));
	
	connect(toolButtonLock, SIGNAL(clicked()), this, SLOT(passwordProtect()));
	connect(toolButtonIcon, SIGNAL(clicked()), this, SLOT(iconManage()));
	connect(textEditDescription, SIGNAL(textChanged()), this, SLOT(descriptionIsChanged()));
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(okPressed()));
	connect(nameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(nameIsChanged(const QString& )));
}


CatalogPropertiesDlg::~CatalogPropertiesDlg()
{
//	delete rpt;
}

void CatalogPropertiesDlg::customInit(int _itemType, int _catId, int _itemId, int _iconId)
{
	//dbConnName = connName;
	itemType = _itemType;
	catId = _catId;
	itemId = _itemId;
	//rpt = new CatalogReports(dm-);
    QString iconName;
	switch (itemType){
		case ROOT_ELEMENT: 			// This is the same if Catalog itself is choosen
			dm->rpt->updateCatalog(_catId);
			toolButtonLock->setEnabled(true);
			initIfCatalog();
            iconName = CATALOG_ICON_ENABLED;
			break;
		case GROUP_ELEMENT:
			initIfGroup();
            iconName = DISK_GROUP_ICON;
			toolButtonLock->setEnabled(false);
			break;
		case DISK_ELEMENT:
			initIfDisk();
            iconName = DISK_CD_ICON;
			toolButtonLock->setEnabled(false);
			break;
		case FOLDER_ELEMENT:
			initIfFolder();
			iconName = FOLDER_OPENED_ICON;
			toolButtonLock->setEnabled(false);
			break;
		case FILE_ELEMENT:
			initIfFile();
			iconName = BINARY_FILE_ICON;
			toolButtonLock->setEnabled(false);
			break;
	}
	descChanged = false;
	
    //IconManager im;
    QPixmap *pix = 0;
    if (_iconId)
    {
        pix = dm->im->getPixmap(catId, _iconId);
        defaultIconUsed = false;
    }else{
        pix = dm->im->getDefaultPixmap(itemType, itemAttr);
        defaultIconUsed = true;
    }
    toolButtonIcon->setIconSize(QSize(32, 32));
    toolButtonIcon->setIcon(QIcon(*pix));
    delete pix;
}

void CatalogPropertiesDlg::setFlagsAndAttr(int _flags, int _attr)
{
	itemFlags = _flags;
	itemAttr = _attr;
}

void CatalogPropertiesDlg::initIfCatalog()
{
	hbLayout01 = new QHBoxLayout;
	hbLayout02 = new QHBoxLayout;
	hbLayout03 = new QHBoxLayout;
	hbLayout04 = new QHBoxLayout;
	hbLayout05 = new QHBoxLayout;
	hbLayout06 = new QHBoxLayout;
	hbLayout07 = new QHBoxLayout;
	hbLayout08 = new QHBoxLayout;
	hbLayout09 = new QHBoxLayout;
	hbLayout10 = new QHBoxLayout;
	hbLayout11 = new QHBoxLayout;
	hbLayout12 = new QHBoxLayout;
	
	sp01 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp02 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp03 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp04 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp05 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp06 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp07 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp08 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp09 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp10 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp11 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp12 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	
	label01 = new QLabel(tr("Total Files:"));
	label02 = new QLabel(tr("Total Folders"));
	label03 = new QLabel(tr("Total Size:"));
	label04 = new QLabel(tr("Groups:"));
	label05 = new QLabel(tr("Disks:"));
	label06 = new QLabel(tr("DVD-ROM:"));
	label07 = new QLabel(tr("CD-ROM:"));
	label08 = new QLabel(tr("HDD:"));
	label09 = new QLabel(tr("USB:"));
	label10 = new QLabel(tr("Net Drives:"));
	label11 = new QLabel(tr("Other:"));
	label12 = new QLabel(tr("Created:"));
	
	labelTotalFiles = new QLabel;
	labelTotalDirs = new QLabel;
	labelTotalSize = new QLabel;
	labelTotalGroups = new QLabel;
	labelTotalDisks = new QLabel;
	labelDVDRoms = new QLabel;
	labelCDRoms = new QLabel;
	labelHDDs = new QLabel;
	labelUSBs = new QLabel;
	labelNetDrives = new QLabel;
	labelOther = new QLabel;
	labelDate = new QLabel;
	
	
	hbLayout01->addWidget(label01);
	hbLayout01->addItem(sp01);
	hbLayout01->addWidget(labelTotalFiles);
	
	hbLayout02->addWidget(label02);
	hbLayout02->addItem(sp02);
	hbLayout02->addWidget(labelTotalDirs);
	
	hbLayout03->addWidget(label03);
	hbLayout03->addItem(sp03);
	hbLayout03->addWidget(labelTotalSize);
	
	hbLayout04->addWidget(label04);
	hbLayout04->addItem(sp04);
	hbLayout04->addWidget(labelTotalGroups);

	hbLayout05->addWidget(label05);
	hbLayout05->addItem(sp05);
	hbLayout05->addWidget(labelTotalDisks);
	
	hbLayout06->addWidget(label06);
	hbLayout06->addItem(sp06);
	hbLayout06->addWidget(labelDVDRoms);
	
	hbLayout07->addWidget(label07);
	hbLayout07->addItem(sp07);
	hbLayout07->addWidget(labelCDRoms);
	
	hbLayout08->addWidget(label08);
	hbLayout08->addItem(sp08);
	hbLayout08->addWidget(labelHDDs);
	
	hbLayout09->addWidget(label09);
	hbLayout09->addItem(sp09);
	hbLayout09->addWidget(labelUSBs);
	
	hbLayout10->addWidget(label10);
	hbLayout10->addItem(sp10);
	hbLayout10->addWidget(labelNetDrives);
	
	hbLayout11->addWidget(label11);
	hbLayout11->addItem(sp11);
	hbLayout11->addWidget(labelOther);
	
	hbLayout12->addWidget(label12);
	hbLayout12->addItem(sp12);
	hbLayout12->addWidget(labelDate);

	vbLayout0 = new QVBoxLayout;
	vbLayout1 = new QVBoxLayout;
	vbLayout1->setSpacing(0);
	vbLayout2 = new QVBoxLayout;
	vbLayout2->setSpacing(0);
	
	vbLayout1->addLayout(hbLayout01);
	vbLayout1->addLayout(hbLayout02);
	vbLayout1->addLayout(hbLayout03);
	vbLayout1->addLayout(hbLayout04);
	vbLayout1->addLayout(hbLayout05);
	vbLayout1->addLayout(hbLayout12);
	

	vbLayout2->addLayout(hbLayout06);
	vbLayout2->addLayout(hbLayout07);
	vbLayout2->addLayout(hbLayout08);
	vbLayout2->addLayout(hbLayout09);
	vbLayout2->addLayout(hbLayout10);
	vbLayout2->addLayout(hbLayout11);
	
	QGridLayout *grLay = new QGridLayout;
	QFrame *vline1 = new QFrame;
	vline1->setFrameShape(QFrame::VLine);
	vline1->setFrameShadow(QFrame::Sunken);
	/*
	grLay->addLayout(vbLayout1, 0, 0);
	grLay->addWidget(vline1, 0, 1, 2, 1);
	grLay->addLayout(vbLayout2, 0, 2, 2, 1);
	*/
	grLay->addLayout(vbLayout1, 0, 0);
	grLay->addWidget(vline1, 0, 1);
	grLay->addLayout(vbLayout2, 0, 2);
	
	QFrame *hline1 = new QFrame;
	hline1->setFrameShape(QFrame::HLine);
	hline1->setFrameShadow(QFrame::Sunken);
	
	autoOpenCheckBox = new QCheckBox;
	autoOpenCheckBox->setText(tr("Auto open Catalog"));
	vbLayout0->addLayout(grLay);
	vbLayout0->addWidget(hline1);
	vbLayout0->addWidget(autoOpenCheckBox);
	
	
	infoFrame->setLayout(vbLayout0);
	

	ReportStruct report;
	dm->rpt->getCatalogReport(catId, report);
	if (report.flags & CATALOG_OPENED)
		autoOpenCheckBox->setCheckState(Qt::Checked);
	else
		autoOpenCheckBox->setCheckState(Qt::Unchecked);
		
	nameLineEdit->setText(report.name);
	labelItemType->setText(tr("Catalog"));
	labelTotalFiles->setText(QString::number(report.numFiles));
	labelTotalDirs->setText(QString::number(report.numDirs));
	labelTotalSize->setText(getSizeText(report.size));
	
	labelTotalGroups->setText(QString::number(report.groups));
	labelTotalDisks->setText(QString::number(report.disks));
	
	labelDVDRoms->setText(QString::number(report.numDVDs));
	labelCDRoms->setText(QString::number(report.numCDs));
	labelHDDs->setText(QString::number(report.numHDD));
	labelUSBs->setText(QString::number(report.numFlash));
	labelNetDrives->setText(QString::number(report.numNetDrives));
	labelOther->setText(QString::number(report.numOther));
	labelDate->setText(QDateTime::fromTime_t(report.dateTime).toString(Qt::LocalDate));
	textEditDescription->setText(report.description);
	connect(autoOpenCheckBox, SIGNAL(stateChanged(int)), this, SLOT(autoOpenTrigger(int)));
}

void CatalogPropertiesDlg::initIfGroup()
{
	
	hbLayout01 = new QHBoxLayout;
	hbLayout02 = new QHBoxLayout;
	hbLayout03 = new QHBoxLayout;
	hbLayout04 = new QHBoxLayout;
	hbLayout05 = new QHBoxLayout;
	hbLayout06 = new QHBoxLayout;
	
	sp01 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp02 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp03 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp04 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp05 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp06 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	
	label01 = new QLabel(tr("Total Files:"));
	label02 = new QLabel(tr("Total Folders"));
	label03 = new QLabel(tr("Total Size:"));
	label04 = new QLabel(tr("SubGroups:"));
	label05 = new QLabel(tr("Disks:"));
	label06 = new QLabel(tr("Created:"));
	
	labelTotalFiles = new QLabel;
	labelTotalDirs = new QLabel;
	labelTotalSize = new QLabel;
	labelTotalGroups = new QLabel;
	labelTotalDisks = new QLabel;
	labelDate = new QLabel;
	
	hbLayout01->addWidget(label01);
	hbLayout01->addItem(sp01);
	hbLayout01->addWidget(labelTotalFiles);
	
	hbLayout02->addWidget(label02);
	hbLayout02->addItem(sp02);
	hbLayout02->addWidget(labelTotalDirs);
	
	hbLayout03->addWidget(label03);
	hbLayout03->addItem(sp03);
	hbLayout03->addWidget(labelTotalSize);
	
	hbLayout04->addWidget(label04);
	hbLayout04->addItem(sp04);
	hbLayout04->addWidget(labelTotalGroups);

	hbLayout05->addWidget(label05);
	hbLayout05->addItem(sp05);
	hbLayout05->addWidget(labelTotalDisks);
	
	hbLayout06->addWidget(label06);
	hbLayout06->addItem(sp06);
	hbLayout06->addWidget(labelDate);
	
	vbLayout1 = new QVBoxLayout;
	vbLayout1->setSpacing(0);
	vbLayout2 = new QVBoxLayout;
	vbLayout2->setSpacing(0);

	vbLayout1->addLayout(hbLayout01);
	vbLayout1->addLayout(hbLayout02);
	vbLayout1->addLayout(hbLayout03);
	
	vbLayout2->addLayout(hbLayout04);
	vbLayout2->addLayout(hbLayout05);
	vbLayout2->addLayout(hbLayout06);
	
	QFrame *vline1 = new QFrame;
	vline1->setFrameShape(QFrame::VLine);
	vline1->setFrameShadow(QFrame::Sunken);
	
	QGridLayout *grLay = new QGridLayout;
	grLay->addLayout(vbLayout1, 0, 0);
	grLay->addWidget(vline1, 0, 1);
	grLay->addLayout(vbLayout2, 0, 2);
	
	infoFrame->setLayout(grLay);
	
	ReportStruct report;
    dm->rpt->initGivenData(report);
    dm->rpt->getGroupReport(catId, itemId, report);
	nameLineEdit->setText(report.name);
	labelItemType->setText(tr("Disk Group"));
	labelTotalFiles->setText(QString::number(report.numFiles));
	labelTotalDirs->setText(QString::number(report.numDirs));
	labelTotalSize->setText(getSizeText(report.size));
	labelTotalGroups->setText(QString::number(report.groups));
	labelTotalDisks->setText(QString::number(report.disks));
	labelDate->setText(QDateTime::fromTime_t(report.dateTime).toString(Qt::LocalDate));
	textEditDescription->setText(report.description);
}

void CatalogPropertiesDlg::initIfDisk()
{
	hbLayout00 = new QHBoxLayout;
	hbLayout01 = new QHBoxLayout;
	hbLayout02 = new QHBoxLayout;
	hbLayout03 = new QHBoxLayout;
	hbLayout04 = new QHBoxLayout;
	hbLayout05 = new QHBoxLayout;
	hbLayout06 = new QHBoxLayout;

	vbLayout1 = new QVBoxLayout;
	vbLayout1->setSpacing(0);
	vbLayout2 = new QVBoxLayout;
	vbLayout2->setSpacing(0);
	
	sp01 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp02 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp03 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp04 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp05 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp06 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

	label01 = new QLabel(tr("Total Files: "));
	label02 = new QLabel(tr("Total Folders: "));
	label03 = new QLabel(tr("Total Size: "));
	label04 = new QLabel(tr("Disk Serial: "));
	label05 = new QLabel(tr("Last update: "));
	label06 = new QLabel(tr("Media Type: "));

	labelTotalFiles = new QLabel;
	labelTotalDirs = new QLabel;
	labelTotalSize = new QLabel;
	labelDate = new QLabel;
	labelDiskSerial = new QLabel;
	comboDiskType = new QComboBox;

	for (int i = 0; i < MEDIA_TYPES_COUNT; i++)
	{
		comboDiskType->addItem(QIcon(mediaTypesArray[i].iconFile), tr(mediaTypesArray[i].mediaName));
	}
	comboDiskType->setCurrentIndex(findIndexFromFullDiskType(itemAttr));
	
	hbLayout01->addWidget(label01);
	hbLayout01->addItem(sp01);
	hbLayout01->addWidget(labelTotalFiles);
	
	hbLayout02->addWidget(label02);
	hbLayout02->addItem(sp02);
	hbLayout02->addWidget(labelTotalDirs);
	
	hbLayout03->addWidget(label03);
	hbLayout03->addItem(sp03);
	hbLayout03->addWidget(labelTotalSize);
	
	hbLayout04->addWidget(label04);
	hbLayout04->addWidget(labelDiskSerial);
	hbLayout04->addItem(sp04);
	
	hbLayout05->addWidget(label05);
	hbLayout05->addWidget(labelDate);
	hbLayout05->addItem(sp05);

	hbLayout06->addWidget(label06);
	hbLayout06->addWidget(comboDiskType);
	hbLayout06->addItem(sp06);
	
	vbLayout1->addLayout(hbLayout01);
	vbLayout1->addLayout(hbLayout02);
	vbLayout1->addLayout(hbLayout03);
	
	vbLayout2->addLayout(hbLayout04);
	vbLayout2->addLayout(hbLayout05);
	vbLayout2->addLayout(hbLayout06);
	
	QFrame *vline1 = new QFrame;
	vline1->setFrameShape(QFrame::VLine);
	vline1->setFrameShadow(QFrame::Sunken);
	
	hbLayout00->addLayout(vbLayout1);
	hbLayout00->addWidget(vline1);
	hbLayout00->addLayout(vbLayout2);
	infoFrame->setLayout(hbLayout00);
	

	ReportStruct report;
    dm->rpt->initGivenData(report);
    dm->rpt->getDiskReportById(catId, itemId, report);
	labelItemType->setText(tr("Disk, No: ") + QString::number(report.diskNo));
	nameLineEdit->setText(report.name);
	labelTotalFiles->setText(QString::number(report.numFiles));
	labelTotalDirs->setText(QString::number(report.numDirs));
	labelTotalSize->setText(getSizeText(report.size));
	labelDate->setText(QDateTime::fromTime_t(report.dateTime).toString(Qt::LocalDate));
	labelDiskSerial->setText(getSerial(report.size));
	//labelTotalGroups->setText(QString::number(report.groups));
	//labelTotalDisks->setText(QString::number(report.disks));
	
	textEditDescription->setText(report.description);
	//getSerial(0x12345678);
	connect(comboDiskType, SIGNAL(currentIndexChanged(int)), this, SLOT(diskTypeTrigger(int)));

}

void CatalogPropertiesDlg::initIfFolder()
{
	hbLayout01 = new QHBoxLayout;
	hbLayout02 = new QHBoxLayout;
	hbLayout03 = new QHBoxLayout;
	hbLayout04 = new QHBoxLayout;
	hbLayout05 = new QHBoxLayout;
	hbLayout06 = new QHBoxLayout;
	
	sp01 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp02 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp03 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp04 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp05 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp06 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	
	label01 = new QLabel(tr("Total Files:"));
	label02 = new QLabel(tr("SubFolders:"));
	label03 = new QLabel(tr("Total Size:"));
	label04 = new QLabel(tr("Create date:"));
	label05 = new QLabel(tr("Create time:"));
	label06 = new QLabel(tr("Attributes:"));
	
	labelTotalFiles = new QLabel;
	labelTotalDirs = new QLabel;
	labelTotalSize = new QLabel;
	labelDate = new QLabel;
	labelTime = new QLabel;
	labelAttributes = new QLabel;
	
	hbLayout01->addWidget(label01);
	hbLayout01->addItem(sp01);
	hbLayout01->addWidget(labelTotalFiles);
	
	hbLayout02->addWidget(label02);
	hbLayout02->addItem(sp02);
	hbLayout02->addWidget(labelTotalDirs);
	
	hbLayout03->addWidget(label03);
	hbLayout03->addItem(sp03);
	hbLayout03->addWidget(labelTotalSize);
	
	hbLayout04->addWidget(label04);
	hbLayout04->addItem(sp04);
	hbLayout04->addWidget(labelDate);

	hbLayout05->addWidget(label05);
	hbLayout05->addItem(sp05);
	hbLayout05->addWidget(labelTime);
	
	hbLayout06->addWidget(label06);
	hbLayout06->addItem(sp06);
	hbLayout06->addWidget(labelAttributes);
	
	vbLayout1 = new QVBoxLayout;
	vbLayout1->setSpacing(0);
	vbLayout2 = new QVBoxLayout;
	vbLayout2->setSpacing(0);

	vbLayout1->addLayout(hbLayout01);
	vbLayout1->addLayout(hbLayout02);
	vbLayout1->addLayout(hbLayout03);
	
	vbLayout2->addLayout(hbLayout04);
	vbLayout2->addLayout(hbLayout05);
	vbLayout2->addLayout(hbLayout06);
	
	QFrame *vline1 = new QFrame;
	vline1->setFrameShape(QFrame::VLine);
	vline1->setFrameShadow(QFrame::Sunken);
	
	QGridLayout *grLay = new QGridLayout;
	grLay->addLayout(vbLayout1, 0, 0);
	grLay->addWidget(vline1, 0, 1);
	grLay->addLayout(vbLayout2, 0, 2);
	
	infoFrame->setLayout(grLay);
	
	ReportStruct report;
    dm->rpt->initGivenData(report);
    dm->rpt->getFolderReport(catId, itemId, report);
	nameLineEdit->setText(report.name);
	labelItemType->setText(tr("Folder"));
	labelTotalFiles->setText(QString::number(report.numFiles));
	labelTotalDirs->setText(QString::number(report.numDirs));
	labelTotalSize->setText(getSizeText(report.size));
	labelDate->setText(QDateTime::fromTime_t(report.dateTime).date().toString(Qt::LocalDate));
	labelTime->setText(QDateTime::fromTime_t(report.dateTime).time().toString(Qt::LocalDate));
    labelAttributes->setText(permsToQString(report.attributes));
	
	textEditDescription->setText(report.description);
}

void CatalogPropertiesDlg::initIfFile()
{
	hbLayout01 = new QHBoxLayout;
	hbLayout02 = new QHBoxLayout;
	hbLayout03 = new QHBoxLayout;
	hbLayout04 = new QHBoxLayout;
	hbLayout05 = new QHBoxLayout;
	hbLayout06 = new QHBoxLayout;
	
	sp01 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp02 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp03 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp04 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp05 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	sp06 = new QSpacerItem(16, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	
	label01 = new QLabel(tr("Total Files:"));
	label02 = new QLabel(tr("SubFolders:"));
	label03 = new QLabel(tr("Total Size:"));
	label04 = new QLabel(tr("Create date:"));
	label05 = new QLabel(tr("Create time:"));
	label06 = new QLabel(tr("Attributes:"));
	
	labelTotalFiles = new QLabel;
	labelTotalDirs = new QLabel;
	labelTotalSize = new QLabel;
	labelDate = new QLabel;
	labelTime = new QLabel;
	labelAttributes = new QLabel;
	
	hbLayout01->addWidget(label01);
	hbLayout01->addItem(sp01);
	hbLayout01->addWidget(labelTotalFiles);
	
	hbLayout02->addWidget(label02);
	hbLayout02->addItem(sp02);
	hbLayout02->addWidget(labelTotalDirs);
	
	hbLayout03->addWidget(label03);
	hbLayout03->addItem(sp03);
	hbLayout03->addWidget(labelTotalSize);
	
	hbLayout04->addWidget(label04);
	hbLayout04->addItem(sp04);
	hbLayout04->addWidget(labelDate);

	hbLayout05->addWidget(label05);
	hbLayout05->addItem(sp05);
	hbLayout05->addWidget(labelTime);
	
	hbLayout06->addWidget(label06);
	hbLayout06->addItem(sp06);
	hbLayout06->addWidget(labelAttributes);
	
	vbLayout1 = new QVBoxLayout;
	vbLayout1->setSpacing(0);
	vbLayout2 = new QVBoxLayout;
	vbLayout2->setSpacing(0);

	vbLayout1->addLayout(hbLayout01);
	vbLayout1->addLayout(hbLayout02);
	vbLayout1->addLayout(hbLayout03);
	
	vbLayout2->addLayout(hbLayout04);
	vbLayout2->addLayout(hbLayout05);
	vbLayout2->addLayout(hbLayout06);
	
	QFrame *vline1 = new QFrame;
	vline1->setFrameShape(QFrame::VLine);
	vline1->setFrameShadow(QFrame::Sunken);
	
	QGridLayout *grLay = new QGridLayout;
	grLay->addLayout(vbLayout1, 0, 0);
	grLay->addWidget(vline1, 0, 1);
	grLay->addLayout(vbLayout2, 0, 2);
	
	infoFrame->setLayout(grLay);
	
	ReportStruct report;
    dm->rpt->initGivenData(report);
    dm->rpt->getFolderReport(catId, itemId, report);
	nameLineEdit->setText(report.name);
	labelItemType->setText(tr("File, Type: "));
	labelTotalFiles->setText(QString::number(report.numFiles));
	labelTotalDirs->setText(QString::number(report.numDirs));
	labelTotalSize->setText(getSizeText(report.size));
	labelDate->setText(QDateTime::fromTime_t(report.dateTime).date().toString(Qt::LocalDate));
	labelTime->setText(QDateTime::fromTime_t(report.dateTime).time().toString(Qt::LocalDate));
    labelAttributes->setText(permsToQString(report.attributes));
	
	textEditDescription->setText(report.description);
}

QString CatalogPropertiesDlg::getSerial(int serial)
{
	union {
		unsigned short hi_lo[2];
		unsigned long int serial;
	}ser;
	
	ser.serial = serial;

	QString text = QString::number(ser.hi_lo[1], 16) + "-" + QString::number(ser.hi_lo[0], 16);
	return text;
	
	
}

QString CatalogPropertiesDlg::getSizeText(quint64 size)
{
	QString text;
	double newSize = size;
	text = QString::number(newSize);
	
	quint64 GB, MB, KB;
	GB = (1024 * 1024 * 1024);
	MB = (1024 * 1024);
	KB = 1024;
	if (size > GB)
	{
		newSize = (double)size / GB; // MB
		text = QString::number(newSize, 'f', 2) + " GB";
		return text;
	}
	if (size > MB)
	{
		newSize = (double)size / MB; // MB
		text = QString::number(newSize, 'f', 2) + " MB";
		return text;
	}
	if (size > KB)
	{
		newSize = (double)size / KB; // KB
		text = QString::number(newSize, 'f', 2) + " KB";
		return text;
	}
		
	return text;
}

void CatalogPropertiesDlg::passwordProtect()
{
    CatalogPasswordDlg dlg(dm->cat, catNode);
	dlg.exec();
}

void CatalogPropertiesDlg::iconManage()
{
    IconManagerDlg dlg(dm, this);
    dlg.customInit(catId, iconId, itemType, itemAttr);
	if (dlg.exec() == QDialog::Accepted)
	{
		int newIcon = dlg.getCurrentIcon();
		QPixmap *pix = 0;
		currIconCat = 0;
        currIconId = 0;
		if (newIcon)
		{
            pix = dm->im->getPixmap(catId, newIcon);
			currIconCat = catId;
			currIconId = newIcon;
            defaultIconUsed = false;
		}
		else{
            pix = dm->im->getDefaultPixmap(itemType, itemAttr);
            defaultIconUsed = true;
        }
		toolButtonIcon->setIconSize(QSize(32, 32));
		toolButtonIcon->setIcon(QIcon(*pix));
		delete pix;
		iconChanged = true;
		
	}
}

void CatalogPropertiesDlg::descriptionIsChanged()
{
	descChanged = true;
}

void CatalogPropertiesDlg::okPressed()
{
	if (itemType == ROOT_ELEMENT)
	{
		if (descChanged || iconChanged || autoOpenChanged || nameChanged)
		{
			if (descChanged)
			{
				// Update Catalog Description
				catNode->catDescription = textEditDescription->toPlainText();
			}
			if (iconChanged)
			{
				// Update Catalog Icon
				catNode->iconId = currIconId;
			}
			if (nameChanged)
			{
				catNode->catName = nameLineEdit->text();	
			}
			if (autoOpenChanged)
			{
				if (autoOpenCheckBox->checkState() == Qt::Checked)
				{
					printQS("Auto open checked");
					if (!(catNode->catFlags & CATALOG_OPENED))
					{
						catNode->catFlags = (catNode->catFlags | CATALOG_OPENED);
					}
				}else{
					printQS("Auto open unchecked");
					if (catNode->catFlags & CATALOG_OPENED)
					{
						catNode->catFlags = (catNode->catFlags ^ CATALOG_OPENED);
					}
				}
/*				
				// update Catalog AutoOpen
				if (autoOpenCheckBox->checkState() == Qt::Checked)
				{
				}else{
					
				}*/
			}
            dm->cat->updateCatalog(*catNode);
			thisChanged = true;
		}
	}else{
		// Other Element properties
		if (descChanged || iconChanged || diskTypeChanged || nameChanged)
		{
			dbNode data;
            dm->dbn->readNode(catId, itemId, data);
			if (descChanged)
			{
				data.nodeDesc = textEditDescription->toPlainText();
			}
			if (iconChanged)
			{
				data.iconId = currIconId;
			}
			if (diskTypeChanged)
			{
				data.nodeAttr = getFullDiskTypeFromIndex(comboDiskType->currentIndex());
				int basicType = getBasicDiskTypeFromIndex(comboDiskType->currentIndex());
				data.nodeFlags = setBasicDiskType(basicType, data.nodeFlags);
			}
			if (nameChanged)
			{
				data.nodeName = nameLineEdit->text();	
			}
            dm->dbn->updateNode(data);
			thisChanged = true;
		}
		
	}

}
/*
void CatalogPropertiesDlg::setCatalogClass(CatalogClass *_cat)
{
	cat = _cat;    
}*/

void CatalogPropertiesDlg::setCatalogNode(catalogNode *_node)
{
    catNode = _node;
}

void CatalogPropertiesDlg::autoOpenTrigger(int state)
{
	autoOpenChanged = true;
	
}

void CatalogPropertiesDlg::diskTypeTrigger(int index)
{
	diskTypeChanged = true;
    if (defaultIconUsed)
    {
     	// Default Icon - So we can change it
        QPixmap *pix = 0;
        pix = dm->im->getDefaultPixmap(DISK_ELEMENT, mediaTypesArray[index].fullType);
        toolButtonIcon->setIconSize(QSize(32, 32));
        toolButtonIcon->setIcon(QIcon(*pix));
        delete pix;
   
    }
}

void CatalogPropertiesDlg::nameIsChanged(const QString & text)
{
	nameChanged = true;
}