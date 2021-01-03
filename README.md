# 68k-mac-cursor-keys
This is a system extension (INIT) for Mac System 6.x through 9.x that adds alternate key layout when capslock is engaged.

## What this system extension (INIT) does
When caps lock is down, turns [{ key into left cursor, \| key into right cursor, =+ key into up cursor, ]} key into down cursor. As of version 0.2.0, it essentially disables CapsLock from capitalizing alphabet characters. This allows the user to leave CapsLock on all the time, only turning it off when a [, ], \ or = key press is needed. 

```
       +------+
       | +  = |
       |      |
       |  UP  |
+--------------------+
| [  { | ]  } | \  | |
|      |      |      |
| LEFT | DOWN | RIGHT|
+--------------------+
```

## Why you might want this:
You have an original 128/512 Mac keyboard and wish it had cursor keys
You have a Mac Plus keyboard, Apple Desktop Bus Keyboard ("IIGS"), M0116 (II/SE "standard keyboard"), or Apple Keyboard II and wished you had cursor keys in an inverted T arrangement

## How to install this:
1. Get it onto your Mac somehow
2. Expand the .SIT file
3. Drop the INIT/extension on your system folder. For System 7+, the Mac will further drop it into your System Extensions folder

## How to use it:
- Engage caps lock key, and use [, ], \, and = as cursor keys. All other keys will operate as normal (non-capitalized) keys.

## Does it have a Control Panel?
No. Nothing is configurable, in the interest of keeping size to a minimum.

## Which systems is it compatible with?
Good question. So far, it has been tested with: 
- Mac Plus with 128/512 keyboard running 6.0.8 (US)
- Mac Classic with Apple Extended Keyboard running 7.1 (US)
- Sheepshaver (OS X) running 9.0 (Japanese)
If you test it on another system, please let me know if it works or not. I would expect it to not work perfectly for any language/country that has a different arrangement in that part of the keyboard. 

## Will it work in all apps?
No, but it should work in all apps that are well-behaved, and which actually have some kind of cursor-based behavior. I noticed in testing, for example, that MacWrite (pre-II versions) didn't do anything with the cursor input. I tried MacWrite with a Mac that had native cursor keys, they didn't do anything either! Your mileage may vary. 

## Why did you make those keys instead of IJKL?
IJKL does make a better inverted T for cursors, the but problem with that is that you basically can't use the keyboard for any typing at all when you are in that mode. 

## Can it do my homework?
Not currently. All it does is map 4 keys. If you have any idea for other keys, I'm not adverse to mapping more. The only concern I have is that the more keys that get remapped, the more the hit on typing performance might be noticeable. I didn't do any testing though, so it's possible it's not a concern. I'm sure it wouldn't be on anything faster than a bare 68000.