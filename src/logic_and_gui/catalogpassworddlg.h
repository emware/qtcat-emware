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
#ifndef CATALOGPASSWORDDLG_H
#define CATALOGPASSWORDDLG_H

#include <QDialog>
#include <ui_catalog_pass.h>

/**
	@author Victor Nike <victor.nike@gmail.com>
*/

class CatalogClass;
struct catalogNode;

class CatalogPasswordDlg : public QDialog, private Ui::CatalogPasswordDialog
{
    Q_OBJECT
    public:
        CatalogPasswordDlg(CatalogClass *_cat, catalogNode *_node, QWidget *parent = 0);
    
        ~CatalogPasswordDlg();
        
    private slots:
        void grpBoxToggled(bool _state);
        void rpStateChanged(int _state);
        void wpStateChanged(int _state);
        virtual void accept();
    private:
        CatalogClass *cat;
        catalogNode *catNode;
};

#endif
