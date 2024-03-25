# FestivalPlayer
## DISCLAIMER
This project was created purely by educational and entertainment purpose. No harm was intended to the game, developers or game community itself.
This project dosen`t do any injection or hooks to the game process.

## Hi!
Hi, that was a small project which was initially started as an helper to pass some of Fortnite Festival challenges. It is important to mention that it primarly targetet to play `**drums**`. I can't figure out how to make it work with long holding music notes.
And it works so greate that I was really curious of what it capable of. Surprisingly this 'little helper' evec made couple of flawless and relatively easy got a top #1000 or #2000 of world leaderboard.
But it really depends on song to play, some works better then another one. Anyway at the average even with mistakes it can easy do 5+ stars at most of the songs. But your mileage may vary.
Have fun to look out source code, download, play with it, or do anything you want.

### Hey,
Here you can [watch the SHOWCASE](https://youtu.be/72h084jQ2EI).

## Settings explained
All settings are inside [FestivalPlayer_Settings.json](https://github.com/Andrii-Berozkin/FestivalPlayer/blob/master/FestivalPlayer_Settings.json). **!__IMPORTANT this json file is only for showcase, the real one will be created at first run by a program where the executable is__!**
### The most important one is -
 - **ReactionDelayMS** - it defines how long it takes to keys be pressed after image capturing and filtering. You want to adjust this value first one because it`s depends on user CPU and overall load on PC. Probably completely unique for everyone.
 - **bEnableDebug** - print or not pressed keys in console.

 - **MusicNoteTargetColor** - target color to find on each (5) sections. Finding this color will trigger button to be pressed. Used in cooperation with 'ColorTolerance'.
 - **ColorTolerance** - how much color spreading allowed to count as TargetColor. Tolerance value applied to each R, G, B individually. For example if target color (10,15,5) and tolerance is (5), this color (15,10,0) or (10,20,5) will return true. And this one (16, 15, 5) will be out of range.

 - CaptureSizeFromTotal
 - **SizeMultiplierX** - specify an width which actually need to be captured. Form an rectangle (work with SizeMultiplierY). For example 3840 px * 0.265 = 1017 px.
 - **SizeMultiplierY** - specify an height which actually need to be captured. Form an rectangle (work with SizeMultiplierX). For example 2160 px * 0.0150 = 32 px.

 - CapturePos
 - **ScreenPercentX** - specify shift by width of capturing area from center of screen. For example 0.5 the capturing will be perfectly centered at screen automatically. !Reccomendede value 0.503 as a Fortnite festival slightly shifted to the right!
 - **ScreenPercentY** - specify shift by height of capturing area from center of screen. For example 0.5 the capturing will be perfectly centered at screen automatically.

 - **MusicNoteSizeXMultiplier** - multiply on capturing width to create proportional sections. As expert mode used 5 lines of notes, recommended value is 0.2.
