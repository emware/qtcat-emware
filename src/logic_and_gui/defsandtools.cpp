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

// System includes
//#include <unistd.h>
//#include <sys/types.h>
//#include <sys/stat.h>

#include <iostream>
#include <cstdlib>

#include <QString>

#include "defsandtools.h"

void printQS(const QString toPrn)
{
    std::cerr << toPrn.toStdString() << std::endl;
}

QString getMediaTypeIconName(int index)
{
 	switch (index)
    {
        case 0: return ":icons/media/icon_cdcat_m_cd_icon.png";
        break;
        case 1: return ":icons/media/icon_cdcat_m_dvd_icon.png";
        break;
        case 2: return ":icons/media/icon_cdcat_m_hdd_icon.png";
        break;
        case 3: return ":icons/media/icon_cdcat_m_floppy_icon.png";
        break;
        case 4: return ":icons/media/icon_cdcat_m_net_icon.png";
        break;
        case 5: return ":icons/media/icon_cdcat_m_flash_icon.png";
        break;
        case 6: return ":icons/media/icon_cdcat_m_other_icon.png";
    }
    
}

int getBasicDiskType(int dbFlags)
{
	return (dbFlags & 0x07);	
}

int getBasicDiskTypeFromIndex(int index)
{
	if (between(index, 0, MEDIA_TYPES_COUNT - 1))
	{
		return mediaTypesArray[index].basicType;
	}else{
		return -1;
	}
	
}

int getFullDiskTypeFromIndex(int index)
{
	if (between(index, 0, MEDIA_TYPES_COUNT - 1))
	{
		return mediaTypesArray[index].fullType;
	}else{
		return -1;
	}
	
}

int setBasicDiskType(int newType, int dbFlags)
{
	int tempFlags = dbFlags & 0xFFFFFFF0; // Clear last 4 bits
	tempFlags = tempFlags + newType;
	
/*	
	switch (newType)
	{
		case dbNode::tCD_ROM:
			tempFlags = tempFlags | MT_CDROM;
			break;
		case dbNode::tDVD_ROM:
			tempFlags = tempFlags | MT_DVDROM;
			break;
		case dbNode::tHDD:
			tempFlags = tempFlags | MT_HDD;
			break;
		case dbNode::tNETDrive:
			tempFlags = tempFlags | MT_NET;
			break;
		case dbNode::tUSBFlash:
			tempFlags = tempFlags | MT_USB;
			break;
		case dbNode::tOTHER:
			tempFlags = tempFlags | MT_OTHER;
			break;
	}
*/	
	return tempFlags;
	
}

inline bool between(int value, int min, int max)
{
	return ((value <= max ) && (value >= min)) ? true : false;
}

int findIndexFromFullDiskType(int fullDiskType)
{
	int result = -1;
	for (int i = 0; i < MEDIA_TYPES_COUNT; i++)
	{
		if (mediaTypesArray[i].fullType == fullDiskType)
		{
			result = i;
			break;
		}
	}
	return result;
}

const QString permsToQString(int m)
{
    // Sample dlh*rw-rw-rwx
 	char perm[14] = "---*---------";
    
//    if ( S_ISLNK( m ) ) perm[ 0 ] = 'l';  // check for symLink
//    if ( S_ISDIR( m ) ) perm[ 0 ] = 'd';  // check for directory

	//ReadUser = 0400, WriteUser = 0200, ExeUser = 0100, Suid = 04000
    if ( m & 0x4000 ) perm[ 4 ] = 'r';
    if ( m & 0x2000 ) perm[ 5 ] = 'w';
    if ( m & 0x1000 ) perm[ 6 ] = 'x';
	//ReadGroup = 0040, WriteGroup = 0020, ExeGroup = 0010, Gid = 02000
    if ( m & 0x0040 ) perm[ 7 ] = 'r';
    if ( m & 0x0020 ) perm[ 8 ] = 'w';
    if ( m & 0x0010 ) perm[ 9 ] = 'x';
	//ReadOther = 0004, WriteOther = 0002, ExeOther = 0001, Sticky = 01000
    if ( m & 0x0004 ) perm[ 10 ] = 'r';
    if ( m & 0x0002 ) perm[ 11 ] = 'w';
    if ( m & 0x0001 ) perm[ 12 ] = 'x';
    // My Custom values = isDir = 0x0400, isLink = 0x0200, isHidden = 0x0100
    if ( m & 0x0400 ) perm[ 0 ] = 'd';
    if ( m & 0x0200 ) perm[ 1 ] = 'l';
    if ( m & 0x0100 ) perm[ 2 ] = 'h';
    
    return QString( perm );
}

