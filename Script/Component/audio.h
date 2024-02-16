#pragma once

#include <xaudio2.h>
#include "../GameObject/gameObject.h"

enum AudioState {
	AUDIODEFAULT = 0,
	AUDIOFADEOUT = 1,
	AUDIOFADEIN=2,
	AUDIOFADEFINISH = 3,
};

class Audio : public Component
{
private:
	static IXAudio2*				m_Xaudio;
	static IXAudio2MasteringVoice*	m_MasteringVoice;

	IXAudio2SourceVoice*	m_SourceVoice{};
	BYTE*					m_SoundData{};

	int						m_Length{};
	int						m_PlayLength{};
	float                   m_Volume = 1.0f;
	AudioState              m_State=AudioState::AUDIODEFAULT;
public:
	
	static void InitMaster();
	static void UninitMaster();

	using Component::Component;

	void Uninit();
	void Update()override;

	void Load(const char *FileName);
	void Play(bool Loop = false);
	void FadeOut() {m_State = AudioState::AUDIOFADEOUT;}
	void FadeIn() { m_State = AudioState::AUDIOFADEIN; m_Volume = 0.0f; }
	void Stop();
	AudioState GetState() { return m_State; }
	void SetVolume(float _volume) { m_Volume = _volume; m_SourceVoice->SetVolume(m_Volume); }
};