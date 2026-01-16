#include <gtest/gtest.h>

#include <Clock.h>
#include <Stardate.h>

namespace ng = starshatter::engine;


TEST(StarsEx, MissionTimeIsGameTime)
{
	ASSERT_TRUE(Clock::Init());
	auto* clock = Clock::GetInstance();
	EXPECT_NEAR(0.0l, ng::MissionTime(), 0.001l);
	clock->SkipGameTime(0.05);
	EXPECT_NEAR(0.05l, ng::MissionTime(), 0.001l);
	clock->SkipGameTime(0.05);
	EXPECT_NEAR(0.10l, ng::MissionTime(), 0.001l);
	Clock::Close();
}


TEST(StarsEx, OperationTimeIsMissionOffset)
{
	ASSERT_TRUE(Clock::Init());
	auto* clock = Clock::GetInstance();
	EXPECT_NEAR(0.0l, ng::OperationTime(), 0.001l);
	ng::SetMissionStart(1000.0l);
	EXPECT_NEAR(1000.0l, ng::OperationTime(), 0.001l);
	clock->SkipGameTime(0.05);
	EXPECT_NEAR(1000.05l, ng::OperationTime(), 0.001l);
	ng::SetMissionStart(1000.0l, true);
	EXPECT_NEAR(2000.05l, ng::OperationTime(), 0.001l);
	Clock::Close();
}


TEST(StarsEx, CurrentTimeIsOperationAndEpoch)
{
	ASSERT_TRUE(Clock::Init());
	auto* clock = Clock::GetInstance();
	EXPECT_NEAR(ng::Epoch(), ng::CurrentTime(), 0.001l);
	ng::SetOperationStart(1000.0l);
	EXPECT_NEAR(ng::Epoch() + 1000.0l, ng::CurrentTime(), 0.001l);
	ng::SetMissionStart(500.0l);
	EXPECT_NEAR(ng::Epoch() + 1500.0l, ng::CurrentTime(), 0.001l);
	clock->SkipGameTime(0.05);
	EXPECT_NEAR(ng::Epoch() + 1500.05l, ng::CurrentTime(), 0.001l);
	ng::SetOperationStart(1000.0l, true);
	EXPECT_NEAR(ng::Epoch() + 2500.05l, ng::CurrentTime(), 0.001l);
	Clock::Close();
}


TEST(StarsEx, FormatOperationTime)
{
	ASSERT_TRUE(Clock::Init());
	auto* clock = Clock::GetInstance();
	EXPECT_EQ("Day 1, 00:00:00", ng::OperationTime().Format());
	Clock::Close();
}