int getFileType(const QString &ext)
{
    if (ext.toUpper() == "TXT")
        return 1;    
    if (ext.toUpper() == "SRT")
        return 1;    
    if (ext.toUpper() == "SUB")
        return 1;    
    if (ext.toUpper() == "AVI")
        return 2;    
    if (ext.toUpper() == "RM")
        return 2;    
    if (ext.toUpper() == "MOV")
        return 2;    
    if (ext.toUpper() == "MPG")
        return 2;    
    if (ext.toUpper() == "MPEG")
        return 2;    
    if (ext.toUpper() == "WMV")
        return 2;    
    if (ext.toUpper() == "MKV")
        return 2;    
    if (ext.toUpper() == "FLV")
        return 2;    
    if (ext.toUpper() == "MP4")
        return 2;    
    if (ext.toUpper() == "ASF")
        return 2;    
    if (ext.toUpper() == "GIF")
        return 3;    
    if (ext.toUpper() == "JP2")
        return 3;    
    if (ext.toUpper() == "JPG")
        return 3;    
    if (ext.toUpper() == "JPEG")
        return 3;    
    if (ext.toUpper() == "PNG")
        return 3;    
    if (ext.toUpper() == "BMP")
        return 3;    
    if (ext.toUpper() == "TIF")
        return 3;    
    if (ext.toUpper() == "TIFF")
        return 3;    
    if (ext.toUpper() == "XMP")
        return 3;    
    if (ext.toUpper() == "SVG")
        return 3;    
    if (ext.toUpper() == "PSD")
        return 3;    
    if (ext.toUpper() == "WB1")
        return 3;    
    return 0;
}

#define AE_NUM 3
QString atomicExtensions[] = {
  	".tar.bz2", ".tar.gz", ".moc.cpp"
};



void parseFileName(const QString &fullFileName, vFileName &data)
{
    data.isHidden = false;
    data.hasExt = false;
    data.isExtAtomic = false;
    
    int loc = fullFileName.lastIndexOf("."); 
    if (loc > 0)
    {
        for (int i = 0; i < AE_NUM; i++)
        {
            if (fullFileName.endsWith(atomicExtensions[i]))
            {
                loc = fullFileName.length() - atomicExtensions[i].length();
                data.isExtAtomic = true;
                break;
            }
        }
        data.fileName = fullFileName.left(loc);
        data.fileExt = fullFileName.mid(loc + 1);
        data.hasExt = true;
    }else{
        data.fileName = fullFileName;   
    }
    if (fullFileName.startsWith("."))
        data.isHidden = true;
    int tt = getFileType(data.fileExt);
    switch (tt)
    {
        case 1:
            data.defIcon = TEXT_FILE_ICON;
            break;
        case 2:
            data.defIcon = MOVIE_FILE_ICON;
            break;
        case 3:
            data.defIcon = IMAGE_FILE_ICON;
            break;
        default:
            data.defIcon = BINARY_FILE_ICON;
    }
}

int getDiskFullTypeFromWhereIsIt(const QString & _dt)
{
	// Return CD-ROM values
    if (_dt == "CD-ROM")
		return mediaTypesArray[1].fullType;
    if (_dt == "CD-R")
		return mediaTypesArray[2].fullType;
	if (_dt == "CD-RW")
		return mediaTypesArray[3].fullType;
	if (_dt == "DD CD-R")
		return mediaTypesArray[2].fullType;
	if (_dt == "DD CD-ROM")
		return mediaTypesArray[1].fullType;
	if (_dt == "DD CD-RW")
		return mediaTypesArray[3].fullType;

	// Return DVD
	if (_dt == "DVD-ROM")
		return mediaTypesArray[6].fullType;
	if (_dt == "DVD+R")
		return mediaTypesArray[8].fullType;
	if (_dt == "DVD+R DL")
		return mediaTypesArray[13].fullType;
	if (_dt == "DVD+RW")
		return mediaTypesArray[10].fullType;
	if (_dt == "DVD+RW DL")
		return mediaTypesArray[15].fullType;
	if (_dt == "DVD-R")
		return mediaTypesArray[7].fullType;
	if (_dt == "DVD-RAM")
		return mediaTypesArray[11].fullType;
	if (_dt == "DVD-RW")
		return mediaTypesArray[9].fullType;
	if (_dt == "DVD-RW DL")
		return mediaTypesArray[14].fullType;
	if (_dt == "HD DVD-R")
		return mediaTypesArray[17].fullType;
	if (_dt == "HD DVD-RAM")
		return mediaTypesArray[18].fullType;
	if (_dt == "HD DVD-ROM")
		return mediaTypesArray[16].fullType;
	if (_dt == "BD-R")
		return mediaTypesArray[20].fullType;
	if (_dt == "BD-RE")
		return mediaTypesArray[21].fullType;
	if (_dt == "BD-ROM")
		return mediaTypesArray[19].fullType;
	// Misc 
	if (_dt == "Hard disk")
		return mediaTypesArray[22].fullType;
	if (_dt == "Network drive")
		return mediaTypesArray[23].fullType;
	
	return mediaTypesArray[0].fullType;

	
}

