/* +---------------------------------------------------------------------------+
   |                 Open MORA (MObile Robot Arquitecture)                     |
   |                                                                           |
   |   Copyright (C) 2010-2014  University of Malaga                           |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics (MAPIR) Lab, University of Malaga (Spain).                  |
   |    Contact: Jose-Luis Blanco  <jlblanco@ctima.uma.es>                     |
   |                                                                           |
   |  This file is part of the MORA project.                                   |
   |                                                                           |
   |     MORA is free software: you can redistribute it and/or modify          |
   |     it under the terms of the GNU General Public License as published by  |
   |     the Free Software Foundation, either version 3 of the License, or     |
   |     (at your option) any later version.                                   |
   |                                                                           |
   |   MORA is distributed in the hope that it will be useful,                 |
   |     but WITHOUT ANY WARRANTY; without even the implied warranty of        |
   |     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         |
   |     GNU General Public License for more details.                          |
   |                                                                           |
   |     You should have received a copy of the GNU General Public License     |
   |     along with MORA.  If not, see <http://www.gnu.org/licenses/>.         |
   |                                                                           |
   +---------------------------------------------------------------------------+ */

#ifndef CMissionReader2ConfigFile_adaptor_H
#define CMissionReader2ConfigFile_adaptor_H

#include <MOOS/libMOOS/App/MOOSApp.h>
#include <mrpt/version.h>
#include <mrpt/utils/CConfigFileBase.h>

/**  A wrapper class that takes a MOOS CProcessConfigReader and shows the interface of as MRPT CConfigFileBase.
  */
class CMissionReader2ConfigFile_adaptor : public mrpt::utils::CConfigFileBase
{
private:
	CProcessConfigReader  *m_pcr;
	bool m_use_section_names;

public:
    CMissionReader2ConfigFile_adaptor(CProcessConfigReader  *pcr ) :
		m_pcr(pcr), m_use_section_names(false)
	{
	}

    virtual ~CMissionReader2ConfigFile_adaptor()
    {
    }

    void enableSectionNames(bool enable=true) {m_use_section_names=enable;}
    void disableSectionNames() {m_use_section_names=false;}

protected:
	/** A virtual method to write a generic string.
	  */
	virtual void  writeString(const std::string &section,const std::string &name, const std::string &str)
	{
	}

	/** Returns a list with all the section names.
	  */
	virtual void getAllSections( mrpt::vector_string	&sections ) const
	{
		sections.clear();
	}

    /** Returs a list with all the keys into a section. */
#if MRPT_VERSION>=0x120
    virtual void getAllKeys( const std::string &section, mrpt::vector_string	&keys ) const
#else
    virtual void getAllKeys( const std::string section, mrpt::vector_string	&keys ) const
#endif
	{
        keys.clear();
        throw std::runtime_error("CMissionReader2ConfigFile_adaptor::getAllKeys: Not implemented.");
    }

	/** A virtual method to read a generic string.
	 * About the parameter "section":
	 *		- If m_use_section_names=true -> empty to read from this current app config block; otherwise will read from the given block.
	 *		- If m_use_section_names=false (default) -> section is ignored and all reads are done for the current application config block.
	 * \exception std::exception If the key name is not found and "failIfNotFound" is true. Otherwise the "defaultValue" is returned.
	 */
	virtual std::string  readString(
		const std::string &section,
		const std::string &name,
		const std::string &defaultStr,
		bool failIfNotFound = false) const
	{
		std::string sect;
		if (section.empty() || !m_use_section_names)
		     sect = m_pcr->GetAppName();
		else sect = section;

		std::string  s;
		if (m_pcr->GetConfigurationParam(sect,name,s))
				return s;
		else
		{
			if (failIfNotFound)
				THROW_EXCEPTION_CUSTOM_MSG1("Mandatory field '%s' not found in MOOS mission file",name.c_str());
			return defaultStr;
		}
	}
};

#endif
