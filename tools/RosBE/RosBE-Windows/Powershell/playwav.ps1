##################################################################################
#                            PowerShell Invaders                                 #
#                                                                                #
# Released under the Creative Commons                                            #
# Attribution-NonCommercial-ShareAlike 2.5 License                               #
#                                                                                #
# For the full license see : http://creativecommons.org/licenses/by-nc-sa/2.5/   #
#                                                                                #
# Authors:                                                                       #
#                                                                                #
#  Adrian Milliner - ps1@soapyfrog.com (aka millinad)                            #
#  Richy King      - richy@wiredupandfiredup.com                                 #
#  Nik Crabtree    - fido@prophecie.co.uk                                        #
#  Brian Long      - brian@blong.com                                             #
#                                                                                #
# Dipswitches :                                                                  #
#                                                                                #
#  If the variable $PSInvadersDipswitches is set to an integer value it          #
#  represents dipswitches on the PowerShell Invaders arcade game circuit board.  #
#  The variable is setup as a bitmask based on the options you require.          #
#  By default all dispswitches are off, so $PSInvadersDipswitches = 0            #
#  For example, to have a more horrific death sequence for your base station:    #
#                                                                                #
#  $PSInvadersDipswitches = 0x100                                                #
#                                                                                #
# Dispswitch meanings:                                                           #
#                                                                                #
#  Switch 1  (value 0x1)    = infinite lives                                     #
#  Switch 2  (value 0x2)    = reserved for future expansion                      #
#  Switch 3  (value 0x4)    = reserved for future expansion                      #
#  Switch 4  (value 0x8)    = reserved for future expansion                      #
#  Switch 5  (value 0x10)   = reserved for future expansion                      #
#  Switch 6  (value 0x20)   = reserved for future expansion                      #
#  Switch 7  (value 0x40)   = reserved for future expansion                      #
#  Switch 8  (value 0x80)   = reserved for future expansion                      #
#  Switch 9  (value 0x100)  = pimp mode (horrific death sequence, etc)           #
#  Switch 10 (value 0x200)  = reserved for future expansion                      #
#  Switch 11 (value 0x400)  = reserved for future expansion                      #
#  Switch 12 (value 0x800)  = reserved for future expansion                      #
##################################################################################

#Activated DIP Switches
$PSInvadersDipswitches = 0x100

# $Id: psinvaders.ps1 94 2007-01-12 07:33:40Z adrian $

# test for dot sourcing
$global:__gtest__ = 1
$local:__gtest__ = 2
if ($global:__gtest__ -eq 2) { write-error "Don't run this with (.)!"; return }


# set up some script globals 
$script:globs = @{
  #size of console used
  'screenwidth'   = [int]60
  'screenheight'  = [int]30

  # internal borders for movement
  'leftborder'    = [int]1
  'rightborder'   = [int]59
  'topborder'     = [int]1

  'baseline'      = [int]28    # where the base lives
  'motherline'    = [int]2     # where the mothership flies
  'alienline'     = [int]4     # top line for invaders

  # size of each wave of aliens
  'waveheight'    = [int]5
  'wavewidth'     = [int]8
}
$script:rawui = $host.UI.RawUI

# set up some changable script globals
$script:cabinet = @{
  'hiscores'      = [int[]](3000,4000,5000,6000,7000,8000,9000,10000)
  'lastscore'     = [int]0
  'credits'       = [int]0
  'scores'        = [int[]](0,0)    # current player scores
}

# get the dipswitch settings
$script:dipswitches = @{
  'infiniteLives' = ($PSInvadersDipswitches -band 0x1) -eq 0x1
  'switch02' = ($PSInvadersDipswitches -band 0x2) -eq 0x2
  'switch03' = ($PSInvadersDipswitches -band 0x4) -eq 0x4
  'switch04' = ($PSInvadersDipswitches -band 0x8) -eq 0x8
  'switch05' = ($PSInvadersDipswitches -band 0x10) -eq 0x10
  'switch06' = ($PSInvadersDipswitches -band 0x20) -eq 0x20
  'switch07' = ($PSInvadersDipswitches -band 0x40) -eq 0x40
  'switch08' = ($PSInvadersDipswitches -band 0x80) -eq 0x80
  'pimpMode' = ($PSInvadersDipswitches -band 0x100) -eq 0x100
  'switch10' = ($PSInvadersDipswitches -band 0x200) -eq 0x200
  'switch11' = ($PSInvadersDipswitches -band 0x400) -eq 0x400
  'switch12' = ($PSInvadersDipswitches -band 0x800) -eq 0x800
}

