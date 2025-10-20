# Breakout

W Kavanagh & N Merchant. Summer 2024 

## controls

A/D to move the paddle right and left.
P to pause.

## Powerups

big/small paddle (blue)
fast/slow ball (fancy yellow)
fire ball (green)

# Tasklist

## Suggested fixes

* Fix the compiler issues in the code

## Suggested tasks

* Implement mouse input for pad
* Improved VFX (ball trail, paddle hit response, brick particle destruction)
* Better UI (progress bar rather than timer for countdown).
* GameLoop
* Better ball physics (Box2D)
* Leaderboards
* More ball types (e.g., multiball, sticky ball [where you shoot the ball from the paddle every time], tiny ball, big ball, brick-tracking ball)
* Sounds with increasing tone between bounces.
* Implement commentary with calls to an LLM such as LLama

# Time Details and Changelist
<Add information to this section about the time you've taken for this task along with a professional changelist.>

Resolved base-class undefined error caused by the include in PowerupBase.h for PowerupFireBall.h (15 min) 

Bugs and code understanding, minor fixes and looking over the code to grasp the structure 1 hour 

Features: 

Controls: Mouse movment implemented and working 45 min 

Visual Effects: Flickering fireball effect, and a visual change to the fireball powerup so that it is destinct 1 hour 20 min 

Sound Implementation: Sounds for collision with walls the paddle and bricks, and a fire sound effect for the fireball power up. 3 hours

Total 6 hours 20 minutes.