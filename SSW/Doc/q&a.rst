===========
STARSHATTER
===========

Q&A
---
#. First off, a question for those who might not be aware of what we’re
   discussing. What, in your words, exactly is Starshatter?

   Starshatter is a military space combat simulation set in the distant
   future.  Unlike most space sims, Starshatter allows you to directly
   command a wide variety of ships, from agile atmospheric and space-based
   fighters to giant cruisers and fleet carriers.  Starshatter will
   take you through the complete space combat experience – from planet
   surface to interstellar space – with several dynamic campaigns set
   in a persistent simulated universe.


#. When did the idea of Starshatter first come to you, and what brought
   it on?

   The original idea was back in 1996, or thereabouts.  Back then, the
   game was going to be a FPS set aboard space ships and space stations.
   Between the first person levels, there would have been space flight
   sequences that allowed you to pick the next level you would play.
   But there wasn’t going to be any space combat, it was just a
   gimmick to add flexibility to the very rigid format of an FPS game.
   I abandoned that game after just a few months of development, but
   that’s where the game universe and back story were first conceived.

   Several months later, I started fooling around with a first person
   version of Asteroids.  It was going to be a bit like Han Solo’s
   chase through the asteroid field in The Empire Strikes Back.  That was
   where the game engine for what is now Starshatter was originally born.

   Then I got a job with Interplay, and I worked on Starfleet Academy
   for a while, so I had to put the Asteroids clone aside.  While I was
   working on Starfleet, I noticed some of the game design problems that
   it had – how do you simulate large ship combat without boring an
   audience that grew up on Wing Commander and X-Wing?

   After I left Interplay, I put everything together: the universe and
   back story for Starshatter, the engine I had started for the Asteroids
   clone, and a game design that included both large and small ships in
   combat scenarios.

   So, I guess I would have to say that it has really evolved over time.
   There was no single moment where the game just sprang to life.


#. Which games would you say had a direct influence on the idea of
   Starshatter, if any?

   Several!  And each in different ways.  There were many non-space-combat
   games that I liked, such as DOOM and Descent, and I wanted to capture
   them in a space combat setting.  And there were things about many of
   the space combat games, such as Wing Commander and Starfleet Academy
   and Battlecruiser 3000AD that I really wanted to avoid.  And over
   time, the Starshatter game design has come to be influenced by some
   of the excellent hardcore jet fighter simulations such as F22 Total
   Air War and Falcon 4.

   At the same time, there have been some very important influences from
   outside gaming as well.  The back story draws on themes that go back
   to E.E. “Doc” Smith’s Lensman series of space operas, as well
   as more recent works such as Vernor Vinge’s A Fire Upon the Deep.
   Some of the combat scenarios are inspired by David Weber and even
   more by Vinge’s Deepness in the Sky and Dan Simmons first two
   Hyperion novels.

   Graphically and artistically, I’ve been influenced by things like
   Babylon 5 and even Star Wars Episode I.


#. I noticed in the demo that the flight model felt very “Elite-ish”,
   with left and right movements rolling the ship rather than turning
   it. Why was this flight model chosen?

   The control model is driven by the physics model.  Starshatter uses a
   fairly hardcore Newtonian flight model, especially for the atmospheric
   sequences.  When you fly the space fighters in atmosphere, you need
   to perform a coordinated turn, a combination of roll and pitch to use
   the wings to generate lift in the direction you want to go.  After a
   while I got tired of having to press “J” to switch between roll
   and yaw on the joystick, so I just learned to use airplane style
   controls all the time, even in space.


