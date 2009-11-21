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
#ifndef WHEREISITXMLIMPORTCLASS_H
#define WHEREISITXMLIMPORTCLASS_H

#include <QTreeWidgetItem>
#include <QDate>
#include <QTime>
#include <QXmlDefaultHandler>
#include <QMap>

/**
	@author Victor Nike <victor.nike@gmail.com>
*/

class QXmlParseException;
class QXmlAttributes;
class QCatDataModuleClass;
struct dbNode;
class ProgressWidget;

struct xmlItem {
    int				itemType;
    QString 		name;
    QString 		ext;
    qint64 			size;
    QString			date;
    QString			time;
    QString			diskName;
    QString			diskType;
    QString			path;
    QString			category;
    QString			description;
    QString			diskNum;
    int				crc;
    QString			flag;
    QString			diskLocation;
};

class ImportTreeItem : public QTreeWidgetItem {
    public:
        //WhereIsItXmlItem() {};
        ImportTreeItem(ImportTreeItem *parent = 0);
        ~ImportTreeItem();
        
        int id;
        int itemType;
        QString itemName;
        qint64 itemSize;
        uint itemDateTime;
        int xmlDiskNo;
        int dbDiskNo;
        QString itemDescription;
        int itemFlags;
        int itemAttr;
};

class Work_ImportWhereIsItItem {

    public:
        Work_ImportWhereIsItItem(QCatDataModuleClass *_dm, int _catId);
        ~Work_ImportWhereIsItItem();
        
        void setMode(int _mode) { parseMode = _mode; }
        void setProgressObject(ProgressWidget *_pw, int progressNo);
        bool processItem(xmlItem &xmlData);
		
		void saveDisks();
		void saveAllFolders();
        
    private:
		
		void saveFolders(int diskNo, ImportTreeItem *root);
        int parseMode;
        int progressBarNo;
        int catId;
		QCatDataModuleClass *dm;
        ProgressWidget *pw;
        
        QMap<QString, ImportTreeItem*> disksMap;
};

class WhereIsItXmlImportClass : public QXmlDefaultHandler{
    public:
        WhereIsItXmlImportClass();
    
        ~WhereIsItXmlImportClass();
        bool startElement(const QString &namespaceURI,
                          const QString &localName,
                          const QString &qName,
                          const QXmlAttributes &attributes);
        bool endElement(const QString &namespaceURI,
                        const QString &localName,
                        const QString &qName);
        bool characters(const QString &str);
        bool fatalError(const QXmlParseException &exception);
    public:
        void setJustCountElements(bool flag) { justCount = flag ; }
        
        /**
         * This method sets class responsible for dealing with paresed data.
         * 
         * */
        void setWorkClass(Work_ImportWhereIsItItem *_wc);

		bool isParseSuccessful() const { return !hasParserError; }
		QString getParseError() const { return errorFromParser; }
        
        QString currentText;
        
    // Must be private
    public:
        int _diskGroupsNum;
        int _disks;
        int _folders;
        int _files;
    private:
        void initGivenData(xmlItem &data);
        Work_ImportWhereIsItItem *wc; // What to do with parsed ItemS
        bool justCount;
        
        xmlItem _data;
		bool hasParserError;
		QString errorFromParser;
};

#endif
