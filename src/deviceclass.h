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
#ifndef DEVICECLASS_H
#define DEVICECLASS_H

/**
	@author Victor Nike <victor.nike@gmail.com>
*/

#include <QString>

class QFile;

class DeviceClass{
    public:
        DeviceClass();
    
        ~DeviceClass();
        
        bool checkOpticalDevice(const QString &deviceFile);
        bool checkIfMounted(const QString &path);
        bool mountDev(const QString &path);
        bool unMountDev(const QString &path);
        bool getInfoForOptical(const QString &dev);
		QString getOptiocalMountMount(const QString &dev);
		
        bool iso9660;
        bool ElTorito;
        bool Joliet;
		bool udf;
        QString vdIso9660;
        QString vdJoliet;
		QString vdUDF;

    private:
		
		int udfMaxDesc;
		int udfFirstSect;
        int sectToOffset(int _sectNo);
        int checkForISO9660(const QString &dev);
        int checkForUDF(const QString &dev);
		bool probeUDF(QFile &dev, int sec_size);
        
        
    

};

#endif