# load game sounds
function load-sounds {
  $sounds = @("duh0","duh1","duh2","duh3","baseexplode"
        "firemissile","insertcoin","invaderexplode",
        "mothershiploop","mothershipexplode")
  $script:soundplayers=@{}
  foreach ($s in $sounds) {
    $p = resolve-path "$_ROSBE_BASEDIR\samples\$s.wav" -erroraction "silentlycontinue"
    if ($p) {
      $pm = new-object media.soundplayer ($p.path)
      $pm.load()
      $script:soundplayers[$s] = $pm
    }
    else {
      write-warning "Unable to find sound $s"
    }
  }
}


# play-sample
function play-sound([string]$sample) {
  $pm = $script:soundplayers[$sample]
  if ($pm) { $pm.play() }
}

# function to create a sprite at a specific x,y position with specific image
function create-sprite {
  param([int]$x, [int]$y, [string[]]$img, [string]$clr, [string]$dimg )
  if ($clr -eq $null -or $clr -eq '') { $clr='white' }
  $sprite = @{
    'x'      = $x   
    'y'      = $y 
    'ox'     = -1 
    'oy'     = -1 
    'img'    = $img 
    'eimg'   = ' '*($img[0].length)
    'dimg'   = $dimg
    '_ioff'  = 0
    'state'  = 'alive'
    'colour' = $clr
  }
  return $sprite
}

# draw a sprite, erasing it from it's previous position, if any
function draw-sprite {
  param($sprite)
  if ($sprite.oy -ge 0) {
    [Console]::SetCursorPosition($sprite.ox,$sprite.oy)
    [Console]::Write($sprite.eimg) # this should be constructed based on size of image
  }
  [Console]::SetCursorPosition($sprite.x,$sprite.y)
  $clr=$rawui.foregroundcolor
  $rawui.foregroundcolor=$sprite.colour
  [string]$render = ''
  if ($sprite.state -eq 'alive') { $render = $sprite.img[$sprite._ioff] }
  else { $render = $sprite.dimg; }
  [Console]::Write( $render )
  $rawui.foregroundcolor=$clr
  $sprite._ioff = ($sprite._ioff + 1) % $sprite.img.length
  $sprite.ox = $sprite.x
  $sprite.oy = $sprite.y
}

# erase a sprite if it's been drawn before
function erase-sprite {
  param($sprite)
  if ($sprite.oy -ge 0) {
    [Console]::SetCursorPosition($sprite.ox,$sprite.oy)
    [Console]::Write($sprite.eimg) # this should be constructed based on size of image
  }
}

# move a sprite to an absolute position
function move-sprite {
  param($sprite, [int]$x, [int]$y)
  $sprite.x = $x
  $sprite.y = $y
  draw-sprite $sprite
}

# move a sprite by a relative position
function delta-sprite {
  param($sprite, [int]$x, [int]$y)
  $sprite.x += $x
  $sprite.y += $y
  draw-sprite $sprite
}


# build invader fleet as a block of 8x5
function build-fleet {
  param([int]$level) # starts from 0 (default)
  # at higher levels, the hoard starts a bit lower
  if ($level -gt 4) { $level = 4 }
  $offset = $globs.alienline + $level;
  $invaders = @()
  $clrs='red','blue','green','cyan','white'
  for ($row=$globs.waveheight-1; $row -ge 0; $row--) {
    for ($col=0; $col -lt 8; $col++) {
      $x = 4 + $col * 4
      $y = $offset + $row * 2
      $inv = create-sprite $x $y '/#\','\#/' $clrs[$row] '>X<'
      $invaders += $inv
    }
  }
  return $invaders
}

# wipe screen, oldskool, with a character and a duration in milliseconds
function wipe-game-screen {
  param([int]$duration, [string]$ch)
  if ($duration -le 0) { $duration = 500 }
  if ($ch -eq '') { $ch = ' '}

  $delay = $duration / $globs.screenheight 
  $rawui.backgroundcolor='black'
  $rawui.foregroundcolor='white'
  $wipe=$ch * $globs.screenwidth
  for ($row=0; $row -lt $globs.screenheight; $row++) {
    [Console]::SetCursorPosition(0,$row)
    [Console]::Write($wipe)
    sleep -m $delay
  }
}


