/***************************************************************************
 *   Copyright (C) 2007 by Victor Nike   *
 *   victor.nikiforov@bsc.bg   *
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

#include <cstdlib>
#include <iostream>
#include <cstring>

#include <QtGui>
#include <QSplitter>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTableView>
#include <QDirModel>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QAbstractItemView>
#include <QTreeView>

#include "mainwindow.h"
#include "configmanager.h"
#include "datamodule.h"
#include "dbnode.h"
#include "defsandtools.h"
#include "catalogtree.h"
#include "catalogtable.h"
#include "rightpanel.h"
#include "iconmanager.h"
#include "dbnode.h"
#include "catalogclass.h"
#include "iconmanagerdlg.h"
#include "searchdlg.h"
#include "importdlg.h"
#include "addscanplaces.h"

/*#define APPNAME "MyTest"*/

MainWindow::MainWindow()
{
	printQS("Constructing MainWindow");
    dm = NULL;
	
    catalogTree = NULL;
    rightPanel = NULL;
	
    readSettings();
	
	
    if (settings->autoOpenLastFile)
        loadFile(settings->lastFile);

    centralFrame = new QFrame(this);
    
    QHBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->setSpacing(0);
    centralFrame->setLayout(layout);
    
    catalogTree = new CatalogTree(dm, this);
    
	rightPanel = new RightPanel(dm, this);
    horiz = new QSplitter(Qt::Horizontal);
    horiz->addWidget(catalogTree);
    horiz->addWidget(rightPanel);
    horiz->setStretchFactor(0, 2);
    horiz->setStretchFactor(1, 5);
    layout->addWidget(horiz);
    
    setCentralWidget(centralFrame);
    createActions();
    createMenus();
    createToolBars();
    createStatusBar();
    catalogTree->loadCatalogs();
    

    connect(catalogTree, SIGNAL(loadCatalog()), this, SLOT(open()));
    
    connect(catalogTree, SIGNAL(newItemSelected(int, int)), this, SLOT(treeNewItemSelected(int, int)));
    
    connect(rightPanel, SIGNAL(goUp()), catalogTree, SLOT(goUpOneLevel()));
    connect(rightPanel, SIGNAL(goDown(int)), catalogTree, SLOT(goDownOneLevel(int)));
	connect(rightPanel, SIGNAL(canceledDrag()), catalogTree, SLOT(stopAnimation()));
    
}

MainWindow::~MainWindow()
{
    //delete rightPanel;
	//delete catalogTree;

}	

void MainWindow::readSettings()
{
    settings = new ConfigManager;
    settings->loadSettings();
    
    resize(settings->appSize);
    move(settings->appPos);
    std::cout << "Last file used is " << settings->lastFile.toStdString() << std::endl;
    
}

void MainWindow::loadFile(QString &fileName)
{
	printQS("Load file " + fileName);
	if (fileName.isEmpty())
	{
		fileName = QDir::home().absolutePath() + "/defaultCatalog.qcat";
	}
	settings->lastFile = fileName;
    settings->lastDir = strippedDir(fileName);
    if (dm)
    {
     	closeCatalog();   
    }

    dm = new QCatDataModuleClass;
    dm->openDb(fileName);
    dm->settings = settings;
    
    if (catalogTree)
    {
        catalogTree->changeDataModule(dm);
    }
    if (rightPanel)
    {
     	rightPanel->changeDataModule(dm);   
    }
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File loaded."), 2000);
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    if (maybeSave()){
        writeSettings();
        event->accept();
    }else{
        event->ignore();
    }
}

void MainWindow::newFile()
{
    if (maybeSave()){
        QString fileName = QFileDialog::getSaveFileName(this, tr("New Catalog"), settings->lastDir, "*.qcat");
        if (fileName.isEmpty())
            return;
		if (!fileName.endsWith(".qcat"))
			fileName += ".qcat";
        loadFile(fileName);
        setCurrentFile(fileName);
    }
}

void MainWindow::treeNewItemSelected(int newCatalog, int newId)
{
    rightPanel->refreshItems(newCatalog, newId);
}

void MainWindow::tableNewItemSelected(QString itemDesc)
{
    statusBar()->showMessage(itemDesc);   
}

void MainWindow::search()
{
    
	SearchDlg dlg(dm, catalogTree->getCurrentCatalog(), this);
	dlg.exec();
    catalogTree->findItemByItemId(catalogTree->getCurrentCatalog(), dlg.getReturnedId());
    
}

