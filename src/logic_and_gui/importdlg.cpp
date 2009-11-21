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
#include <QCheckBox>
#include <QFileDialog>
#include <QXmlInputSource>
#include <QXmlSimpleReader>
#include <QMessageBox>

#include "defsandtools.h"
#include "datamodule.h"
#include "importdlg.h"
#include "configmanager.h"
#include "whereisitxmlimportclass.h"
#include "catalogclass.h"
#include "progresswidget.h"
#include "iconmanager.h"

ImportDlg::ImportDlg(QCatDataModuleClass *_dm, QWidget *parent)
	: dm(_dm), QDialog(parent), Ui::ImportDialog()		

{
	createNewCat = true;
	setupUi(this);
	catalogsComboBox->setVisible(false);
	catalogNameLineEdit->setVisible(true);
    
    for (int i = 0; i < dm->cat->items.count(); i++)
    {
        QPixmap *pix = 0;
        catalogNode *catNode = dm->cat->items.at(i);
        if (catNode->iconId != 0)
            pix = dm->im->getPixmap(catNode->catId, catNode->iconId);
        else
            pix = dm->im->getPixmap(0, CATALOG_ICON_ENABLED);
        
     	catalogsComboBox->addItem(QIcon(*pix), catNode->catName);   
        delete pix;
    }
	
	prg = new ProgressWidget(2);
	prg->setProgressBarTitle(0, "Current Progress");
	prg->setProgressBarTitle(1, "Total Progress");
	//prg->setProgressBarMinMax(0, 0, 100);
	//prg->setProgressBarMinMax(1, 0, 500);
	
	//prg->setProgressValue(0, 20);
	//prg->setProgressValue(1, 75);
	QVBoxLayout *_l = new QVBoxLayout;
	_l->addWidget(prg);
	
	progressGroupBox->setLayout(_l);
	
	connect(createNewCheckBox, SIGNAL(stateChanged(int)), this, SLOT(createNewStateChanged(int)));
	connect(browseToolButton, SIGNAL(clicked()), this, SLOT(browseForFile()));
}


ImportDlg::~ImportDlg()
{
	
}

void ImportDlg::createNewStateChanged(int _state)
{
	if (_state == Qt::Checked)
	{
		createNewCat = true;
		catalogsComboBox->setVisible(false);
		catalogNameLineEdit->setVisible(true);
	}
	else{
		createNewCat = false;
		catalogsComboBox->setVisible(true);
		catalogNameLineEdit->setVisible(false);
	}
	
}

void ImportDlg::browseForFile()
{
	QString fileName = QFileDialog::getOpenFileName(this, tr("Select WhereIsIt export file"), dm->settings->lastDir, tr("WhereIsIt export (*.xml)"));
	fileNameLineEdit->setText(fileName);
}

void ImportDlg::import_db()
{
	int catId = 0;
	if (createNewCat)
	{
		QString catName = catalogNameLineEdit->text();
		catalogNode data;
		dm->cat->initGivenData(data);
		data.catName = catName;
		dm->cat->createCatalog(data);
		catId = data.catId;

	}else{
		catId = dm->cat->items.at(catalogsComboBox->currentIndex())->catId;
	}
	QString fileName = fileNameLineEdit->text();
	QFile file(fileName);
	QXmlInputSource inputSource(&file);
	QXmlSimpleReader reader;
	
	Work_ImportWhereIsItItem wc(dm, catId);
	wc.setMode(PROCESS_JUST_COUNT);
    wc.setProgressObject(prg, 0);
    
	WhereIsItXmlImportClass imp;
	imp.setJustCountElements(true);
	imp.setWorkClass(&wc);
	reader.setContentHandler(&imp);
	reader.setErrorHandler(&imp);
	reader.parse(inputSource);
	if ( !imp.isParseSuccessful() )
	{
		QMessageBox::warning(this, QObject::tr("SAX Handler"), imp.getParseError());
		return;
	}
    
    int _groups = imp._diskGroupsNum;
    int _disks = imp._disks;
    int _folders = imp._folders;
    int _files = imp._files;
    
    prg->setProgressBarMinMax(1, 0, 6);
    
	imp.setJustCountElements(false);
	wc.setMode(PROCESS_GROUPS);
    prg->setProgressBarMinMax(0, 0, _groups);
    prg->setProgressBarTitle(0, "Parsing Groups");
    inputSource.reset();
    reader.parse(inputSource);
    prg->stepProgressBar(1);
	
	wc.setMode(PROCESS_DISKS);
    prg->setProgressBarMinMax(0, 0, _disks);
    prg->setProgressBarTitle(0, "Parsing Disks");
    inputSource.reset();
	reader.parse(inputSource);
    prg->stepProgressBar(1);
	
	wc.setMode(PROCESS_FOLDERS);
    prg->setProgressBarMinMax(0, 0, _folders);
    prg->setProgressBarTitle(0, "Parsing Folders");
    inputSource.reset();
	reader.parse(inputSource);
    prg->stepProgressBar(1);
	
    prg->setProgressBarMinMax(0, 0, _disks);
    prg->setProgressBarTitle(0, "Importing Disks");
    wc.saveDisks();
    prg->stepProgressBar(1);
    prg->setProgressBarMinMax(0, 0, _folders);
    prg->setProgressBarTitle(0, "Importing Folders");
    wc.saveAllFolders();
    prg->stepProgressBar(1);
	
    wc.setMode(PROCESS_FILES);
    prg->setProgressBarMinMax(0, 0, _files);
    prg->setProgressBarTitle(0, "Importing Files");
    inputSource.reset();
	reader.parse(inputSource);
    prg->stepProgressBar(1);

	printQS(QString("GROUPS = %1\nDISKS = %2\nFOLDERS = %3\nFILES = %4").arg(imp._diskGroupsNum).arg(imp._disks).arg(imp._folders).arg(imp._files));

	
	
}

void ImportDlg::accept()
{
	import_db();
	printQS("Accepted import");
	done(QDialog::Accepted);
}


