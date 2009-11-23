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
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QAction;
class QMenu;
class QTextEdit;
class QSplitter;
class QTableView;
class QTreeView;
class QTreeWidget;
class QFrame;
class ConfigManager;
class QTreeWidgetItem;
class catTreeItem;
class QSqlQueryModel;
class CatalogTree;
class CatalogTable;
class RightPanel;
class dbDVDCat;
class IconManager;
class dbNodeClass;
class CatalogClass;
class QCatDataModuleClass;

class MainWindow : public QMainWindow
{
    Q_OBJECT
            
    public:
        MainWindow();
        MainWindow(QStringList opts);
        ~MainWindow();

	protected:
        void construct(QStringList *opts = 0);
        
    protected:
        void closeEvent(QCloseEvent *event);
        
    private slots:
        void enableDebugMode();
        void newFile();
        void open();
        bool save();
        bool saveAs();
        bool closeCatalog();
        void about();
        void documentWasModified();
        void addMedia();
        void addGroup();
        void treeNewItemSelected(int newCatalog, int newId);
        void tableNewItemSelected(QString itemDesc);
		void manageIcons();
		void search();
		void importExternalDB();
		void manageDrives();        
    private:
        void createActions();
        void createMenus();
        void createToolBars();
        void createStatusBar();
        void readSettings(QStringList *opts);
        void writeSettings();
        bool maybeSave();
        void loadFile(QString &fileName);
        bool saveFile(const QString &fileName);
        void setCurrentFile(const QString &fileName);
        QString strippedName(const QString &fullFileName);
        QString strippedDir(const QString &fullFileName);
        
        QString curFile;
		
        // ConfigManager
		//QString dbConnName;
		QCatDataModuleClass *dm;
        ConfigManager *settings;
        /*
        dbDVDCat *dbdvd;
		IconManager *im;
		dbNodeClass *dbn;
		CatalogClass *cat;
        */
        // GUI Elements
        QFrame *centralFrame;
        CatalogTree *catalogTree;
        QSplitter *horiz;
        RightPanel *rightPanel;
        
        QMenu *fileMenu;
        QMenu *editMenu;
        QMenu *searchMenu;
        QMenu *importExportMenu;
        QMenu *toolsMenu;
        QMenu *helpMenu;
        
        QToolBar *fileToolBar;
        QToolBar *editToolBar;
        QToolBar *searchToolBar;
        QToolBar *importExportToolBar;
        QToolBar *toolsToolBar;
        // File Actions
        
        QAction *newCatalogAct;
        QAction *openAct;
        QAction *saveAct;
        QAction *saveAsAct;
        QAction *recentAct;
        QAction *closeCatalogAct;
        QAction *exitAct;
        
        //Edit Actions
        QAction *addMediaAct;
        QAction *addGroupAct;
        QAction *deleteMediaAct;
        QAction *updateMediaAct;
        QAction *renameAct;
        QAction *propertiesAct;
        QAction *cutAct;
        QAction *copyAct;
        QAction *pasteAct;
        
        //Search Actions
        QAction *searchAct;
        QAction *searchDupAct;
        
        //Import/Export Actions
        QAction *importAct;
        QAction *exportAct;
        
        //Tools Actions
        QAction *settingsAct;
        QAction *repairDBAct;
        QAction *renumerateAct;
		QAction *manageIconsAct;
		QAction *manageDrivesAct;
        
        // Help Actions
        QAction *helpAct;
        QAction *aboutAct;
        QAction *aboutQtAct;


        QAction *enableDebugAct;
		bool debugEnabled;
		QStringList debugParams;
	public:
		void setDebugParams(QStringList params);

};
#endif