void MainWindow::open()
{
    if (maybeSave())
    {
        QString fileName = QFileDialog::getOpenFileName(this, tr("Open Catalog DB"), settings->lastDir, "*.qcat");
        if (!fileName.isEmpty()){
            loadFile(fileName);
            //catalogTree->refreshItems();
        }
    }
}

bool MainWindow::save()
{
    if (curFile.isEmpty())
    {
        return saveAs();
    }else{
        return saveFile(curFile);
    }
}

bool MainWindow::saveAs()
{
    QString fileName = QFileDialog::getSaveFileName(this);
    if (fileName.isEmpty())
        return false;
    
    return saveFile(fileName);
                
}

bool MainWindow::closeCatalog()
{
    if (catalogTree)
    {
        catalogTree->changeDataModule(NULL);
    }
    if (rightPanel)
    {
        rightPanel->changeDataModule(NULL);
    }
    delete dm;
    dm = NULL;
    std::cout << "Close catalog action\n";    
    return false;
}

void MainWindow::addMedia()
{
    std::cout << "Add Media action\n";    
    
}

void MainWindow::addGroup()
{
    std::cout << "Add Group action\n";    

}

void MainWindow::manageIcons()
{
    
	IconManagerDlg dlg(dm, this);
    dlg.customInit(0, 0, 0, 0);
    dlg.exec();    
    
}

void MainWindow::importExternalDB()
{
	ImportDlg dlg(dm);
    if (dlg.exec())
        catalogTree->loadCatalogs();
}


void MainWindow::about()
{
    QMessageBox::about(this, tr("About " APPNAME), 
                       tr("This is " APP_NAME " - a CD/DVD catalog application \n"
	                     "Version " APP_VER "\nLicence - GPLv2!\nAutor Victor Nikiforov\nemail: kernel_daemon@users.sourceforge.net"));
}

void MainWindow::documentWasModified()
{
//    setWindowModified(textEdit->document()->isModified());
}

void MainWindow::createActions()
{
    // File actions
    newCatalogAct = new QAction(QIcon(":icons/interface/new.png"), tr("&New"), this);
    newCatalogAct->setShortcut(tr("CTRL+N"));
    newCatalogAct->setStatusTip(tr("Create a new file"));
    connect(newCatalogAct, SIGNAL(triggered()), this, SLOT(newFile()));
    
    openAct = new  QAction(QIcon(":icons/interface/open.png"), tr("&Open..."), this);
    openAct->setShortcut(tr("CTRL+O"));
    openAct->setStatusTip(tr("Open a file..."));
    connect(openAct, SIGNAL(triggered()), this, SLOT(open()));
    
    saveAct = new QAction(QIcon(":icons/interface/save.png"), tr("&Save"), this);
    saveAct->setShortcut(tr("CTRL+S"));
    saveAct->setStatusTip(tr("Save current file to disk."));
    saveAct->setEnabled(false);
    connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
    
    saveAsAct = new QAction(tr("S&ave As..."), this);
    saveAsAct->setShortcut(tr("CTRL+F12"));
    saveAsAct->setStatusTip(tr("Save the file with new name."));
    connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveAs()));
    
    recentAct = new QAction(tr("Recent files..."), this);
    recentAct->setStatusTip(tr("Recent files."));
    
    closeCatalogAct = new QAction(QIcon(":icons/interface/fileclose.png"), tr("Close Catalog"), this);
    closeCatalogAct->setShortcut(tr("CTRL+W"));
    closeCatalogAct->setStatusTip(tr("Close current catalog."));
    connect(closeCatalogAct, SIGNAL(triggered()), this, SLOT(closeCatalog()));
    
    exitAct = new QAction(tr("E&xit"), this);
    exitAct->setShortcut(tr("Ctrl+Q"));
    exitAct->setStatusTip(tr("Exit the application"));
    connect(exitAct, SIGNAL(triggered()), this, SLOT(close()));

    //Edit Actions
    
    addMediaAct = new QAction(QIcon(":/icons/interface/add.png"), tr("Add media"), this);
    addMediaAct->setShortcut(tr("CTRL+D"));
    addMediaAct->setStatusTip(tr("Scans new media and add to catalog."));
    //connect(addMediaAct, SIGNAL(triggered()), this, SLOT(addMedia()));
    connect(addMediaAct, SIGNAL(triggered()), catalogTree, SLOT(addNewDisk()));
    
    
    addGroupAct = new QAction(tr("Add Media Group"), this);
    addGroupAct->setStatusTip(tr("Add new group to catalog."));
    connect(addGroupAct, SIGNAL(triggered()), catalogTree, SLOT(addNewGroup()));
    
    deleteMediaAct = new QAction(QIcon(":/icons/interface/remove.png"), tr("Delete"), this);
    deleteMediaAct->setShortcut(tr("Del"));
    deleteMediaAct->setStatusTip(tr("Delete media from catalog."));
    connect(deleteMediaAct, SIGNAL(triggered()), catalogTree, SLOT(deleteItem()));
    
    updateMediaAct = new QAction(tr("Update Media"), this);
    updateMediaAct->setShortcut(tr("CTRL+U"));
    updateMediaAct->setStatusTip(tr("Update media to catalog."));
    updateMediaAct->setEnabled(false);
    
    renameAct = new QAction(tr("Rename"), this);
    renameAct->setShortcut(tr("F2"));
    renameAct->setStatusTip(tr("Rename current object"));
    connect(renameAct, SIGNAL(triggered()), catalogTree, SLOT(renameItem()));
   
    propertiesAct = new QAction(tr("Properties"), this);
    propertiesAct->setShortcut(tr("ALT+Enter"));
    propertiesAct->setStatusTip(tr("Properties for current object."));
    connect(propertiesAct, SIGNAL(triggered()), catalogTree, SLOT(catalogProperties()));
    
    cutAct = new QAction(QIcon(":icons/interface/editcut.png"), tr("Cu&t"), this);
    cutAct->setShortcut(tr("Ctrl+X"));
    cutAct->setStatusTip(tr("Cut the current selection's contents to the "
            "clipboard"));
