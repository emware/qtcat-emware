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
#ifndef ADDSCANPLACES_H
#define ADDSCANPLACES_H

#include <QDialog>

/**
	@author Victor Nike <victor.nike@gmail.com>
*/

#include <QGridLayout>
#include <QVBoxLayout>
#include <QHBoxLayout>
//class QGridLayout;
//class QVBoxLayout;
//class QHBoxLayout;
class QLabel;
class QTreeWidget;
class QTreeWidgetItem;
class QSpacerItem;
class QToolButton;
class QLineEdit;
class QComboBox;
class QPushButton;
class QCheckBox;

class ConfigManager;

class AddScanPlaces : public QDialog
{
	Q_OBJECT
	public:
    	AddScanPlaces(QWidget *parent = 0);

	    ~AddScanPlaces();
		
		void initGUI();
	
	
		void setConfigManager(ConfigManager *cm);
		void loadItems();
	private slots:
		void addNewPath();
        void updateCurrentPath();
		void removeCurrent();
		void cancelButtonClicked();
		void okButtonClicked();
		void fileDialogExec();
		void itemChanged(QTreeWidgetItem *newItem, QTreeWidgetItem *prevItem);
        
        void nameChanged(const QString &text);
        void pathChanged(const QString &text);
        void typeChanged(int index);
		
	private:
		ConfigManager *settings;
		bool bCurrentItemChanged;
		bool bAnyItemChanged;
#define GUI2
#ifdef	GUI1
	// GUI1
	QGridLayout		*grLayout1;
		// Cell 0, 0
		QVBoxLayout		*vbLayout1;
		QLabel			*placesLabel;
		QTreeWidget		*placesTree;
		
		QHBoxLayout		*hbLayout1;
			QSpacerItem		*spacer1;
			QToolButton		*addPlaceButton;
            QToolButton		*updatePlaceButton;
            QToolButton		*removePlaceButton;
		// End vbLayout2	
		// Cell 0, 1
		QVBoxLayout		*vbLayout2;
		QLabel			*placeLabel;
		QHBoxLayout		*hbLayout3;
			QLineEdit		*placeLineEdit;
			QToolButton		*fileDialogButton;
		QLabel			*nameLabel;
		QLineEdit		*nameLineEdit;
		QLabel			*placeComboLabel;
		QComboBox		*placeTypeCombo;
		// End vbLayout3
		//Cell 1, 0 - 1, 1
		QHBoxLayout		*hbLayout2;
		QSpacerItem		*spacer2;
		QPushButton		*okButton;
		QPushButton		*cancelButton;
#else		
		// GUI2
		QGridLayout		*grLayout1;
		// Cell 0, 0
			QVBoxLayout		*vbLayout1;
			QHBoxLayout		*hbLayout1;
				QLabel			*placesLabel;
			
				QHBoxLayout		*hbLayout2;
				QSpacerItem		*spacer1;
				QToolButton		*addPlaceButton;
                QToolButton		*updatePlaceButton;
				QToolButton		*removePlaceButton;
				
			QTreeWidget		*placesTree;
				
		// Cell 0, 1
			
		QVBoxLayout		*vbLayout2;
		QPushButton		*okButton;
		QPushButton		*cancelButton;
		QSpacerItem		*spacer2;
		
		// Cell 1, 0
		QGridLayout			*grLayout2;
			// Cell 0, 0
			QVBoxLayout		*vbLayout3;
			QLabel			*nameLabel;
			QLineEdit		*nameLineEdit;
			
			// Cell 0, 1
			QVBoxLayout		*vbLayout4;
			QLabel			*placeLabel;
			QHBoxLayout		*hbLayout3;
				QLineEdit		*placeLineEdit;
				QToolButton		*fileDialogButton;
				
			// Cell 0, 2
			QVBoxLayout		*vbLayout5;
			QLabel			*placeComboLabel;
			QComboBox		*placeTypeCombo;
			
			QVBoxLayout 	*vbLayout6;
			QCheckBox		*autoMountCheckBox;
			QSpacerItem		*spacer3;
#endif		

};

#endif
