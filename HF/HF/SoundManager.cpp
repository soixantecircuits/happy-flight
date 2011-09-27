using namespace std;

#include "SoundManager.h"
#include "SDL.h"
#include "SDL_audio.h"

SoundManager::SoundManager()
{

}

SoundManager::~SoundManager()
{
	SDL_CloseAudio();
}

void mixaudio(void *unused, Uint8 *stream, int len)
{
	int i;
	Uint32 amount;
	SoundManager* pSM = SoundManager::GetInstance();

	for ( i=0; i<NUM_SOUNDS; ++i )
	{
		if( pSM->m_oSounds[i].bPaused )
			continue;
		amount = (pSM->m_oSounds[i].dlen-pSM->m_oSounds[i].dpos);
		if ( amount > (Uint32)len )
		{
			amount = len;
		}
		SDL_MixAudio(stream, &pSM->m_oSounds[i].data[pSM->m_oSounds[i].dpos], amount, SDL_MIX_MAXVOLUME);
		pSM->m_oSounds[i].dpos += amount;
		if( pSM->m_oSounds[i].bLoop && pSM->m_oSounds[i].dpos == pSM->m_oSounds[i].dlen )
			pSM->m_oSounds[i].dpos = 0;
	}
}

void SoundManager::Init()
{
	SDL_AudioSpec fmt;

	/* Un son sereo de 16 bits à 22kHz */
	fmt.freq = 22050;
	fmt.format = AUDIO_S16;
	fmt.channels = 2;
	fmt.samples = 512;        /*Une bonne valeur pour les jeux */
	fmt.callback = mixaudio;
	fmt.userdata = NULL;

	/* Ouvre le contexte audio et joue le son */
	if ( SDL_OpenAudio(&fmt, NULL) < 0 ) {
		//fprintf(stderr, "Impossible d'accéder à l'audio: %s\n", SDL_GetError());
		exit(1);
	}
	SDL_PauseAudio(0);
}

SoundSample* SoundManager::LoadWav( std::string fn )
{
	std::map<std::string, SoundSample* >::iterator pos = m_oSoundDB.find( fn );
	if ( pos != m_oSoundDB.end() )
	{
		return pos->second;
	}

	SDL_AudioSpec wave;
	Uint8 *data;
	Uint32 dlen;
	SDL_AudioCVT cvt;

	/* Load the sound file and convert it to 16-bit stereo at 22kHz */
	if ( SDL_LoadWAV(fn.c_str(), &wave, &data, &dlen) == NULL )
	{
		//fprintf(stderr, "Couldn't load %s: %s\n", file, SDL_GetError());
		return NULL;
	}

	SDL_BuildAudioCVT(&cvt, wave.format, wave.channels, wave.freq, AUDIO_S16, 2, 22050);
	cvt.buf = (Uint8*)malloc(dlen*cvt.len_mult);
	memcpy(cvt.buf, data, dlen);
	cvt.len = dlen;
	SDL_ConvertAudio(&cvt);
	SDL_FreeWAV(data);

	SoundSample* pSample = new SoundSample;
	pSample->data = cvt.buf;
	pSample->dlen = cvt.len_cvt;

	m_oSoundDB[ fn ] = pSample;

	return pSample;
}

void SoundManager::Play( std::string fn, bool bLoop, bool bPaused )
{
	int index;

	/* Look for an empty (or finished) sound slot */
	for ( index=0; index<NUM_SOUNDS; ++index )
	{
		if ( m_oSounds[index].dpos == m_oSounds[index].dlen )
		{
			break;
		}
	}
	if ( index == NUM_SOUNDS )
		return;

	SoundSample* pSample = LoadWav( fn );

	/* Put the sound data in the slot (it starts playing immediately) */
	SDL_LockAudio();
	m_oSounds[index].data = pSample->data;
	m_oSounds[index].dlen = pSample->dlen;
	m_oSounds[index].dpos = 0;
	m_oSounds[index].bLoop = bLoop;
	m_oSounds[index].bPaused = bPaused;
	m_oSounds[index].sName = fn;
	SDL_UnlockAudio();
}

void SoundManager::Stop( std::string fn )
{
	for ( int i=0; i<NUM_SOUNDS; ++i )
	{
		if( m_oSounds[i].sName == fn )
		{
			m_oSounds[i].bLoop = false;
			m_oSounds[i].dpos = m_oSounds[i].dlen;
		}
	}
}

void SoundManager::Pause( std::string fn, bool bPause )
{
	for ( int i=0; i<NUM_SOUNDS; ++i )
	{
		if( m_oSounds[i].sName == fn )
			m_oSounds[i].bPaused = bPause;
	}
}
