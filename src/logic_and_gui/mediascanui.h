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
#ifndef MEDIASCANUI_H
#define MEDIASCANUI_H

#include <QDialog>

/**
	@author Victor Nike <victor.nike@gmail.com>
*/

class QVBoxLayout;
class QHBoxLayout;
class QGridLayout;
class QLabel;
class QTreeWidget;
class QTreeWidgetItem;
class QLineEdit;
class QPushButton;
class QComboBox;
class QSplitter;
class QProgressDialog;
class QTextEdit;
class QSpacerItem;
class QToolButton;
class QCheckBox;

class QCatDataModuleClass;
class DeviceClass;

class MediaScanUI : public QDialog
{
	Q_OBJECT
	public:
        MediaScanUI(QCatDataModuleClass *_dm, QWidget *parent = 0);
		~MediaScanUI();
		
		void setCatId(int _catId);
		void setParentId(int _parentId);
		void setParentName(const QString &_parentName);
		void makeProgress(int i);
        void loadItems();
		
	private slots:
		void startScan();
		void managePaths();
		void itemChanged(QTreeWidgetItem *newItem, QTreeWidgetItem *prevItem);
		
	private:
		void initGUI();
		bool loadingItems;		
		// new Layouts
		QVBoxLayout		*vbLayout4;
			QHBoxLayout		*hbLayout4;	
		
				QVBoxLayout 	*vbLayout1;
				QLabel 			*favsLabel;
				QTreeWidget		*pathsTree;
				QLabel			*pathLabel;
				QLineEdit		*pathLineEdit;
				
				QHBoxLayout		*hbLayout6;
				
				QVBoxLayout		*vbLayout3;
					QHBoxLayout		*hbLayout7;
					QLabel			*parentNameLabel;
					QLineEdit		*parentNameLineEdit;
					
					QVBoxLayout 	*vbLayout2;
					QLabel			*mediaNameLabel;
					QLineEdit		*mediaNameLineEdit;
				
					QHBoxLayout		*hbLayout1;
					QLabel			*serialLabel;
					QLineEdit		*serialLineEdit;
				
					QHBoxLayout		*hbLayout2;
					QLabel			*mediaTypeLabel;
					QComboBox		*mediaTypeCombo;
				
					QGridLayout		*grLayout1;
					QLabel			*commentLabel;
					QTextEdit		*commentTextEdit;
					QSpacerItem		*spacer1;
			// ------ End of hbLayout4		
					
			QHBoxLayout		*hbLayout5;
				QToolButton		*manageLocations;
				QCheckBox		*doNotCloseCheckBox;
			
				QSpacerItem		*spacer2;
				QPushButton		*okScanButton;
				QPushButton		*cancelButton;
				QSpacerItem		*spacer3;
		
        QProgressDialog *progressDlg;
		// NonGUI
        QCatDataModuleClass *dm;
		DeviceClass *device;
		bool mountByMe;
		//ConfigManager *settings;
		//dbNodeClass *dbn;
		
		int catId;
		int parentId;
		QString parentName;

};

#endif
