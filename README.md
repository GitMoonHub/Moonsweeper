# Moonsweeper
A Minesweeper clone programmed using C++ (ISO C++ 14 Standard) and the *Win32 API* for the GUI.


Features include:
- Three difficulty presets
- Individually setting each parameter to create a custom difficulty. 
- Dynamic resizing of the game grid with the window, maximizing the game grid size.
- Choosing a seed for the RNG engine.
- Chording (https://en.wikipedia.org/wiki/Chording#Minesweeper_tactic)
- Timer and a "flags remaining" indicator
- GIMP fueled art by a non-artist 🙂
  
![image](https://github.com/GitMoonHub/Moonsweeper/assets/48960814/aa7d8d33-cbf2-4d23-9ed4-3d3dcef13d59)
![image](https://github.com/GitMoonHub/Moonsweeper/assets/48960814/83cb3927-d895-465c-9425-09f25df533bd)

## Some thoughts
This was the second time I was diving into the Win32 API and I couldn't remember much from my first dip into it. I learned a lot about how Windows handles the GUI of its programs and deepened my knowledge on its message-based system. This was a good toy project because of its simplicity (i.e. no spatial partitioning needed or multithreading) and because I unironically enjoy Minesweeper a lot.
