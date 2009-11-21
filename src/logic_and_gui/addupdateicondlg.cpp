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
#include <QWidget>
#include <QIcon>
#include <QVBoxLayout>
#include <QPixmap>
#include <QFileDialog>

#include "addupdateicondlg.h"
// 
AddUpdateIconDlg::AddUpdateIconDlg(QWidget *parent)
 : QDialog(parent), Ui::AddUpdateIconDialog()
{
    setupUi(this);
    isUpdate = false;
    isProtectedName = false;
    connect(browseToolButton, SIGNAL(clicked()), this, SLOT(browseFileFired()));
    connect(this, SIGNAL(accepted()), this, SLOT(iconAccepted()));
    
}


AddUpdateIconDlg::~AddUpdateIconDlg()
{
//    delete icon;
}

void AddUpdateIconDlg::customInit(bool isUpdate, bool isProtectedName, QString &_iconName, const QPixmap &_pixmap)
{
    if (isProtectedName)
        nameLineEdit->setEnabled(false);
	else
        nameLineEdit->setEnabled(true);
    if (!_iconName.isEmpty())
 		nameLineEdit->setText(_iconName);
    
    if (isUpdate && !_pixmap.isNull())
        picLabel->setPixmap(_pixmap);
    
}

void AddUpdateIconDlg::browseFileFired()
{
    fileName = QFileDialog::getOpenFileName(this, tr("Select Icon"), "", tr("Images (*.png *.xpm *.jpg)"));
    fileNameLineEdit->setText(fileName);
    QPixmap pix(fileName);
    if ((pix.height() > 48) || (pix.width() > 48))
        pix = pix.scaled(QSize(48, 48)); 
    picLabel->setPixmap(pix);
    
}

void AddUpdateIconDlg::iconAccepted()
{
    iconName = nameLineEdit->text();
    
}
        