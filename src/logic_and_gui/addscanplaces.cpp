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
#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLabel>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QSpacerItem>
#include <QToolButton>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QStringList>
#include <QMessageBox>
#include <QFileDialog>
#include <QHeaderView>
#include <QCheckBox>

#include "defsandtools.h"
#include "configmanager.h"
#include "addscanplaces.h"

AddScanPlaces::AddScanPlaces(QWidget *parent)
 : QDialog(parent)
{
	initGUI();
}


AddScanPlaces::~AddScanPlaces()
{

}

void AddScanPlaces::setConfigManager(ConfigManager *cm)
{
	settings = cm;
}

void AddScanPlaces::initGUI()
{
	bCurrentItemChanged = true;
	bAnyItemChanged = true;
	
#ifdef GUI1	
	grLayout1 = new QGridLayout;
	
	vbLayout1 = new QVBoxLayout;
	vbLayout2 = new QVBoxLayout;
	
	hbLayout1 = new QHBoxLayout;
	hbLayout2 = new QHBoxLayout;
	hbLayout3 = new QHBoxLayout;
	
	placesLabel = new QLabel(tr("Favourite places:"));
	placesTree	= new QTreeWidget;
	vbLayout1->addWidget(placesLabel);
	vbLayout1->addWidget(placesTree);
	
	QStringList headers;
	headers << tr("Name") << "Path" << "Type" << "Hidden"; 
	placesTree->setHeaderLabels(headers);
	placesTree->setColumnHidden(3, true);
	
	spacer1 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	addPlaceButton = new QToolButton();
	addPlaceButton->setText(tr("+"));
    updatePlaceButton = new QToolButton();
    updatePlaceButton->setText(tr("update"));
	removePlaceButton = new QToolButton;
	removePlaceButton->setText(tr("-"));
	hbLayout1->addItem(spacer1);
    hbLayout1->addWidget(addPlaceButton);
    hbLayout1->addWidget(updatePlaceButton);
    hbLayout1->addWidget(removePlaceButton);
	
	vbLayout1->addLayout(hbLayout1);
	
	
	placeLabel = new QLabel(tr("Place to scan"));
	
		placeLineEdit = new QLineEdit;
		fileDialogButton = new QToolButton;
		fileDialogButton->setText(tr("..."));
		hbLayout3->addWidget(placeLineEdit);
		hbLayout3->addWidget(fileDialogButton);
	
	nameLabel = new QLabel(tr("Place name:"));
	nameLineEdit = new QLineEdit;
	placeComboLabel = new QLabel(tr("Device type"));
	placeTypeCombo = new QComboBox;
	
    placeTypeCombo->addItem(QIcon(getMediaTypeIconName(0)), tr("CD-ROM"));
    placeTypeCombo->addItem(QIcon(getMediaTypeIconName(1)), tr("DVD-ROM"));
    placeTypeCombo->addItem(QIcon(getMediaTypeIconName(2)), tr("HDD"));
    placeTypeCombo->addItem(QIcon(getMediaTypeIconName(3)), tr("Floppy"));
    placeTypeCombo->addItem(QIcon(getMediaTypeIconName(4)), tr("netDrive"));
    placeTypeCombo->addItem(QIcon(getMediaTypeIconName(5)), tr("USB Flash"));
    placeTypeCombo->addItem(QIcon(getMediaTypeIconName(6)), tr("Other"));
	
	vbLayout2->addWidget(placeLabel);
	vbLayout2->addLayout(hbLayout3);
	vbLayout2->addWidget(nameLabel);
	vbLayout2->addWidget(nameLineEdit);
	vbLayout2->addWidget(placeComboLabel);
	vbLayout2->addWidget(placeTypeCombo);
	
	spacer2 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
	okButton = new QPushButton(tr("Ok"));
	cancelButton = new QPushButton(tr("Cancel"));
	hbLayout2->addItem(spacer2);
	hbLayout2->addWidget(okButton);
	hbLayout2->addWidget(cancelButton);
	
	grLayout1->addLayout(vbLayout1, 0, 0, Qt::AlignTop);
	grLayout1->addLayout(vbLayout2, 0, 1, Qt::AlignTop);
	grLayout1->addLayout(hbLayout2, 1, 1);
	grLayout1->setColumnStretch(0, 3);
	grLayout1->setColumnStretch(1, 1);
	
#else
	grLayout1 = new QGridLayout;
	grLayout2 = new QGridLayout;
	
	vbLayout1 = new QVBoxLayout;
	vbLayout2 = new QVBoxLayout;
	vbLayout3 = new QVBoxLayout;
	vbLayout4 = new QVBoxLayout;
	vbLayout5 = new QVBoxLayout;
	vbLayout6 = new QVBoxLayout;
	
	hbLayout1 = new QHBoxLayout;
	hbLayout2 = new QHBoxLayout;
	hbLayout3 = new QHBoxLayout;
	
	// Start Cell 0, 0
	placesLabel = new QLabel(tr("Favourite places:"));
	hbLayout1->addWidget(placesLabel);
	
		spacer1 = new QSpacerItem(60, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
        addPlaceButton = new QToolButton();
        addPlaceButton->setText(tr("+"));
        updatePlaceButton = new QToolButton();
        updatePlaceButton->setText(tr("update"));
        removePlaceButton = new QToolButton;
		removePlaceButton->setText(tr("-"));
		hbLayout2->addItem(spacer1);
        hbLayout2->addWidget(addPlaceButton);
        hbLayout2->addWidget(updatePlaceButton);
        hbLayout2->addWidget(removePlaceButton);
	hbLayout1->addLayout(hbLayout2);
	
	
	placesTree	= new QTreeWidget;
	QStringList headers;
	headers << tr("Name") << "Path" << "Type" << "Mount";
	placesTree->setHeaderLabels(headers);
	placesTree->setRootIsDecorated(false);
	placesTree->header()->resizeSection(0, 120);
	placesTree->header()->resizeSection(1, 200);
	placesTree->header()->resizeSection(2, 120);
	placesTree->header()->resizeSection(3, 80);
	
	vbLayout1->addLayout(hbLayout1);
	vbLayout1->addWidget(placesTree);
	// End Cell 0, 0
	// Start Cell 0, 1
	spacer2 = new QSpacerItem(60, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
	okButton = new QPushButton(tr("Ok"));
	cancelButton = new QPushButton(tr("Cancel"));
	vbLayout2->addWidget(okButton);
	vbLayout2->addWidget(cancelButton);
	vbLayout2->addItem(spacer2);
	// End Cell 0, 1
	
		nameLabel = new QLabel(tr("Place name:"));
		nameLineEdit = new QLineEdit;
		vbLayout3->addWidget(nameLabel);
		vbLayout3->addWidget(nameLineEdit);
	
		placeLabel = new QLabel(tr("Path to scan"));
			placeLineEdit = new QLineEdit;
			fileDialogButton = new QToolButton;
			fileDialogButton->setText(tr("..."));
			hbLayout3->setSpacing(0);
			hbLayout3->addWidget(placeLineEdit);
			hbLayout3->addWidget(fileDialogButton);
		vbLayout4->addWidget(placeLabel);
		vbLayout4->addLayout(hbLayout3);
		
		placeComboLabel = new QLabel(tr("Device type"));
		placeTypeCombo = new QComboBox;
		
        placeTypeCombo->addItem(QIcon(getMediaTypeIconName(0)), tr("CD-ROM"));
        placeTypeCombo->addItem(QIcon(getMediaTypeIconName(1)), tr("DVD-ROM"));
        placeTypeCombo->addItem(QIcon(getMediaTypeIconName(2)), tr("HDD"));
        placeTypeCombo->addItem(QIcon(getMediaTypeIconName(3)), tr("Floppy"));
        placeTypeCombo->addItem(QIcon(getMediaTypeIconName(4)), tr("netDrive"));
        placeTypeCombo->addItem(QIcon(getMediaTypeIconName(5)), tr("USB Flash"));
        placeTypeCombo->addItem(QIcon(getMediaTypeIconName(6)), tr("Other"));
		
		vbLayout5->addWidget(placeComboLabel);
		vbLayout5->addWidget(placeTypeCombo);
		
		autoMountCheckBox = new QCheckBox;
		autoMountCheckBox->setText(tr("Mount before use"));
		spacer3 = new QSpacerItem(60, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
		
		//vbLayout6->addItem(spacer3);
		//vbLayout6->addWidget(autoMountCheckBox);
		
	grLayout2->addLayout(vbLayout3, 0, 0, Qt::AlignTop);
	grLayout2->addLayout(vbLayout4, 0, 1, Qt::AlignTop);
	grLayout2->addLayout(vbLayout5, 0, 2, Qt::AlignTop);
	
	grLayout2->addWidget(autoMountCheckBox, 0, 3, Qt::AlignBottom);
	//grLayout2->addLayout(vbLayout6, 0, 3, Qt::AlignTop);
	
	
	
	
	grLayout1->addLayout(vbLayout1, 0, 0, Qt::AlignTop);
	grLayout1->addLayout(vbLayout2, 0, 1, Qt::AlignTop);
	//grLayout1->addLayout(grLayout2, 1, 0, 1, 2);
	grLayout1->addLayout(grLayout2, 1, 0);
	grLayout1->setColumnStretch(0, 4);
	grLayout1->setColumnStretch(1, 1);
		
#endif
	setLayout(grLayout1);
    setWindowTitle(tr("Add new scan path"));
	//loadItems();
	
    connect(addPlaceButton, SIGNAL(clicked()), this, SLOT(addNewPath()));
    connect(updatePlaceButton, SIGNAL(clicked()), this, SLOT(updateCurrentPath()));
    connect(removePlaceButton, SIGNAL(clicked()), this, SLOT(removeCurrent()));
    
    
	connect(cancelButton, SIGNAL(clicked()), this, SLOT(close()));
	connect(okButton, SIGNAL(clicked()), this, SLOT(okButtonClicked()));
	connect(fileDialogButton, SIGNAL(clicked()), this, SLOT(fileDialogExec()));
	connect(placesTree, SIGNAL(currentItemChanged (QTreeWidgetItem*, QTreeWidgetItem*)), this, SLOT(itemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    
    connect(nameLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(nameChanged(const QString&)));
    connect(placeLineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(pathChanged(const QString&)));
    connect(placeTypeCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(typeChanged(int)));
	
	resize(700, 350);
	placesTree->setFocus();
}

void AddScanPlaces::addNewPath()
{
    bAnyItemChanged = true;
    QString name = nameLineEdit->text();
	QString place = placeLineEdit->text();
	if (name.isEmpty())
	{
		QMessageBox::critical(this, tr("Empty field"), tr("Name field is empty!"));
		return;
	}
	if (place.isEmpty())
	{
		QMessageBox::critical(this, tr("Empty field"), tr("Path field is empty!"));
		return;
	}
	if (placesTree->findItems(name, Qt::MatchExactly, 0).size() > 0){
		QMessageBox::critical(this, tr("Alreay exists"), tr("This name already exist!"));
		return;
	}
	if (placesTree->findItems(place, Qt::MatchExactly, 1).size() > 0){
		QMessageBox::warning(this, tr("Alreay exists"), tr("This scan path already exist!\nDuplicating."));
	}
	int _i = placeTypeCombo->currentIndex();
	if (_i < 2)
	{
		// Item is CD or DVD-ROM
		if (!place.startsWith("/dev/"))
		{
			QMessageBox::critical(this, tr("Wrong device"), tr("Because you have selected optical device it is mandatory to specify device hardware name.\n You can find it in /dev\n\nExample: /dev/cdrom, /dev/hdc, etc."));
			return;
			
		}
	}
	
	QTreeWidgetItem *item = new QTreeWidgetItem;
	item->setIcon(2, placeTypeCombo->itemIcon(_i));
	item->setText(0, nameLineEdit->text());
	item->setText(1, placeLineEdit->text());
	item->setText(2, placeTypeCombo->itemText(_i));
	if (autoMountCheckBox->checkState() == Qt::Checked)
		item->setText(3, tr("mount-it"));
	else
		item->setText(3, tr(""));
	item->setText(4, QString::number(_i));
	placesTree->addTopLevelItem(item);
}

void AddScanPlaces::updateCurrentPath()
{
    QTreeWidgetItem *item = placesTree->currentItem();
    
    QString oldName = item->text(0);
    QString oldPAth = item->text(1);
    int oldType = item->text(2).toInt();
    
    QString newName = nameLineEdit->text();
    QString newPath = placeLineEdit->text();
    int newType = placeTypeCombo->currentIndex();
    
    if (newName.isEmpty())
    {
        QMessageBox::critical(this, tr("Empty field"), tr("Name field is empty!"));
        return;
    }
    if (newPath.isEmpty())
    {
        QMessageBox::critical(this, tr("Empty field"), tr("Path field is empty!"));
        return;
    }
    
    
    
    if (oldName != newName)
    {
	    if (placesTree->findItems(newName, Qt::MatchExactly, 0).size() > 1){
    	    QMessageBox::critical(this, tr("Alreay exists"), tr("This name already exist!"));
	        return;
    	}
    }
    if (oldPAth != newPath)
    {
        if (placesTree->findItems(newPath, Qt::MatchExactly, 1).size() > 0){
            QMessageBox::warning(this, tr("Alreay exists"), tr("This scan path already exist!\nDuplicating."));
        }
    }
	
	if (newType < 2)
	{
		// Item is CD or DVD-ROM
		if (!newPath.startsWith("/dev/"))
		{
			QMessageBox::critical(this, tr("Wrong device"), tr("Because you have selected optical device it is mandatory to specify device hardware name.\n You can find it in /dev\n\nExample: /dev/cdrom, /dev/hdc, etc."));
			return;
			
		}
	}
	
    bAnyItemChanged = true;
	
    item->setIcon(2, placeTypeCombo->itemIcon(newType));
    item->setText(0, newName);
    item->setText(1, newPath);
    item->setText(2, placeTypeCombo->itemText(newType));
	if (autoMountCheckBox->checkState() == Qt::Checked)
		item->setText(3, tr("mount-it"));
	else
		item->setText(3, tr(""));
	item->setText(4, QString::number(newType));
    //placesTree->addTopLevelItem(item);
}

void AddScanPlaces::removeCurrent()
{
    bAnyItemChanged = true;
    QTreeWidgetItem *item = placesTree->currentItem();
	int index = placesTree->indexOfTopLevelItem(item);
	placesTree->takeTopLevelItem(index);
	delete item;
	
}

void AddScanPlaces::fileDialogExec()
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"), placeLineEdit->text(), QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
	placeLineEdit->setText(dir);
}

void AddScanPlaces::cancelButtonClicked()
{
	reject();
}

void AddScanPlaces::okButtonClicked()
{
	//close();
    if (bAnyItemChanged)
    {
        settings->scanPlaces.clear();
        for (int i = 0; i < placesTree->topLevelItemCount(); ++i)
        {
            QTreeWidgetItem *item = placesTree->topLevelItem(i);
            structScanPlaces sp;
            sp.name = item->text(0);
            sp.path = item->text(1);	
            sp.itemType = item->text(4).toInt();
			sp.useMount = (item->text(3).isEmpty()) ? false : true;
            settings->scanPlaces.append(sp);
        }
    }
	accept();
	
}
void AddScanPlaces::loadItems()
{
	//close();
	
	for (int i = 0; i < settings->scanPlaces.size(); ++i)
	{
		QTreeWidgetItem *item = new QTreeWidgetItem;
		int itemType = settings->scanPlaces.at(i).itemType;
		item->setText(0, settings->scanPlaces.at(i).name);
		item->setIcon(2, placeTypeCombo->itemIcon(itemType));
		item->setText(1, settings->scanPlaces.at(i).path);
		item->setText(2, placeTypeCombo->itemText(itemType));
		if (settings->scanPlaces.at(i).useMount)
			item->setText(3, tr("mount-it"));
			
		item->setText(4, QString::number(itemType));
		placesTree->addTopLevelItem(item);
	}

}

void AddScanPlaces::itemChanged(QTreeWidgetItem *newItem, QTreeWidgetItem *prevItem)
{
	nameLineEdit->setText(newItem->text(0));
	placeLineEdit->setText(newItem->text(1));
	int i = newItem->text(4).toInt();
	placeTypeCombo->setCurrentIndex(i);
	if (newItem->text(3).isEmpty())
		autoMountCheckBox->setCheckState(Qt::Unchecked);
	else
		autoMountCheckBox->setCheckState(Qt::Checked);
}

void AddScanPlaces::nameChanged(const QString &text)
{
    bCurrentItemChanged = true;
    /*
    QTreeWidgetItem *item = placesTree->currentItem();
    item->setText(0, text);
    */
}

void AddScanPlaces::pathChanged(const QString &text)
{
    bCurrentItemChanged = true;
    /*
    QTreeWidgetItem *item = placesTree->currentItem();
    item->setText(1, text);
    */
}

void AddScanPlaces::typeChanged(int index)
{
    bCurrentItemChanged = true;
    
    /*
    QTreeWidgetItem *item = placesTree->currentItem();
    QString text = placeTypeCombo->currentText();
    item->setText(2, text);
    item->setIcon(2, QIcon(placeTypeCombo->itemIcon(index)));
    item->setText(3, QString::number(index));
    */
}