//    connect(cutAct, SIGNAL(triggered()), textEdit, SLOT(cut()));

    copyAct = new QAction(QIcon(":icons/interface/editcopy.png"), tr("&Copy"), this);
    copyAct->setShortcut(tr("Ctrl+C"));
    copyAct->setStatusTip(tr("Copy the current selection's contents to the "
            "clipboard"));
//    connect(copyAct, SIGNAL(triggered()), textEdit, SLOT(copy()));

    pasteAct = new QAction(QIcon(":icons/interface/editpaste.png"), tr("&Paste"), this);
    pasteAct->setShortcut(tr("Ctrl+V"));
    pasteAct->setStatusTip(tr("Paste the clipboard's contents into the current "
            "selection"));
//     connect(pasteAct, SIGNAL(triggered()), textEdit, SLOT(paste()));
    
    // Search Actions
    searchAct = new QAction(QIcon(":icons/interface/find.png"), tr("&Search"), this);
    searchAct->setShortcut(tr("Ctrl+F"));
    searchAct->setStatusTip(tr("Find file in catalog."));
	connect(searchAct, SIGNAL(triggered()), this, SLOT(search()));
    
    
    //Import-Export Actions
    
    importAct = new QAction(tr("&Import DB"), this);
	importAct->setShortcut(tr("Ctrl+I"));
	importAct->setStatusTip(tr("Import other catalog(s)"));
	connect(importAct, SIGNAL(triggered()), this, SLOT(importExternalDB()));
    
    exportAct = new QAction(tr("Ex&port DB"), this);
    exportAct->setStatusTip(tr("Export database."));
    exportAct->setEnabled(false);
    
    //Tools Actions;
    settingsAct = new QAction(tr("Se&ttings"), this);
    settingsAct->setStatusTip(tr("Application settings"));
    settingsAct->setEnabled(false);
    
    repairDBAct = new QAction(tr("&Repair DB"), this);
    repairDBAct->setStatusTip(tr("Repairs DB"));
    repairDBAct->setEnabled(false);
    
    renumerateAct = new QAction(tr("Renumber disks"), this);
    renumerateAct->setStatusTip(tr("Renumber disk in the collection"));
    renumerateAct->setEnabled(false);
    
	manageIconsAct = new QAction(tr("Manage Icons"), this);
	manageIconsAct->setStatusTip(tr("Manage common icons."));
	connect(manageIconsAct, SIGNAL(triggered()), this, SLOT(manageIcons()));
	
	manageDrivesAct = new QAction(tr("Manage Drives"), this);
	manageDrivesAct->setStatusTip(tr("Manage drives."));
	connect(manageDrivesAct, SIGNAL(triggered()), this, SLOT(manageDrives()));
    
    //Help Actions
    
    helpAct = new QAction(tr("Help"), this);
    helpAct->setStatusTip(tr("Help"));
    helpAct->setEnabled(false);
    
    aboutAct = new QAction(tr("&About"), this);
    aboutAct->setStatusTip(tr("Show the application's About box"));
    connect(aboutAct, SIGNAL(triggered()), this, SLOT(about()));

    aboutQtAct = new QAction(tr("About &Qt"), this);
    aboutQtAct->setStatusTip(tr("Show the Qt library's About box"));
    connect(aboutQtAct, SIGNAL(triggered()), qApp, SLOT(aboutQt()));

    cutAct->setEnabled(false);
    copyAct->setEnabled(false);
    saveAsAct->setEnabled(false);
    pasteAct->setEnabled(false);
    //connect(textEdit, SIGNAL(copyAvailable(bool)), cutAct, SLOT(setEnabled(bool)));
    //connect(textEdit, SIGNAL(copyAvailable(bool)), copyAct, SLOT(setEnabled(bool)));
}