#. I also noticed that, again while playing the demo, that the AI was
   pretty cunning, to the point that it was rather hard to beat. Could you
   go into a bit more detail about the AI present in Starshatter?

   The philosophy behind the AI design is probably unusual for a space
   combat simulation; the AI pilots don’t cheat.  The AI controls
   ships that use the same physics model as the player, and the AI
   is limited to the same kind of control inputs that the player has.
   On each frame, the AI pilots get to move their joystick, throttle,
   and press triggers, just like you do.

   The goal in designing the AI this way was to create AI pilots that
   were just as much fun and just as challenging to fight as real people.
   To help me tune the AI, the game has a mode where the AI controls all
   of the ships, even the player’s.  That way, I can watch through the
   AI pilot’s eyes as he tries to fight through a mission.  As I’m
   watching, I’m constantly evaluating “Does this seem right?
   Is that how I would handle the situation?  Why did he turn that way?
   Why doesn’t he just line up the shot and finish it?”

   The AI is built using a layered design, with low levels being
   responsible for primitive behavior, and higher levels handling
   more abstract reasoning.  At the lowest level, the AI uses a hybrid
   flocking / steering model to turn the ship in the desired direction.
   The next layer up is the tactical layer, which is responsible for
   choosing targets and evaluating threats.  There will actually be at
   least one more layer above that which is responsible for coordinating
   group behavior.

   And if you think the AI is tough now, remember that 1) it isn’t
   done yet and 2) the AI fighters have about half the armor, and half
   the gun power, and one quarter the number of missiles that you have...


#. You’ve been working on this project for a long time. Where, would
   you say, are you in Starshatter’s development cycle, and how long do
   you think it will take to complete?

   Well, I’m getting closer all the time.  It’s hard to project how
   much longer it will take, because it is governed by how much time I
   get to work on it.  I do plan to keep releasing demos and information
   right up until the game is finished.  At the very least, there will
   be another interim release around Christmas 2001.


#. In the demo, we’re treated to a few missions involving space fighter
   battles, capital ship battles, and some ground attacks. What’s the
   feature set we can expect to see in either future installments of the
   demo, or the final version?

   That would be telling!

   Combat-wise, the demo is pretty complete.  There are a few features
   still to add like jammers and stealth.  And I need to finish take-offs
   and landings on planets, as well as launching and docking fighters
   in space.  And of course, there will be ship-to-ship communication
   so that you can order your wingmen about.

   I’m just getting started working on the dynamic campaign system.
   The next demo won’t have any scripted missions in it, they will all
   be generated automatically by the campaign engine.  And the missions
   will be more sophisticated than they are now, with pre-set nav routes
   and more related force packages working together to achieve a goal.

   There are many other cool features that I would like to include if
   time permits.  But I don’t want to promise people anything that
   isn’t core to the design of the game.


#. In the time you’ve spent making this game, what are some of the
   facets of game programming you’ve found that work and don’t work in
   creating a game such as Starshatter?

   *[ I can’t really think of anything interesting.  Would you like to
   substitute a different question here? ]*


#. Will the final release of Starshatter have any editing tools, such
   as a mission/campaign editor, or perhaps a way to edit ships as well?

   Probably.  I imagine that there might be some interest in people doing
   mods to use the game engine for other science fiction settings such
   as Babylon 5 or Space: Above and Beyond, which have more realistic
   flight and combat models.


#. Finally, what’s your philosophy and final goal in creating
   Starshatter?

   My philosophy is that the Wing Commander “interactive movie”
   concept is an evolutionary dead end.  Games that are seduced by
   the ability to wrap up the player’s emotions with actors and a
   character-driven plot become trapped by that ability.  An interactive
   movie can’t show the player anything that hasn’t been scripted
   and recorded before the game even shipped.

   My philosophy is that games should be about giving the player the
   tools and setting in which he can create a new story every time
   the game is played.  That’s why Starshatter is based on a dynamic
   campaign.  The game is all about experiencing new combat scenarios.
   There won’t even be a way to replay “failed” missions (how’s
   that for dynamic tension?).  The story will just keep unfolding with
   you in the middle of it all.

   On a more personal level, my main goal is just to create the best
   game I can and have fun doing it.
