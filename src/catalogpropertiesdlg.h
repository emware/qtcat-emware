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
#ifndef CATALOGPROPERTIESDLG_H
#define CATALOGPROPERTIESDLG_H

#include <QDialog>
#include <QCheckBox>
#include <ui_catalog_properties.h>

/**
	@author Victor Nike <victor.nike@gmail.com>
*/
class CatalogReports;
class IconManager;
class dbNodeClass;
class CatalogClass;
class catalogNode;
class QComboBox;
class QCatDataModuleClass;

class CatalogPropertiesDlg : public QDialog, private Ui::CatalogPropertiesDialog
{
	Q_OBJECT
	public:
		CatalogPropertiesDlg(QCatDataModuleClass *_dm, QWidget *parent = 0);
	
		~CatalogPropertiesDlg();
		
        void customInit(int _itemType, int _catId, int _itemId, int _iconId);
		
        void setFlagsAndAttr(int _flags, int _attr); // if We show Disk
        //void setCatalogClass(CatalogClass *_cat); // if we show Catalog
        void setCatalogNode(catalogNode *_node); // if we show Catalog
        
		bool isIconChanged() { return iconChanged; }
		int getCurrIconId() { return currIconId; }
		int getCurrIconCat() { return currIconCat; }
        
		bool isChanged() { return thisChanged; } 
	private slots:
		void passwordProtect();
		void iconManage();
		void descriptionIsChanged();
		void okPressed();
		void autoOpenTrigger(int state);
		void diskTypeTrigger(int index);
		void nameIsChanged(const QString & text);
	private:
		
		void initIfCatalog();
		void initIfGroup();
		void initIfDisk();
		void initIfFolder();
		void initIfFile();
		QString getSizeText(quint64 size);
		QString getSerial(int serial);
		

		//QString dbConnName;
		int catId;
		int itemId;
		int itemType;
		int iconId;
		int itemFlags;
		int itemAttr;
		int currIconId;
		int currIconCat;
		//CatalogReports *rpt;
        QCatDataModuleClass *dm;
		//IconManager *im;
		//dbNodeClass *dbn;
        //CatalogClass *cat;
        catalogNode *catNode;
        bool defaultIconUsed;
		bool descChanged;
		bool iconChanged;
		bool autoOpenChanged;
		bool diskTypeChanged;
		bool nameChanged;
		bool thisChanged;
		
		//GUI Elements
		// Common Elements
		QLabel *labelTotalFiles;
		QLabel *labelTotalDirs;
		QLabel *labelTotalSize;
		QLabel *labelTotalGroups;
		QLabel *labelTotalDisks;
		QLabel *labelDate;
		QLabel *labelTime;

		// Catalog Propertios
		QLabel *labelDVDRoms;
		QLabel *labelCDRoms;
		QLabel *labelHDDs;
		QLabel *labelUSBs;
		QLabel *labelNetDrives;
		QLabel *labelOther;
		
		QCheckBox *autoOpenCheckBox;
		
		// Group Properties
		
		// Disk Properties
		QLabel *labelDiskSerial;
		QComboBox *comboDiskType;
		
		// File, Folder Properties
		QLabel *labelAttributes;
		
		//Layouts
		QVBoxLayout *vbLayout0;
		QVBoxLayout *vbLayout1;
		QVBoxLayout *vbLayout2;
		QVBoxLayout *vbLayout3;
		QVBoxLayout *vbLayout4;
		
		QHBoxLayout *hbLayout00;
		QHBoxLayout	*hbLayout01;
		QHBoxLayout	*hbLayout02;
		QHBoxLayout	*hbLayout03;
		QHBoxLayout	*hbLayout04;
		QHBoxLayout	*hbLayout05;
		QHBoxLayout	*hbLayout06;
		QHBoxLayout	*hbLayout07;
		QHBoxLayout	*hbLayout08;
		QHBoxLayout	*hbLayout09;
		QHBoxLayout	*hbLayout10;
		QHBoxLayout	*hbLayout11;
		QHBoxLayout	*hbLayout12;
		
		// Labels
		QLabel *label01;
		QLabel *label02;
		QLabel *label03;
		QLabel *label04;
		QLabel *label05;
		QLabel *label06;
		QLabel *label07;
		QLabel *label08;
		QLabel *label09;
		QLabel *label10;
		QLabel *label11;
		QLabel *label12;
		
		// Spacer Items
		QSpacerItem *sp01;
		QSpacerItem *sp02;
		QSpacerItem *sp03;
		QSpacerItem *sp04;
		QSpacerItem *sp05;
		QSpacerItem *sp06;
		QSpacerItem *sp07;
		QSpacerItem *sp08;
		QSpacerItem *sp09;
		QSpacerItem *sp10;
		QSpacerItem *sp11;
		QSpacerItem *sp12;
		

};

#endif
