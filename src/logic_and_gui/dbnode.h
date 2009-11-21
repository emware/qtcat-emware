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


#ifndef DBNODE_H
#define DBNODE_H

/**
	@author Victor Nike <victor.nike@gmail.com>
 */

#include <QList>
#include <QSqlDatabase>

class QString;
class QSqlQuery;
struct dbNode;

class dbNodeClass{
    public:
		dbNodeClass(const QSqlDatabase &db);
        
        ~dbNodeClass();
        
        //Members
        void initGivenData(dbNode &data);
        //int isExistRootElement(int catId);
        int isExistGroup(int catId, int parent, const QString &grpName);
        int isExistDisk(int catId, int parent, const QString &diskName);

        int createNode(dbNode &data);
        int createGroup(dbNode &data);
        int createDisk(dbNode &data);
    
        int getNodeType(int catId, int _id, bool filesTable);
        int getNodeId(int catId, int parent, const QString &name);
		int getNextDiskNo(int catId);
		bool setNextDiskNo(int catId, int diskNo);
        bool readNode(int catId, int id, dbNode &data);
        bool readFile(int catId, int id, dbNode &data);
        QList<dbNode *> *readAllChildsForParent(int catId, int parent);
		void readAllChildsForParent(int catId, int parent, QList<dbNode *> &data);
		void clearNodeList(QList<dbNode *> &pList);
        
        bool updateNode(const dbNode &data);
        bool updateGroup(const dbNode &data);
        bool updateDisk(const dbNode &data);
        
        // Calculate node - Disk or Group Size
        bool calcNodeSize(dbNode &data);
        
        bool deleteNode(const dbNode &data);
        bool deleteDisk(const dbNode &data);
        int deleteGroup(const dbNode &data, bool deleteChildren);
        int deleteRootElement(const dbNode &data, bool deleteChildren);
        
        bool renameNode(int _catId, int _id, const QString &name);
        bool renameFile(int _catId, int _id, const QString &name);
        
		
		int getSuitableParentForScan(int catId, int id);
        
        // Update funcions
        bool updateNodeDescription(int _catId, int _id, const QString &newDescription);
        bool updateNodeIcon(int _catId, int _id, int _iconId);
        bool updateNodeIconAndDescription(int _catId, int _id, int _iconId, const QString &newDescription);
        bool updateNodeParent(int _catId, int _id, int _newParent);
        
                // Update funcions
        bool updateFileDescription(int _catId, int _id, const QString &newDescription);
        bool updateFileIcon(int _catId, int _id, int _iconId);
        bool updateFileIconAndDescription(int _catId, int _id, int _iconId, const QString &newDescription);
        bool updateFileParent(int _catId, int _id, int _newParent);

        QSqlDatabase db;
    private:
        void makeDeleteSql(const dbNode &data, QString &sql);
		void prepareQueryForInsert(QSqlQuery *q, const dbNode &data);
		void prepareQueryForUpdate(QSqlQuery *q, const dbNode &data);
        
        bool runSql(const QString &sql);
        
        bool checkRelationship(int parentType, int childType);
        
        bool execQuery(const QString &sql);
        
        QString dbConnName;
};


#endif
