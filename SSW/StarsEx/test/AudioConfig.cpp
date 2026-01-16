#include <gtest/gtest.h>

#include <AudioConfig.h>


TEST(StarsEx, AudioConfig)
{
	AudioConfig audio;  // as a side-effect constructor sets the static instance
	ASSERT_EQ(&audio, AudioConfig::GetInstance());
	audio.SetMenuMusic(100);
	ASSERT_EQ(0, AudioConfig::MenuMusic());
	audio.SetMasterVolume(50);
	ASSERT_EQ(-2500, AudioConfig::MenuMusic());
	audio.SetMusicVolume(50);
	ASSERT_EQ(-3750, AudioConfig::MenuMusic());
	audio.SetMenuMusic(0);
	ASSERT_EQ(-5000, AudioConfig::MenuMusic());
	audio.SetEfxVolume(100);
	ASSERT_EQ(-2500, AudioConfig::EfxVolume());
	audio.SetMasterVolume(100);
	ASSERT_EQ(0, AudioConfig::EfxVolume());
	audio.SetWorldVolume(20);
	ASSERT_EQ(-4000, AudioConfig::EfxVolume());
	audio.SetEfxVolume(50);
	ASSERT_EQ(-4500, AudioConfig::EfxVolume());
}