void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newCatalogAct);
    fileMenu->addAction(openAct);
    fileMenu->addAction(saveAct);
    fileMenu->addAction(saveAsAct);
    fileMenu->addSeparator();
    fileMenu->addAction(recentAct);
    fileMenu->addAction(closeCatalogAct);
    fileMenu->addAction(exitAct);
    
    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(addMediaAct);
    editMenu->addAction(addGroupAct);
    editMenu->addAction(deleteMediaAct);
    editMenu->addAction(updateMediaAct);
    editMenu->addAction(renameAct);
    editMenu->addAction(propertiesAct);
    editMenu->addSeparator();
    editMenu->addAction(cutAct);
    editMenu->addAction(copyAct);
    editMenu->addAction(pasteAct);
    
    searchMenu = menuBar()->addMenu(tr("&Search"));
    searchMenu->addAction(searchAct);
    
    importExportMenu = menuBar()->addMenu(tr("&Import/Export"));
    importExportMenu->addAction(importAct);
    importExportMenu->addAction(exportAct);
    
    toolsMenu = menuBar()->addMenu(tr("&Tools"));
    toolsMenu->addAction(repairDBAct);
	toolsMenu->addAction(renumerateAct);
	toolsMenu->addAction(manageIconsAct);
	toolsMenu->addAction(manageDrivesAct);
	toolsMenu->addSeparator();
    toolsMenu->addAction(settingsAct);
    
    menuBar()->addSeparator();
    
    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAct);
    helpMenu->addSeparator();
    helpMenu->addAction(aboutAct);
    helpMenu->addAction(aboutQtAct);
    
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("File"));
    fileToolBar->addAction(newCatalogAct);
    fileToolBar->addAction(openAct);
    fileToolBar->addAction(saveAct);
    
    editToolBar = addToolBar(tr("Edit"));
    editToolBar->addAction(cutAct);
    editToolBar->addAction(copyAct);
    editToolBar->addAction(pasteAct);
    
    searchToolBar = addToolBar(tr("Search"));
    searchToolBar->addAction(searchAct);
}

void MainWindow::createStatusBar()
{
    statusBar()->showMessage(tr("Ready"));
}


void MainWindow::writeSettings()
{
    settings->appSize = size();
    settings->appPos = pos();
    settings->saveSettings();
}

bool MainWindow::maybeSave()
{
/*    if (textEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr(APPNAME),
                                  tr("The document has been modified.\n"
                                    "Do you want to save the changes?"),
                                  QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else
            if (ret == QMessageBox::Cancel)
                return false;
    }*/
    return true;

}

bool MainWindow::saveFile(const QString &fileName)
{
    //QFile file(fileName);
/*    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr(APPNAME), 
                            tr("Can not write file %1.\n%2.").arg(fileName).arg(file.errorString()));
        return false;
        
    }
    QTextStream out(&file);
    QApplication::setOverrideCursor(Qt::WaitCursor);
    out << textEdit->toPlainText();
    QApplication::restoreOverrideCursor();
    
    setCurrentFile(fileName);
    statusBar()->showMessage(tr("File saved"), 2000);*/
    return true;
}

void MainWindow::setCurrentFile(const QString &fileName)
{
    curFile = fileName;
    //textEdit->document()->setModified(false);
    setWindowModified(false);
    
    QString showName;
    
    if (curFile.isEmpty())
        showName = "untitled";
    else
        showName = strippedName(curFile);
    
    setWindowTitle(tr("%1[*] - %2").arg(showName).arg(tr(APPNAME)));
                
}

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

QString MainWindow::strippedDir(const QString &fullFileName)
{
    return (QFileInfo(fullFileName).absoluteDir()).absolutePath();   
}

void MainWindow::manageDrives()
{
	AddScanPlaces dlg;
	dlg.setConfigManager(dm->settings);
	dlg.loadItems();
	dlg.exec();
}