# clear the screen for play
function clear-game-screen {
  $rawui.backgroundcolor='darkmagenta'
  cls
}

function draw-centre-string {
  param([int]$row, [string]$txt)
  [int]$x = ( $globs.screenwidth - $txt.length ) / 2 
  [Console]::SetCursorPosition($x,$row); 
  [Console]::Write($txt)
}

function draw-left-string {
  param([int]$row, [string]$txt)
  [Console]::SetCursorPosition(0,$row); 
  [Console]::Write($txt)
}

function draw-right-string {
  param([int]$row, [string]$txt)
  [int]$x = ( $globs.screenwidth - $txt.length ) 
  [Console]::SetCursorPosition($x,$row); 
  [Console]::Write($txt)
}



# draw the top line of the play window
function draw-scores-etc {
  $clr=$rawui.foregroundcolor
  $rawui.foregroundcolor='white'
  draw-left-string 0 ([string]::Format('1UP: {0:00000#}',$cabinet.scores[0]))
  draw-centre-string 0 ([string]::Format('HIGH: {0:00000#}',$cabinet.hiscores[-1]))
  draw-right-string 0 ([string]::Format('CREDITS: {0:0#}',$cabinet.credits))
  $rawui.foregroundcolor=$clr
}





# main func
function play-game {
  wipe-game-screen 300 '*'
  wipe-game-screen 300 ' '
  [int]$seed = [DateTime]::Now.Millisecond
  $random = new-object Random -argumentList $seed

  [int]$level = 10
  [int]$lives = 0
  if($dipswitches.infiniteLives){ 
    $lives = 999 
  } 
  else{ 
    $lives = 2
  }
  [int]$numshots = 0

  # build alien fleet
  $invaders = build-fleet $level
  [int]$fleetsize = $invaders.Count

  # create base
  [int]$basex = 20 + $globs.leftborder
  [int]$basey = $globs.baseline
  $base = create-sprite $basex $basey @('<@>') 'white' 'XXX'

  # create horrificdeathsequence if in pimpMode
  $horrificdeathsequence = @()
  if ($dipswitches.pimpMode) {
    $horrificdeathsequence += create-sprite 0 0 "        <*@*>        " "Red"
    $horrificdeathsequence += create-sprite 0 0 "      . / @ \ .      " "Red"
    $horrificdeathsequence += create-sprite 0 0 "      \ . @ . /      " "Yellow"
    $horrificdeathsequence += create-sprite 0 0 "    / . . * . . \    " "Yellow"
    $horrificdeathsequence += create-sprite 0 0 "    .  .  .  .  .    " "DarkYellow"
    $horrificdeathsequence += create-sprite 0 0 "   .      .      .   " "DarkYellow"
    $horrificdeathsequence += create-sprite 0 0 "  .       .       .  " "DarkRed"
    $horrificdeathsequence += create-sprite 0 0 " .        .        . " "DarkRed"
    $horrificdeathsequence += create-sprite 0 0 ".                   ." "Gray"
    $horrificdeathsequence += create-sprite 0 0 "                     " "DarkGray"
  }
  #missile
  $missile = 0

  #mothership
  $mothership = 0
  $frameToLaunchShip = ($numshots + 20 + $random.Next(5))

  $bombs= @()
  for ([int]$temp=0; $temp -lt 5; $temp++) {
    $bomb = 0
    $bombs += $bomb
  }

  $bases= @()
  for ([int]$temp=0; $temp -lt 24; $temp++){
    $bbit = 0
    $bases += $bbit
  }

#draw the bases
[int]$c = 0
for([int]$temp = 10; $temp -lt 50; $temp += 10){
     for([int]$off = 0; $off -lt 3 ; $off++){
      $bases[$c] = create-sprite ($temp+$off) ($globs.baseline-1) "#"
      draw-sprite $bases[$c]
      $c += 1
      $bases[$c] = create-sprite ($temp+$off) ($globs.baseline-2) "#"
      draw-sprite $bases[$c]
      $c += 1
     }
}

  [int]$dx = 1
  [int]$dy = 0
  [int]$xmax = 0
  [int]$xmin = 0
  [int]$ymax = 0
  [int]$basedx = 0

  [int]$playing = 1
  $cabinet.scores[0] = 0
  [int]$framecount = 0
  [int]$duhcount = 0

  draw-scores-etc

  while ($playing) {
    $xmax = 0; $xmin = 999; $ymax = 0
    [int]$duh = ($duhcount++) % 4
    play-sound "duh$duh"
    foreach ($inv in $invaders) {
      # here's the main game loop
      $framecount++

      # process next alien
      if ($inv.state -eq 'alive') {
        delta-sprite $inv $dx $dy
        # randomly self-destruct
        if ($random.Next(50) -eq 1) {
          for($temp=0;$temp -lt $bombs.length;$temp++){
            if($bombs[$temp] -eq 0){
              $bombs[$temp] = create-sprite ($inv.x+1) ($inv.y+1) ':','+'
              break
            }
          }
        }
      }
      elseif ($inv.state -eq 'dying') {
        erase-sprite $inv 
        $inv.state = 'dead'
        $fleetsize--
        if ($fleetsize -eq 0) { 
	        $level += 1
	        $invaders = build-fleet $level
	        [int]$fleetsize = $invaders.Count
        }
      }
      else { # dead
        continue;
      }
      # move missile if there
      if ($missile -ne 0){
        if($missile.y -eq $globs.topborder){
	      erase-sprite $missile
          $missile = 0
        }else{
          delta-sprite $missile 0 -1
        }
      } 

      # move bombs
      for($temp=0;$temp -lt $bombs.length;$temp++){
        if($bombs[$temp] -ne 0){
          if($bombs[$temp].y -eq $globs.baseline){
            erase-sprite $bombs[$temp]
            $bombs[$temp]= 0
          }else{
            delta-sprite $bombs[$temp] 0 1
          }      
        }      
      }


      #do mothership?
      if($mothership -ne 0){
        if(($mothership.x + 5) -gt $globs.rightborder){
		      erase-sprite $mothership
		      $mothership = 0
          $frameToLaunchShip = ($numshots + 20 + $random.Next(5))
	      }else{
		      delta-sprite $mothership 1 0
	      }
      }else{
	      if ($numshots -eq $frameToLaunchShip){
		      $mothership = create-sprite $globs.leftborder $globs.motherline @('[=O=]') 'green' '//o\\'
        }
      }

      # update the mins and maxes
      $xmax = [Math]::Max($inv.x,$xmax)
      $xmin = [Math]::Min($inv.x,$xmin)
      $ymax = [Math]::Max($inv.y,$ymax)
      # scan keyboard
      # left/right arrow down set basedx to -1/+1 and these are
      # reset on key up
      # a fire event is just set when space key down is registered.
      [int]$dofire = 0;
      while ($rawui.KeyAvailable ) {
        $rk = $rawui.ReadKey('NoEcho,IncludeKeyDown,IncludeKeyUp')
        $k = $rk.VirtualKeyCode
        if ($rk.KeyDown) {
          switch ($k) {
            37 { $basedx = -1; break }
            39 { $basedx = 1; break }
            32 { $dofire = 1; break }
            27 { return 'quit' }
            ([int][char]'C') { 
              if (-not $dipswitches.infiniteLives) { 
                $cabinet.credits++ 
                play-sound "insertcoin"
              }
              draw-scores-etc
              break
            }
          }
        }
        else {
          if ($k -eq 37) { $basedx = 0 }
          elseif ($k -eq 39) { $basedx = 0}
        }
      }

      # fire missile
      if ($dofire -eq 1){
        if($missile -eq 0){
          play-sound "firemissile"
      	  $missile = create-sprite ($basex+1) ($basey-1) @('|')
          $numshots += 1
        }
      }

      # move the base
      $basex += $basedx;
      if ($basex -lt $globs.leftborder) { $basex = $globs.leftborder; $basedx = 0 }
      elseif ($basex -ge ( $globs.rightborder - 3 ) ) { $basex = $globs.rightborder - 3; $basedx = 0 }
      move-sprite $base $basex $basey

      # and check for collisions, change state to 'dying' etc for next loop
      # did we hit an alien?
      if($missile -ne 0){
	      foreach ($inv in $invaders){
          if($inv.state -ne 'dead' -and $inv.state -ne 'dying' -and $inv.y -eq $missile.y -and ($missile.x -ge ($inv.x) -and $missile.x -lt ($inv.x+3))){
            play-sound "invaderexplode"
            $inv.state = 'dying'
            erase-sprite $missile
            draw-sprite $inv
            $missile = 0
            $cabinet.scores[0] += 100
            draw-scores-etc 
          }
        }
      
	      #oo what about a mothership
	      if($mothership -ne 0){
	        if($missile.y -eq $mothership.y -and ($missile.x -ge ($mothership.x) -and $missile.x -lt ($mothership.x+5))){
	  	      $mothership.state = 'dying'
		        erase-sprite $missile
		        draw-sprite $mothership
		        $missile = 0
		        $cabinet.scores[0] += $random.Next(5) * 1000
		        draw-scores-etc
		        erase-sprite $mothership
	 	        $mothership = 0
	          }
	        }
        }
        #did we hit any bases?
        if($missile -ne 0){
        for($basecount=0; $basecount -lt $bases.length; $basecount++){
             if($bases[$basecount] -ne 0){
	      if($missile.y -eq $bases[$basecount].y -and $missile.x -eq $bases[$basecount].x){
                erase-sprite $missile
		$missile=0
                erase-sprite $bases[$basecount]
		$bases[$basecount]=0
	      }
             }
            }
        
        }
        #did the aliens hit any bases
for($basecount=0; $basecount -lt $bases.length; $basecount++){
             if($bases[$basecount] -ne 0){
        for($temp=0;$temp -lt $bombs.length;$temp++){
          if($bombs[$temp] -ne 0){
            
	      if($bombs[$temp].y -eq $bases[$basecount].y -and $bombs[$temp].x -eq $bases[$basecount].x){
                erase-sprite $bombs[$temp]
		$bombs[$temp]=0
                erase-sprite $bases[$basecount]
		$bases[$basecount]=0
	      }
             }
            }
          }
        }        

        # and did any aliens hit us?
        $isdead = $false
        for($temp=0;$temp -lt $bombs.length;$temp++){
          if($bombs[$temp] -ne 0){
            if($bombs[$temp].y -eq $globs.baseline-1 -and ($bombs[$temp].x -ge ($basex) -and $bombs[$temp].x -lt ($basex+3))){
                play-sound "baseexplode"
                $isdead = $true
		if ($dipswitches.pimpMode){
                $deathx=$basex-(($horrificdeathsequence[0].eimg.length-$base.img.length)/2)
                $ferocity = 20
                foreach ($deathframe in $horrificdeathsequence) {
		 	            move-sprite $deathframe $deathx $basey
                  sleep -m $ferocity
                  $ferocity = $ferocity*1.5
                }
              }
              else{
                for($flash=0;$flash -lt 10; $flash++){
		              $base.state = 'dying'
                              draw-sprite $base
		              sleep -m 20
		              $base.state = 'alive'
		              draw-sprite $base
		              sleep -m 20
		            }
              }
	            if($lives -gt 0){
	              if (-not $dipswitches.infiniteLives) { $lives -= 1 }
                $basex = (20 + $globs.leftborder)
	              move-sprite $base  $basex $basey
	            }else{
                $playing=0
                break
	            }
            }      
          }      
        }
	

	#if dead remove bombs...
        if($isdead){
	  for($temp=0;$temp -lt $bombs.length;$temp++){
	   if($bombs[$temp] -ne 0){
            erase-sprite $bombs[$temp]
            $bombs[$temp]= 0
           }
          }
	}

    
        sleep -m 10
      } 
      # end of main game loop (invader drawing block)
      # everything below is done once per movement wave

      # determine if the block as hit the edges, and if so change direction
      if ($ymax -ge $globs.baseline) { $playing=0; break};
      if ($dx -eq 1) { 
        if ($xmax -ge ( $globs.rightborder - 3 ) ) { $dx = 0; $dy = 1 }
      }
      elseif ($dx -eq -1) {
        if ($xmin -lt $globs.leftborder ) { $dx = 0; $dy = 1 }
      }
      elseif ($dx -eq 0) {
        if ($xmin -lt $globs.leftborder) {
          $dx = 1;
        }
        else {
          $dx = -1;
        }
        $dy = 0;
      }

    }
    # game is over here
  }

  # game intro sequence.. shows high score table, collects credits
  # and waits for user to quit or choose one player game.
  # returns 'quit' if prog should end, or '1up' if player 1 should start
  