/*
int setDiskSubType(int diskType, int diskSubType, int dbAttributes)
{
	int tempAttr = dbAttributes & 0xFFFF0000;
	
	switch (diskType)
	{
		case dbNode::tCD_ROM:
			switch (diskSubType)
			{
				case dbNode::CD_ROM:
					tempAttr = tempAttr | CD_CDROM;	
					break;
				case dbNode::CD_R:
					tempAttr = tempAttr | CD_CDR;	
					break;
				case dbNode::CD_RW:
					tempAttr = tempAttr | CD_CDRW;	
					break;
			}
			break;
		case dbNode::tDVD_ROM:
			switch (diskSubType)
			{
				case dbNode::DVD_ROM:
					tempAttr = tempAttr | DVD_DVDROM;	
					break;
				case dbNode::DVD_MINUS_R:
					tempAttr = tempAttr | DVD_DVD_MINUS_R;	
					break;
				case dbNode::DVD_PLUS_R:
					tempAttr = tempAttr | DVD_DVD_PLUS_R;	
					break;
				case dbNode::DVD_MINUS_RW:
					tempAttr = tempAttr | DVD_DVD_MINUS_RW;	
					break;
				case dbNode::DVD_PLUS_RW:
					tempAttr = tempAttr | DVD_DVD_PLUS_RW;	
					break;
				case dbNode::DVD_RAM:
					tempAttr = tempAttr | DVD_DVD_RAM;	
					break;
				case dbNode::DVD_MINUS_R_DL:
					tempAttr = tempAttr | DVD_DVD_MINUS_R_DL;	
					break;
				case dbNode::DVD_PLUS_R_DL:
					tempAttr = tempAttr | DVD_DVD_PLUS_R_DL;	
					break;
				case dbNode::DVD_MINUS_RW_DL:
					tempAttr = tempAttr | DVD_DVD_MINUS_RW_DL;	
					break;
				case dbNode::DVD_PLUS_RW_DL:
					tempAttr = tempAttr | DVD_DVD_PLUS_RW_DL;	
					break;
				case dbNode::HD_DVD_ROM:
					tempAttr = tempAttr | DVD_HD_DVD_ROM;	
					break;
				case dbNode::HD_DVD_R:
					tempAttr = tempAttr | DVD_HD_DVD_R;	
					break;
				case dbNode::HD_DVD_RAM:
					tempAttr = tempAttr | DVD_HD_DVD_RAM;	
					break;
				case dbNode::BD_ROM:
					tempAttr = tempAttr | DVD_BD_ROM;	
					break;
				case dbNode::BD_R:
					tempAttr = tempAttr | DVD_BD_R;	
					break;
				case dbNode::BD_RE:
					tempAttr = tempAttr | DVD_BD_RE;	
					break;
			}
			break;
		case dbNode::tUSBFlash:
			switch (diskSubType)
			{
				case dbNode::DVD_ROM:
					tempAttr = tempAttr | DVD_DVDROM;	
					break;
				case dbNode::DVD_MINUS_R:
					tempAttr = tempAttr | DVD_DVD_MINUS_R;	
					break;
				case dbNode::DVD_PLUS_R:
					tempAttr = tempAttr | DVD_DVD_PLUS_R;	
					break;
				case dbNode::DVD_MINUS_RW:
					tempAttr = tempAttr | DVD_DVD_MINUS_RW;	
					break;
				case dbNode::DVD_PLUS_RW:
					tempAttr = tempAttr | DVD_DVD_PLUS_RW;	
					break;
				case dbNode::DVD_RAM:
					tempAttr = tempAttr | DVD_DVD_RAM;	
					break;
				case dbNode::DVD_MINUS_R_DL:
					tempAttr = tempAttr | DVD_DVD_MINUS_R_DL;	
					break;
				case dbNode::DVD_PLUS_R_DL:
					tempAttr = tempAttr | DVD_DVD_PLUS_R_DL;	
					break;
				case dbNode::DVD_MINUS_RW_DL:
					tempAttr = tempAttr | DVD_DVD_MINUS_RW_DL;	
					break;
				case dbNode::DVD_PLUS_RW_DL:
					tempAttr = tempAttr | DVD_DVD_PLUS_RW_DL;	
					break;
				case dbNode::HD_DVD_ROM:
					tempAttr = tempAttr | DVD_HD_DVD_ROM;	
					break;
				case dbNode::HD_DVD_R:
					tempAttr = tempAttr | DVD_HD_DVD_R;	
					break;
				case dbNode::HD_DVD_RAM:
					tempAttr = tempAttr | DVD_HD_DVD_RAM;	
					break;
				case dbNode::BD_ROM:
					tempAttr = tempAttr | DVD_BD_ROM;	
					break;
				case dbNode::BD_R:
					tempAttr = tempAttr | DVD_BD_R;	
					break;
				case dbNode::BD_RE:
					tempAttr = tempAttr | DVD_BD_RE;	
					break;
			}
			break;
		
		default:
			tempAttr = tempAttr | OTHER_TYPE;	
			break;
			
	}
	return tempAttr;
}

int getDiskSubType(int diskType, int dbAttributes)
{
	switch (diskType) 
	{
		case dbNode::tCD_ROM:
			if (dbAttributes & CD_CDROM)
				return dbNode::CD_ROM;
			if (dbAttributes & CD_CDR)
				return dbNode::CD_R;
			if (dbAttributes & CD_CDRW)
				return dbNode::CD_RW;
			return dbNode::OTHER;
			break;
		case dbNode::tDVD_ROM:
			if (dbAttributes & DVD_DVDROM)
				return dbNode::DVD_ROM;
			if (dbAttributes & DVD_DVD_MINUS_R)
				return dbNode::DVD_MINUS_R;
			if (dbAttributes & DVD_DVD_PLUS_R)
				return dbNode::DVD_PLUS_R;
			if (dbAttributes & DVD_DVD_MINUS_RW)
				return dbNode::DVD_MINUS_RW;
			if (dbAttributes & DVD_DVD_PLUS_RW)
				return dbNode::DVD_PLUS_RW;
			if (dbAttributes & DVD_DVD_RAM)
				return dbNode::DVD_RAM;
			if (dbAttributes & DVD_DVD_MINUS_R_DL)
				return dbNode::DVD_MINUS_R_DL;
			if (dbAttributes & DVD_DVD_PLUS_R_DL)
				return dbNode::DVD_PLUS_R_DL;
			if (dbAttributes & DVD_DVD_MINUS_RW_DL)
				return dbNode::DVD_MINUS_RW_DL;
			if (dbAttributes & DVD_DVD_PLUS_RW_DL)
				return dbNode::DVD_PLUS_RW_DL;
			if (dbAttributes & DVD_HD_DVD_ROM)
				return dbNode::HD_DVD_ROM;
			if (dbAttributes & DVD_HD_DVD_R)
				return dbNode::HD_DVD_R;
			if (dbAttributes & DVD_HD_DVD_RAM)
				return dbNode::HD_DVD_RAM;
			if (dbAttributes & DVD_BD_ROM)
				return dbNode::BD_ROM;
			if (dbAttributes & DVD_BD_R)
				return dbNode::BD_R;
			if (dbAttributes & DVD_BD_RE)
				return dbNode::BD_RE;
			return dbNode::OTHER;
			break;
		default:
			return dbNode::OTHER;
			break;
			
	}
}

bool getDiskTypeAndSubTypeFromIndex(int index, int *diskType, int *diskSubType)
{
	int result = false;
	int dt = 0;
	int dst = 0;
	//int ind = index;
	if (between(index, 0, 2))
	{
		dt = MT_CDROM;
		result = true;
	}
	if (between(index, 3, 18))
	{
		dt = MT_DVDROM;
		result = true;
	}
	if (index == 19)
	{
		dt = MT_HDD;
		result = true;
	}
	if (index == 20)
	{
		dt = MT_NET;
		result = true;
	}
	if (between(index, 21, 35))
	{
		dt = MT_USB;
		result = true;
	}
	if (index == 36)
	{
		dt = MT_OTHER;
		result = true;
	}
	switch (dt){
		case MT_CDROM:
			dst = index;
			break;
		case MT_DVDROM:
			index -=3;
			dst = index;
			break;
		case MT_HDD:
			dst = dbNode::OTHER;
			break;
		case MT_NET:
			dst = dbNode::OTHER;
			break;
		case MT_USB:
			index -= 21;
			dst = index;
			break;
		case MT_OTHER:
			dst = dbNode::OTHER;
			break;
	}
	*diskType = dt;
	*diskSubType = dst;
	return result;
}
*/


