#ifndef SOUNDMANAGER_HH
#define SOUNDMANAGER_HH

#include "SDL.h"
#include "Singleton.h"
#include <string>
#include <map>

#define NUM_SOUNDS		32
struct SoundSample
{
	SoundSample()
		: data(NULL)
		, dpos(0)
		, dlen(0)
		, bLoop(false)
		, bPaused(false){}

	Uint8		*data;
	Uint32		dpos;
	Uint32		dlen;
	bool		bLoop;
	bool		bPaused;
	std::string	sName;
};

class SoundManager : public Singleton<SoundManager>
{
	friend class Singleton<SoundManager>;

public:
	SoundManager();
	~SoundManager();

	void Init();

	SoundSample* LoadWav( std::string fn );
	void Play( std::string fn, bool bLoop = false, bool bPaused = false );
	void Stop( std::string fn );
	void Pause( std::string fn, bool bPause );

	SoundSample m_oSounds[NUM_SOUNDS];

private:
	std::map<std::string, SoundSample*> m_oSoundDB;
};


#endif //#ifndef SURFACE_DB_HH