function play-intro {
  wipe-game-screen 300 '*'; wipe-game-screen 300 ' '

  [string]$icmsg = 'INSERT COIN'
  [string]$icblank = (' ' * $icmsg.length)

  [int]$framecount = 0
  [int]$seqduration = 400

  # max out credits if infiniteLives switch is on 
  if ($dipswitches.infiniteLives) { $cabinet.credits = 99 }
    
  while ($true) {

    # show title
    $rawui.foregroundcolor='white'
    draw-centre-string 5 'POWERSHELL INVADERS'

    # different things get drawn in the middle, depending on sequence
    $rawui.foregroundcolor='green'
    $sequence = ($framecount % $seqduration)

    if ($sequence -eq ($seqduration/2) -or $sequence -eq 0) {
      # clear area on sequence change
      $blank = ' ' * $globs.screenwidth
      for ([int]$i = 7; $i -lt 20; $i++) { draw-centre-string $i $blank }
    }

    if ($sequence -lt ($seqduration/2)) {
      # high score table
      draw-centre-string 8 'HIGH SCORE TABLE'
      for($i = 0; $i -lt $cabinet.hiscores.Count; $i++) {
        [string]$str = [string]::Format('{0} . . . . . {1:00000#}',(1+$i),$cabinet.hiscores[-1-$i])
        draw-centre-string (10+$i) $str
      }
    }
    else {
      # score advance table
      draw-centre-string 8 'SCORE ADVANCE TABLE'
      draw-centre-string 11 '[=O=]  . . . . MYSTERY'
      draw-centre-string 13 '/#\  . . . . . . . 100'
    }

    $rawui.foregroundcolor='white'
    draw-centre-string 21 'PRESS C FOR CREDIT - 1 FOR PLAYER 1 - ESC TO QUIT' 

    draw-scores-etc

    while ( $rawui.KeyAvailable ) {
      $rk = $rawui.ReadKey('NoEcho,IncludeKeyDown,IncludeKeyUp')
      $k = $rk.VirtualKeyCode
      if ($rk.KeyDown) {
        # check for C key to add a credit
        if (-not $dipswitches.infiniteLives -and $k -eq 67) {
          $cabinet.credits++ 
          play-sound "insertcoin"
        }
        # check for ESC key to quit
        elseif ($k -eq 27) {
          return 'quit'
        }
        # check for '1' key to start player 1
        elseif ($k -eq 49 -and $cabinet.credits -gt 0) {
          if (-not $dipswitches.infiniteLives) { $cabinet.credits-- }
          return '1up'
        }
      }
    }

    $rawui.foregroundcolor='red'
    if ($cabinet.credits -eq 0 -and ($framecount % 20 -lt 10)) {
      draw-centre-string 23  $icmsg : $icblank
    }
    else {
      draw-centre-string 23 $icblank
    }

    sleep -m 20 # 50 times a second loop
    $framecount++
  }

}

