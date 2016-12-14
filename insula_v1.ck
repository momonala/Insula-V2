<<<"\n", "Welcome to Insula V1. Prepare for a fantasic musical jounrey.", "\n">>>;

//--------Open Serial Connectopn ----------------------------------------------------
SerialIO.list() @=> string list[]; // create array of available serial devices
for (int i; i<list.size(); i++){
    <<<i, ":", list[i] >>>; // print em out 
}

SerialIO cereal; // instance of serial IO to communicate 
cereal.open(0, SerialIO.B115200, SerialIO.ASCII); //open port [port# from list, baud, ASCII or BINARY]
//make sure serial is reading at right positions in array
//read a few lines, check to see if "100" marker is at right position
//blackhole if not

for (0 => int i; i < 5; i++){
    cereal.onInts(15) => now; //reading of current line 
    cereal.getInts() @=>  int line[]; //cast reading to variable 
    //<<<line[0], line[1], line[2], line[3], line[4], line[5], line[6], line[7], line[8], line[9], line[10], line[11], line[12], line[13], line[14] >>>;    
    if (i == 4 && line[0] != 100){
        <<<"poor data stream... replace shred">>>;       
        SinOsc s => blackhole; 
        1::week => now; 
    }
}

0.25 => float onGain; 
me.dir() + "/soundFiles/" => string path;

SndBuf heartBeatDAC => dac;
path + "kick_01.wav" => heartBeatDAC.read; // dir + sound file we want to play

/*SndBuf inhaleDAC => dac;
path + "minor.wav" => inhaleDAC.read; // dir + sound file we want to play

SndBuf exhaleDAC => dac;
path + "major.wav" => exhaleDAC.read; // dir + sound file we want to play
*/

SinOsc inhale => dac;
SinOsc exhale => dac;

/*
//fun void playBreath()

fun void playPulse(int breathInput){
    SndBuf heartBeatDAC => dac;
    me.dir() + "/soundFiles/kick_01.wav" => heartBeatDAC.read; // dir + sound file we want to play
    if  (breathInput == 1){   //if we detect a pulse 
        onGain => heartBeatDAC.gain; //play the note 
        0 => heartBeatDAC.pos; // from beginning 
        500::ms => now;
    }
}

spork ~ playPulse(int pulse);
*/
//===============Infinite Loop =======================================================
while (true){
    cereal.onInts(3) => now; //reading of current line 
    cereal.getInts() @=>  int line[]; //cast reading to variable
    line[0] => int check;
    line[1] => int breath;
    line[2] => int pulse; 
    
   if  (pulse == 1){   //if we detect a pulse 
        //onGain => heartBeatDAC.gain; //play the note 
        //0 => heartBeatDAC.pos; // from beginning 
    }
   /*
    if  (breath == 1){   //if we detect a pulse 
        onGain => inhaleDAC.gain; //play the note 
        50000 => inhaleDAC.pos; // from beginning 
    }
   if  (breath == 0){   //if we detect a pulse 
        onGain => exhaleDAC.gain; //play the note 
        0 => exhaleDAC.pos; // from beginning 
    }
    */
    if  (breath == 1){   //if we detect a pulse 
        0 => exhale.gain;
        onGain => inhale.gain; //play the note 
        300 => inhale.freq ; // from beginning 
    }
   else if  (breath == 0){   //if we detect a pulse 
       0 => inhale.gain; 
       onGain => exhale.gain; //play the note 
        220 => exhale.freq; // from beginning 
    }
    
    1::ms => now;
    <<<breath, pulse>>>;
}