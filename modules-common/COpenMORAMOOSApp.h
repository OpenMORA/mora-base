/* +---------------------------------------------------------------------------+
   |                 Open MORA (MObile Robot Arquitecture)                     |
   |                                                                           |
   |   Copyright (C) 2010-2014  University of Malaga                           |
   |                                                                           |
   |    This software was written by the Machine Perception and Intelligent    |
   |      Robotics (OpenMORA) Lab, University of Malaga (Spain).                  |
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

#ifndef COpenMORAApp_H
#define COpenMORAApp_H

#include <MOOS/libMOOS/App/MOOSApp.h>
#include "CMissionReader2ConfigFile_adaptor.h"
#include <mrpt/utils/CSerializable.h>
#include <mrpt/system/datetime.h>
#include <mrpt/system/filesystem.h>
#include <mrpt/synch/CCriticalSection.h>

/**  A layer over CMOOSApp that adds some extra functionality:
  *  - m_ini: A MRPT-like interface to read the own section of the mission file.
  *  - m_Comms_delayed: Like m_comms, but delaying the actual notifications until the call to IterateBase(), which typically is called at Iterate()
  *  - AddMOOSVariable_OpenMORA(), etc: Like AddMOOSVariable(), etc. but with simplified interface for registering and allowing reading the first value of the var even if it was old when the module starts.
  *
  */
class COpenMORAApp: public CMOOSApp
{

public:
	class CDelayedMOOSCommClient
	{
	public:
		typedef std::list<std::pair<std::string,std::string> > TList;
		typedef std::list<std::pair<std::string,double> > TList2;

		void Notify(const std::string &var, const std::string &val)
		{
			m_cs.enter();
			m_buf.push_back(std::make_pair(var,val));
			m_cs.leave();
		}
		void Notify(const std::string &var, const double valf)
		{
			m_cs.enter();
			m_buf2.push_back(std::make_pair(var,valf));
			m_cs.leave();

		}

		void getBufAndClear(TList &out)
		{
			m_cs.enter();
			out = m_buf;
			m_buf.clear();
			m_cs.leave();
		}
		void getBufAndClear(TList2 &out)
		{
			m_cs.enter();
			out = m_buf2;
			m_buf2.clear();
			m_cs.leave();
		}

	private:
		mrpt::synch::CCriticalSection m_cs;
		TList  m_buf;
		TList2  m_buf2;
	};

protected:
	CMissionReader2ConfigFile_adaptor   m_ini;
	std::list<std::string> 				m_lstTempFilesToDeleteAtExit;
	CDelayedMOOSCommClient				m_Comms_delayed;  //!< \sa IterateBase()

	MOOSVARMAP							m_OpenMORA_subscribedVars;  //!< Used in AddMOOSVariable_OpenMORA and UpdateMOOSVariables_OpenMORA

    /** Add a dynamic (run time) variable, like MOOS' AddMOOSVariable but without the limitation of discarding old ("not fresh") variables the first time they are received.
        @param sName name of the variable
        @param CommsTime - if sSubscribeName is not empty this is the minimum time between updates which you are interested in knowing about, so if CommsTime=0.1 then the maximum update rate you will see on the variable from the DB is 10HZ.
        * \sa RegisterMOOSVariables_OpenMORA, GetMOOSVar_OpenMORA, UpdateMOOSVariables_OpenMORA, AddMOOSVariable_OpenMORA
        */
    bool AddMOOSVariable_OpenMORA(std::string sName,double dfCommsTime)
    {
		CMOOSVariable NewVar(sName,sName,sName,dfCommsTime);
		//does it already exist?
		if(m_OpenMORA_subscribedVars.find(sName)!=m_OpenMORA_subscribedVars.end())
			return false;
		m_OpenMORA_subscribedVars[sName] = NewVar;
		return true;
    }

