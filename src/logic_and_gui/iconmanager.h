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
#ifndef ICONMANAGER_H
#define ICONMANAGER_H

/**
	@author Victor Nike <victor.nike@gmail.com>
*/

#include <QSqlDatabase>
#include <QMap>

class QString;
class QIcon;
class QPixmap;

class IconManager{
	public:
		IconManager();
		IconManager(const QSqlDatabase &db);
		//IconManager(const QString &connName);
		
		~IconManager();
		
		void addIconFromFile(int catId, const QString &iconName, const QString &fileName);
        void addIconFromRes(int catId, const QString &iconName, const QString &resName);
        void updateIconFromFile(int catId, int iconId, const QString &iconName, const QString &fileName);
        void updateIconName(int catId, int iconId, const QString &iconName);
        void getIconAsRef(int iconId, QIcon &icon);
		//QIcon* getIconAsPtr(int iconId); 
        
		QPixmap* getPixmap(int catId, int iconId);
        QPixmap* getPixmap(int catId, const QString &iconName);
        QPixmap* getDefaultPixmap(int itemType, int diskFullType);
        int getIconId(int catId, const QString &iconName);
        void deleteAllIcons(int catId);
        void deleteIcon(int catId, int iconId);
        void saveIcon(int catId, int iconId, const QString &fileName);
        void fillDefaultIcons();
        
		
	private:
        void readGlobalIcons();
		//QString dbConnName;
		QSqlDatabase db;
        QMap <QString, QPixmap*> globalIcons;
	
};

#endif
