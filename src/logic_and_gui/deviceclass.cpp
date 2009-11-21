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
#include <QFile>
#include <QStringList>
#include <QProcess>
#include <QTextCodec>
#include <QTextDecoder>

#include <cstring>

#include "deviceclass.h"
#include "defsandtools.h"

DeviceClass::DeviceClass()
{
    ElTorito = false;
    iso9660 = false;
    Joliet = false;
	udf = false;
	udfFirstSect = 0;
	udfMaxDesc = 0;
}


DeviceClass::~DeviceClass()
{
    
}

bool DeviceClass::checkOpticalDevice(const QString & deviceFile)
{
    if (deviceFile.startsWith("/dev/"))
        return true;
    else
        return false;
}


bool DeviceClass::unMountDev(const QString & path)
{
    QProcess umount;
    umount.start( "umount", QStringList() << path );
    umount.waitForFinished();
    if (umount.exitCode())
        return false;
    else
        return true;

}

bool DeviceClass::mountDev(const QString & path)
{
    QProcess mount;
    mount.start( "mount", QStringList() << path );
    mount.waitForFinished();
    if (mount.exitCode())
        return false;
    else
        return true;
}

bool DeviceClass::checkIfMounted(const QString & path)
{
    
    QFile mtab("/etc/mtab");
    bool checkForDevice = false;
    if (path.startsWith("/dev/"))
        checkForDevice = true;
    if (checkForDevice)
    {
        if ( mtab.open(QIODevice::ReadOnly) ) 
        {
            QStringList mtabCont;
            mtabCont << QString( mtab.readAll() ).split("\n");
    
            QStringListIterator i(mtabCont);
            while (i.hasNext())
                if (i.next().startsWith(path)) return true;
        }
    }else{
        if ( mtab.open(QIODevice::ReadOnly) ) 
        {
            QStringList mtabCont;
            mtabCont << QString( mtab.readAll() ).split("\n");
    
            QStringListIterator i(mtabCont);
            
            while (i.hasNext())
            {
                QStringList mTabItem;
                QString _str = i.next();
                mTabItem << _str.split(" ");
                if (mTabItem.count() > 1)
                	if ((mTabItem[1] != "/") && (path.startsWith(mTabItem[1])))
	                	return true;
            }
        
    	}
    }
    return false;
}


QString DeviceClass::getOptiocalMountMount(const QString & dev)
{
	QFile fstab("/etc/fstab");
	if (!dev.startsWith("/dev/"))
		return dev;
	if ( fstab.open(QIODevice::ReadOnly) ) 
	{
		QStringList fstabCont;
		fstabCont << QString( fstab.readAll() ).split("\n");

		QStringListIterator i(fstabCont);
		
		while (i.hasNext())
		{
			QString _str = i.next();
			if (_str.startsWith(dev))
			{
				QStringList fsTabItem;
				_str = _str.simplified();
				fsTabItem << _str.split(" ");
				if (fsTabItem.count() > 1)
				{
					QString _ss = fsTabItem.at(1);
					return _ss;
				}
			}
		}
	}
	return "";
}


int DeviceClass::checkForISO9660(const QString & dev)
{
    QFile fd(dev);
    bool endReached = false;
    if (fd.open(QIODevice::ReadOnly) && fd.seek(sectToOffset(16))) {
        for(int i = 16; i < 33; i++){
            fd.seek(sectToOffset(i));
            QByteArray cdIdentifier = fd.read(6);
            unsigned int cdType = cdIdentifier.at(0);
            QString cdStr(cdIdentifier.right(5));
            
            QTextCodec *codec = QTextCodec::codecForName("UTF-16BE");
            QTextDecoder utf16Dec(codec);
            
            if (cdStr != "CD001")
                continue;
            switch (cdType)
            {
                case 0:
                    ElTorito = true;
                    break;
                case 1:
                    iso9660 = true;
                    fd.seek(sectToOffset(i) + 40);
                    vdIso9660 = QString(fd.read(32));
                    break;
                case 2:
                    Joliet = true;
                    fd.seek(sectToOffset(i) + 40);
                    vdJoliet = utf16Dec.toUnicode(fd.read(32));
                    /*
                    _jd = fd.read(32).data();
                    vdJoliet = QString::fromUtf16(_jd);
                    */
                    break;
                case 0xff:
                    endReached = true;
            }
            if (endReached)
                break;
        }
        
    }

}

