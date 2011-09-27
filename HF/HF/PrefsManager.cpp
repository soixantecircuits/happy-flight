#include "PrefsManager.h"
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

bool PrefsManager::LoadPrefs( const string& sFileName )
{
	m_oValues.clear();
	string line;
	ifstream myfile( sFileName.c_str() );
	if( myfile.is_open() )
	{
		while( myfile.good() )
		{
			getline( myfile,line );
			istringstream iss( line );
			string sKey;
			iss >> sKey;
			int iValue;
			iss >> iValue;
			if( sKey != "" )
				m_oValues[ sKey ] = iValue;
		}
		myfile.close();
		return true;
	}
	else cerr << "Unable to open file";
	return false;
}

bool PrefsManager::SavePrefs( const string& sFileName )
{
	ofstream myfile( sFileName.c_str() );
	if( myfile.is_open() )
	{
		for( map< string, int >::iterator it = m_oValues.begin(); it != m_oValues.end(); ++it )
		{
			myfile << it->first << "\t" << it->second << std::endl;
		}
		myfile.close();
	}
		else cerr << "Unable to open file";
	return 0;
}

int PrefsManager::GetValue( const string& sKey )
{
	return m_oValues[ sKey ];
}

void PrefsManager::SetValue( const string& sKey, int iValue )
{
	m_oValues[ sKey ] = iValue;
}
