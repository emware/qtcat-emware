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
#include <QMessageBox>


#include "catalogpassworddlg.h"
#include "catalogclass.h"
#include "defsandtools.h"

CatalogPasswordDlg::CatalogPasswordDlg(CatalogClass *_cat, catalogNode *_node, QWidget *parent)
 : QDialog(parent), Ui::CatalogPasswordDialog()
{
	setupUi(this);
    cat = _cat;
    catNode = _node;
    if (catNode->isReadProtected || catNode->isWriteProtected){
    	passGroupBox->setChecked(true);    
        
    }else{
        passGroupBox->setChecked(false);    
    }
    if (catNode->isReadProtected){
        RPcheckBox->setChecked(true);
        rpLineEdit1->setText(catNode->catReadPass);
        rpLineEdit2->setText(catNode->catReadPass);
           
    }
    if (catNode->isWriteProtected){
        WPcheckBox->setChecked(true);   
        wpLineEdit1->setText(catNode->catWritePass);
        wpLineEdit2->setText(catNode->catWritePass);

    }
    
    connect(RPcheckBox, SIGNAL(stateChanged(int)), this, SLOT(rpStateChanged(int)));
    connect(WPcheckBox, SIGNAL(stateChanged(int)), this, SLOT(wpStateChanged(int)));
    connect(passGroupBox, SIGNAL(toggled(bool)), this, SLOT(grpBoxToggled(bool)));
    
}


CatalogPasswordDlg::~CatalogPasswordDlg()
{
    
}

void CatalogPasswordDlg::grpBoxToggled(bool _state)
{
    if (_state)
    {
        if (RPcheckBox->checkState() == Qt::Unchecked)
        {
            rpLineEdit1->setEnabled(false);   
            rpLineEdit2->setEnabled(false);   
        }else{
            rpLineEdit1->setEnabled(true);   
            rpLineEdit2->setEnabled(true);   
        }
        if (WPcheckBox->checkState() == Qt::Unchecked)
        {
            wpLineEdit1->setEnabled(false);   
            wpLineEdit2->setEnabled(false);   
        }else{
            wpLineEdit1->setEnabled(true);   
            wpLineEdit2->setEnabled(true);   
        }
    }else{
        RPcheckBox->setCheckState(Qt::Unchecked);   
        WPcheckBox->setCheckState(Qt::Unchecked);   
        rpLineEdit1->clear();
        rpLineEdit2->clear();
        wpLineEdit1->clear();
        wpLineEdit2->clear();
    }
    
}

void CatalogPasswordDlg::rpStateChanged(int _state)
{
	if (_state == Qt::Checked)
    {
        rpLineEdit1->setEnabled(true);   
        rpLineEdit2->setEnabled(true);   
    }else{
        rpLineEdit1->setEnabled(false);   
        rpLineEdit2->setEnabled(false);   
        rpLineEdit1->clear();
        rpLineEdit2->clear();
    }
}

void CatalogPasswordDlg::wpStateChanged(int _state)
{
    if (_state == Qt::Checked)
    {
        wpLineEdit1->setEnabled(true);   
        wpLineEdit2->setEnabled(true);   
    }else{
        wpLineEdit1->setEnabled(false);   
        wpLineEdit2->setEnabled(false);   
        wpLineEdit1->clear();
        wpLineEdit2->clear();
    }
    
}

void CatalogPasswordDlg::accept()
{
    bool rpMatch = true;
    bool wpMatch = true;
    bool rpProt = (RPcheckBox->checkState() == Qt::Unchecked) ? false : true;
    bool wpProt = (WPcheckBox->checkState() == Qt::Unchecked) ? false : true;
    
    if (rpProt)
    {
        // If Read password is empty - Uncheck
        if (rpLineEdit1->text().isEmpty() && rpLineEdit2->text().isEmpty())
        {
            RPcheckBox->setCheckState(Qt::Unchecked);
            rpProt = false;
        }
        rpMatch = (rpLineEdit1->text() == rpLineEdit2->text()) ? true : false;
     	   
    }
    if (wpProt)
    {
        // If Write password is empty - Uncheck
        if (wpLineEdit1->text().isEmpty() && wpLineEdit2->text().isEmpty())
        {
            WPcheckBox->setCheckState(Qt::Unchecked);
         	wpProt = false;   
        }
        wpMatch = (wpLineEdit1->text() == wpLineEdit2->text()) ? true : false;
    }
    
    if (!rpMatch)
    {
        QMessageBox::critical(this, tr("Error"), tr("The Read-protect passwords do not match!"));   
        return;
    }
    if (!wpMatch)
    {
		QMessageBox::critical(this, tr("Error"), tr("The Write-protect passwords do not match!"));   
		return;
    }
    catNode->isReadProtected = rpProt;
    catNode->isWriteProtected = wpProt;
    catNode->catReadPass = rpLineEdit1->text();
    catNode->catWritePass = wpLineEdit1->text();
    catNode->isReadUnlocked = (rpProt) ? true : false;
    catNode->isWriteUnlocked = (wpProt) ? true : false;
    cat->setPasswordFlags(*catNode, rpProt, wpProt);
    cat->updateCatalog(*catNode);
    done(QDialog::Accepted);
}