int DeviceClass::checkForUDF(const QString & dev)
{
	unsigned char sig_bea[7]  = {0x00, 0x42, 0x45, 0x41, 0x30, 0x31, 0x01};
	/*   B     E     A     0     1  */
	unsigned char sig_nsr2[7] = {0x00, 0x4e, 0x53, 0x52, 0x30, 0x32, 0x01};
	/*   N     S     R     0     2  */
	unsigned char sig_nsr3[7] = {0x00, 0x4e, 0x53, 0x52, 0x30, 0x33, 0x01};
	/*   N     S     R     0     3  */
	unsigned char sig_tea[7]  = {0x00, 0x54, 0x45, 0x41, 0x30, 0x31, 0x01};
	/*   T     E     A     0     1  */
	int sizes[] = { 2048, 512, 1024, 4096, -1 };	
	
	QFile fd(dev);
	bool endReached = false;
	bool sig1 = false;
	bool sig2 = false;
	bool sig3 = false;
	bool sig4 = false;
	int udfSecsize = 0;
	if (fd.open(QIODevice::ReadOnly) && fd.seek(sectToOffset(16))) {
		for(int i = 16; i < 33; i++){
			fd.seek(sectToOffset(i));
			QByteArray udfIdentifier = fd.read(7);
			if (memcmp(udfIdentifier.constData(), sig_bea, 7) == 0)
				sig1 = true;
			if (memcmp(udfIdentifier.constData(), sig_nsr2, 7) == 0)
				sig2 = true;
			if (memcmp(udfIdentifier.constData(), sig_nsr3, 7) == 0)
				sig3 = true;
			if (memcmp(udfIdentifier.constData(), sig_tea, 7) == 0)
				sig4 = true;
		}
		if (sig1 && sig4 && (sig2 || sig3))
			udf = true;
		
		for (int i = 0; sizes[i] > 0; i++)
		{
			if (probeUDF(fd, sizes[i]))
			{	
				udfSecsize = sizes[i];
				break;	
			}
		}
		QTextCodec *codec = QTextCodec::codecForName("UTF-16BE");
		QTextDecoder utf16Dec(codec);

		//printQS("UDF Sec Size is = " + QString::number(udfSecsize));
		for(int i = udfFirstSect; i < udfFirstSect + udfMaxDesc; i++)
		{
			fd.seek(i * udfSecsize);
			QByteArray udfDescr = fd.read(udfSecsize);
			int descrType = udfDescr.at(0);
			QString vl;
			if (descrType == 1)
			{
				if (udfDescr.at(24) == 8)
					vdUDF = QString(udfDescr.mid(25, 31));
				else
					vdUDF = utf16Dec.toUnicode(udfDescr.mid(25, 31));
				break;
			}
			
		}
		
		
	}
	

}

int DeviceClass::sectToOffset(int _sectNo)
{
    return _sectNo * 0x800;
}

bool DeviceClass::getInfoForOptical(const QString & dev)
{
    checkForISO9660(dev);
	checkForUDF(dev);
}

bool DeviceClass::probeUDF(QFile & dev, int sec_size)
{
	unsigned char buf[16];
	int cnt;
	int addr;
	dev.seek(256*sec_size);
	dev.read((char *)&buf, 16);
	int chkSum = 0;
	for (int i = 0; i < 16; i++)
	{
		if (i != 4)
			chkSum += buf[i];
	}
	if ((chkSum & 0xFF) != buf[4])
		return false;
	if (buf[5] != 0)
		return false;
	
	dev.read((char *)&cnt, 4);
	dev.read((char *)&addr, 4);
	
	udfMaxDesc = cnt;
	udfFirstSect = addr;
	
	
	
	return true;
	
}


