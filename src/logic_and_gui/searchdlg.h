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
#ifndef SEARCHDLG_H
#define SEARCHDLG_H

#include <QDialog>
#include <QMap>
#include <QSqlDatabase>
#include <QTreeWidgetItem>
#include <ui_searchdialog.h>


/**
	@author Victor Nike <victor.nike@gmail.com>
*/

class QCatDataModuleClass;
class QSqlQuery;

class SearchDlg : public QDialog, private Ui::SearchDialog
{
    Q_OBJECT
    public:
        SearchDlg(QCatDataModuleClass *_dm, int _catId, QWidget *parent = 0);
    
        ~SearchDlg();
        
        int getReturnedId() { return returnedId; }
    private slots:
        void searchClicked();
        void settingsClicked();
        void caseSensitiveStateChanged(int state);
        void searchInFilesStateChanged(int state);
        void searchInDirsStateChanged(int state);
        void searchInDescStateChanged(int state);      
        void itemIsActivated(QTreeWidgetItem *_item, int column);  
	
	protected:
		virtual void resizeEvent(QResizeEvent *event);
		
    private:
		void fillRes(QSqlQuery *q);
		QString getDiskName(int diskId);
        QString dbConnName;
        QCatDataModuleClass *dm;
        int catId;
        int returnedId;
		QMap <int, QString> diskNames;
        
        bool searchInDirs;
        bool searchInFiles;
        bool searchInDescriptions;
        bool caseSensitiveSearch;
        
};

#endif