# outro sequence when the game ends (due to death, usually)
function play-outro {
  [int]$y = ($globs.screenheight / 2)
  $rawui.foregroundcolor='yellow'
  draw-centre-string ($y++) '                     '
  draw-centre-string ($y++) '  G A M E   O V E R  '
  draw-centre-string ($y++) '                     '
  $rawui.foregroundcolor='white'
  # add a score, sort it, remove smallest
  $cabinet.hiscores += $cabinet.scores[0]
  [array]::Sort( $cabinet.hiscores )
  $cabinet.hiscores = $cabinet.hiscores[1..($cabinet.hiscores.Count-1)]  
  sleep 2
}

# program kicks off here
load-sounds
$Script:oldBClr = $rawui.backgroundcolor
$Script:oldFClr = $rawui.foregroundcolor
$Script:oldCursorSize = $rawui.CursorSize
$rawui.CursorSize = 0
$Script:oldTitle = $rawui.WindowTitle
$rawui.WindowTitle = 'playwav'
clear-game-screen
while ($true) {
  $action = ''
  $action = play-intro
  if ($action -eq 'quit') { break }
  elseif ($action -eq '1up') {
    $action = play-game
    $action = play-outro
  }
}
$rawui.WindowTitle = $oldTitle
$rawui.CursorSize = $oldCursorSize
$rawui.backgroundcolor = $oldBClr
$rawui.foregroundcolor = $oldFClr
cls
'Bye bye'


######################################
# Issues
#
# 1) Need support for entering scores
# 2) Need support for persisting scores
#
######################################
