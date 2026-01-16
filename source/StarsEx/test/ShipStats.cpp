#include <gtest/gtest.h>

#include <Clock.h>
#include <SimEvent.h>


TEST(StarsEx, EffectivePoints)
{
	Clock::Init();
	ShipStats::Initialize();
	auto* first = ShipStats::Find("first");
	ASSERT_NE(nullptr, first);
	EXPECT_EQ(0, first->EffectivePoints());
	first->SetFlightTime(590.0l);
	EXPECT_EQ(0, first->EffectivePoints());
	first->SetFlightTime(610.0l);
	EXPECT_EQ(10, first->EffectivePoints());
	first->SetFlightTime(1210.0l);
	EXPECT_EQ(20, first->EffectivePoints());
	const auto* destroyed = first->AddEvent(SimEvent::DESTROYED);
	first->Summarize();
	EXPECT_EQ(0, first->EffectivePoints());
	auto* second = ShipStats::Find("second");
	ASSERT_NE(nullptr, second);
	const auto* docked = second->AddEvent(SimEvent::DOCK);
	EXPECT_EQ(100, second->EffectivePoints());
	ShipStats::Close();
	Clock::Close();
}
