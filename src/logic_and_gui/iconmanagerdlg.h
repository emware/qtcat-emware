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
#ifndef ICONMANAGERDLG_H
#define ICONMANAGERDLG_H

#include <QDialog>
#include <QSqlDatabase>
#include <ui_iconmanagedialog.h>

/**
	@author Victor Nike <victor.nike@gmail.com>
*/
class IconManager;
class QListWidgetItem;
class QCatDataModuleClass;

class IconManagerDlg : public QDialog, private Ui::IconManageDialog
{
	Q_OBJECT
	public:
        IconManagerDlg(QCatDataModuleClass *_dm, QWidget *parent = 0);
        IconManagerDlg(IconManager *_im, QWidget *parent = 0);
		
		~IconManagerDlg();
			
        void customInit(int _catId, int _iconId, int _itemType, int _itemAttr);
		
		int getCurrentIcon();
    
    private slots:
        void addClicked();
        void updateClicked();
        void removeClicked();
        void saveClicked();
		void acceptedFired();
        void defaultClicked();
        void itemChanged(QListWidgetItem *currentItem, QListWidgetItem *prevItem);
	private:
        QCatDataModuleClass *dm;
        int catId;
        int maxIdNum;
		int currIcon;
        int itemType;
        int itemAttr;
        bool useDefaultIcon;
        
        void reloadIcons();
};

#endif
