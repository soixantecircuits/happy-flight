#ifndef __PREFS_MANAGER_H__
#define __PREFS_MANAGER_H__

#include "Singleton.h"
#include <string>
#include <map>

class PrefsManager : public Singleton<PrefsManager>
{
	friend class Singleton<PrefsManager>;

public:
	PrefsManager(){}
	~PrefsManager(){}

	bool LoadPrefs( const std::string& sFileName );
	bool SavePrefs( const std::string& sFileName );

	int GetValue( const std::string& sKey );
	void SetValue( const std::string& sKey, int iValue );

private:
	std::map< std::string, int > m_oValues;
};

#endif  // __PREFS_MANAGER_H__