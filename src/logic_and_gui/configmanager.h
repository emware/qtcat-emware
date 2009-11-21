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
#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#define APPNAME					"QCat"
//#define APPNAME					"Media Catalog"
#define APPCOMPANY				"Armagedon Inc."
#define SCR_POS					"ScreenPos"
#define SCR_SIZE				"ScreenSize"
#define LAST_FILE_USED			"LastFileUsed"
#define LAST_DIR_USED			"LastDirUsed"
#define AUTO_OPEN_LAST_FILE		"AutoOpenLastFile"
#define COLS_CATALOG			"CatalogAndGroupsColumns"
#define COLS_DISK				"DiskAndFoldersColumns"
#define SCAN_PLACES				"ScanPlaces"
#define DONT_CLOSE_AFTER_SCAN	"DontCloseAfterScan"
#define SEARCH_CS				"SearchIsCaseSensitive"
#define SEARCH_FN				"SearchInFileNames"
#define SEARCH_DI				"SearchInDirNames"
#define SEARCH_DE				"SearchInDescriptions"

#include <QList>
#include <QPoint>
#include <QSize>

/**
	@author Victor Nike <victor.nike@gmail.com>
*/

class QSettings;

struct structScanPlaces;

class ConfigManager{

public:
    ConfigManager();

    ~ConfigManager();
    
    void saveSettings();
    void loadSettings();
    bool isCatalogColumnVisible(int columnNo);
    bool isDiskColumnVisible(int columnNo);
	void testCM();

public: 
    // App Size and Pos
    QPoint appPos;
    QSize appSize;
    
    // Last Four files user
    QString lastFile;
    QString lastDir;
    QString file1, file2, file3, file4;
    bool autoOpenLastFile;
    bool dontCloseAfterScan;
    bool searchFileNames;
    bool searchDirNames;
    bool searchDescriptions;
    bool searchCaseSensitive;
    
    //Columsns Settigns
    int catalogsColumns;
    int disksColumns;
    int catColumnSizes[8];
    int diskColumnSizes[11];
	int numPlaces;
	QList<structScanPlaces> scanPlaces;
    
    // DB connection
    /**
     * dbType
     *      1 - SQLite
     *      2 - MySQL
     *      3 - PostgreSQL
     *      4 - ODBC
     **/
    int dbType; 
    QString userName;
    QString password;
    QString dbHost;
    QString dbName;
    
    
            
private:
    QSettings *allSettings;        
};

#endif