    /** Register with the DB to be mailed about any changes to any dynamic variables which were created with non-empty sSubscribeName fields
        * \sa RegisterMOOSVariables_OpenMORA, GetMOOSVar_OpenMORA, UpdateMOOSVariables_OpenMORA, AddMOOSVariable_OpenMORA
        */
    bool RegisterMOOSVariables_OpenMORA()
    {
		bool bSuccess = true;

		for(MOOSVARMAP::iterator p = m_OpenMORA_subscribedVars.begin();p!=m_OpenMORA_subscribedVars.end();p++)
		{
			CMOOSVariable & rVar = p->second;
			if(!rVar.GetSubscribeName().empty())
			{
				double dfCommsTime = rVar.GetCommsTime();
				if(dfCommsTime<0) dfCommsTime = 0;
				bSuccess &= m_Comms.Register(rVar.GetSubscribeName(),dfCommsTime);
			}
		}
		return bSuccess;
    }

    /** Pass mail (usually collected in OnNewMail) to the set of dynamic variables. If they are interested (mail name matches their subscribe name) they will update themselves automatically
      *  Like MOOS' UpdateMOOSVariables but for variables registered with AddMOOSVariable_OpenMORA()
	  * \sa RegisterMOOSVariables_OpenMORA, GetMOOSVar_OpenMORA, UpdateMOOSVariables_OpenMORA, AddMOOSVariable_OpenMORA
	  */
    bool UpdateMOOSVariables_OpenMORA(MOOSMSG_LIST & NewMail)
    {
		for(MOOSVARMAP::iterator p = m_OpenMORA_subscribedVars.begin();p!=m_OpenMORA_subscribedVars.end();p++)
		{
			CMOOSVariable & rVar = p->second;
			CMOOSMsg Msg;
			if(m_Comms.PeekMail(NewMail,rVar.GetSubscribeName(),Msg))
			{
				rVar.Set(Msg);
				rVar.SetFresh(true);
			}
		}
		return true;
    }

    /** return a pointer to a named variable
	  * \sa RegisterMOOSVariables_OpenMORA, GetMOOSVar_OpenMORA, UpdateMOOSVariables_OpenMORA, AddMOOSVariable_OpenMORA
      */
	CMOOSVariable * GetMOOSVar_OpenMORA(std::string sName)
	{
		MOOSVARMAP::iterator p =  m_OpenMORA_subscribedVars.find(sName);
		if(p==m_OpenMORA_subscribedVars.end())
				return NULL;
		else	return &(p->second);
	}

public:
    COpenMORAApp() : m_ini(&m_MissionReader)
	{
		::setlocale(LC_ALL,"C");
	}

    virtual ~COpenMORAApp() {
    	for (std::list<std::string>::const_iterator it=m_lstTempFilesToDeleteAtExit.begin();it!=m_lstTempFilesToDeleteAtExit.end();it++)
			mrpt::system::deleteFile(*it);
    }

    bool IsConnectedToMOOSDB() {
    	return m_Comms.IsConnected();
    }

    /** This method should be called at the beginning of Iterate() to deal automatically with m_comms_delayed */
    void IterateBase()
    {
    	CDelayedMOOSCommClient::TList lst;
		CDelayedMOOSCommClient::TList2 milst;


		m_Comms_delayed.getBufAndClear(lst);
    	for (CDelayedMOOSCommClient::TList::const_iterator it=lst.begin();it!=lst.end();++it)
			m_Comms.Notify(it->first,it->second);


    	m_Comms_delayed.getBufAndClear(milst);
    	for (CDelayedMOOSCommClient::TList2::const_iterator it=milst.begin();it!=milst.end();++it)
			m_Comms.Notify(it->first,it->second);
    }

	/** Send a formated string to some central GUI. The format is: "[modulename|time] <TEXT>"
	  */
    inline void SendSystemLogString(const std::string &text )
    {
		m_Comms.Notify(
			"MORA_GLOBAL_LOG",
			mrpt::format("[%s|%s] %s",
				mrpt::system::timeLocalToString(mrpt::system::now(),3).c_str(),
				this->GetAppName().c_str(),
				text.c_str()
				) );
    }

	void SendCommand(const std::string module, const std::string command)
	{
		m_Comms.Notify(module+"_cmd",command);
	}

	/** Deserializes an MPRT object that was sent thru MOOS as a block of binary data with ObjectToOctetVector() */
	mrpt::utils::CSerializablePtr MOOS2MRPT_deserialize(const CMOOSVariable &pVarObs) const
	{
		mrpt::utils::CSerializablePtr obj;
		mrpt::utils::RawStringToObject(pVarObs.GetStringRef(), obj);
		return obj;
	}

};

#endif
