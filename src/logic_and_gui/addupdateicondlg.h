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
#ifndef ADDUPDATEICONDLG_H
#define ADDUPDATEICONDLG_H

#include <QString>

#include <QDialog>
#include <ui_addupdateicon.h>

/**
	@author Victor Nike <victor.nike@gmail.com>
*/
class QPixmap;

class AddUpdateIconDlg : public QDialog, private Ui::AddUpdateIconDialog
{
    Q_OBJECT
    public:
		AddUpdateIconDlg(QWidget *parent = 0);
    
        ~AddUpdateIconDlg();
        
        void customInit(bool isUpdate, bool isProtectedName, QString &_iconName, const QPixmap &_pixmap);
        
        QString fileName;
        QString iconName;
    private slots:
        void browseFileFired();
        void iconAccepted();
    private:
        //QIcon *icon;
        //QPixmap pix;
        
        bool isUpdate;
        bool isProtectedName;
    
};

#endif
