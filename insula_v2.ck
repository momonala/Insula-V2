<<<"\n","--------------------------------------------------------">>>;
<<<" Welcome to Insula. Prepare for a fantasic musical jounrey.", "\n", "--------------------------------------------------------", "\n">>>;
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
    cereal.onInts(16) => now; //reading of current line 
    cereal.getInts() @=>  int line[]; //cast reading to variable 
    //<<<line[0], line[1], line[2], line[3], line[4], line[5], line[6], line[7], line[8], line[9], line[10], line[11], line[12], line[13], line[14], line[15]>>>;    
    if (i == 4 && line[0] != 100){
        <<<"poor data stream... replace shred">>>;       
        SinOsc s => blackhole; 
        1::week => now; 
    }
    else if (i == 4 && line[0] == 100){
        <<<"data stream looks dank. lets go!">>>;
    }
}

me.dir() + "/soundFiles/" => string path; //for playing sound files 
SndBuf startup => dac; //startup sound :)
//path + "E-Mu-Proteus-FX-XtraSlow-C2" + ".wav" => startup.read;
//0 => startup.pos;

//--------Intialize Oscillators ----------------------------------------------------
//STKinstruments: BlowBotl, Flute, Saxofony, Brass, ModalBar, BandedWG, Moog, Rhodey 
//-----FINGERS----- 
0.07 => float fGain; 
StkInstrument f_osc[4];                          //oscillators 
Rhodey inst0 @=> f_osc[0] => NRev fRev0 => dac;
HevyMetl inst1 @=> f_osc[1] => NRev fRev1 => dac;
Flute inst2 @=> f_osc[2] => NRev fRev2 => dac;
Flute inst3 @=> f_osc[3] => NRev fRev3 => dac;
0.35 => fRev0.mix => fRev1.mix => fRev2.mix => fRev3.mix; //set reverb/dry mixture  

//-----ACCEL-----
0.07 => float aGain; 
1 => int accelThreshold;        //which accelVel must cross to register sound 
SinOsc a_inst[3];                             // instruments for accel
for (0 => int i; i < a_inst.cap(); i++){ 
    a_inst[i] => NRev aRev => dac;            //to DAC with reverb                                   
    0.3 => aRev.mix;
}

//-----ORIENT-----
int oldOrient; //intialize previous value 
SndBuf orientChangeDAC => ADSR e => dac;
e.set(1:: second, 0.1::second, 0.5, 0.1::second);
path + "Casio-VZ-10M-Bright-Phaze-C4" + ".wav" => orientChangeDAC.read;
.04 => orientChangeDAC.gain;
//=======================FUNCTIONS ==================================================
//--------Fingers Fuction ----------------------------------------------------
//PICK DESIRED EFFECT 
"click" => string type;
//"smooth" => string type;

fun void fingerScream(int f1, int f2, int f3, int f4){
    [f1, f2, f3, f4] @=> int f[]; //flex values array 
    for (0 => int i; i < f_osc.cap(); i++){
        if (f[i] > 25){
            if (type == "click"){               //clicky effect 
                Std.mtof(aTune(f[i])) => f_osc[i].freq; //autotune, convert midi to freq
            }
            else if (type == "smooth"){          // smooth effect
                 f[i]*2 => f_osc[i].freq;
            }
            1 => f_osc[i].noteOn;             //COMMENT FOR SINOSC   
            fGain => f_osc[i].gain;             //switch on all oscillators  
        }
        else{
            0 => f_osc[i].gain; 
            1 => f_osc[i].noteOff;            //COMMENT FOR SINOSC
        } 
    }
}  

//-----------Accel Function ----------------------------------------------------
fun void accelTwitch(int a1, int a2, int a3, int av1, int av2, int av3){
    [a1,a2,a3] @=> int a[]; //acceleration array 
    [av1,av2,av3] @=> int av[]; //velo of accel array 
    for (0 => int i; i < a.cap(); i++){
        //a_osc[i].Delay(10);
        if (Std.abs(av[0]) > accelThreshold || Std.abs(av[1]) > accelThreshold || Std.abs(av[2]) > accelThreshold){ //if we sense a change in position 
            Std.mtof(aTune(Std.abs(a[i]))) => a_inst[i].freq;
            aGain => a_inst[i].gain;
            //1 => a_inst[i].noteOn;
        }
        else{ 
            0 => a_inst[i].gain;
           // 1 => a_inst[i].noteOff; 
        }
    }
}

fun void orientChange(int orient){
    
    orient => float oriFloat; 

    if (orient != oldOrient){ //if we detect a change in orientation 
        0 => orientChangeDAC.pos; //play the sound from beginning
        1 => e.keyOn;
        
        /*if (orient == 0){ //choose synth rate (note) 
            orient+1 => orientChangeDAC.rate;   
        }
        else if (orient == 1){ 
            orient+1 => orientChangeDAC.rate;
        }
        else if (orient == 2){
            orient+1 => orientChangeDAC.rate;
        }
        else if (orient == 3){
            orient+1 => orientChangeDAC.rate;
        }
        else if (orient == 4){
            orient+1 => orientChangeDAC.rate;
        }*/
        //(oriFloat+1)/4 => orientChangeDAC.rate;
    }
    orient => oldOrient; //reset
}

//DOES NOT WORK...
fun void accelSwoosh(int av1, int av2, int av3){
    10 => int threshold;
    SndBuf accelSwooshDAC => dac;
    me.dir() + "/soundFiles/snare_01.wav" => accelSwooshDAC.read; // dir + sound file we want to play
    if  (av1 > threshold || av2 > threshold || av3 > threshold){  //if we detect a pulse 
        .1 => accelSwooshDAC.gain; //play the note 
        0 => accelSwooshDAC.pos; // from beginning 
        //10::ms => now; 
    }
}

//--------Autotune (C for now) ----------------------------------------------------
[1,3,6,8,10] @=> int offNotes[];//autotune array of out-of-key midi notes, can change the key sig
fun int aTune(int rawNote){
    int aTuneNote;
    rawNote % 12 => int checkNote;
    //check if note (within 8 note scale) is in offNote array 
    if (checkNote == offNotes[0] || checkNote == offNotes[1] || checkNote == offNotes[2] || checkNote == offNotes[3] || checkNote == offNotes[4]){
        rawNote + 1 => aTuneNote; //autotune if off key 
    }
    else {
        rawNote => aTuneNote; //keep if in key 
    }
    return aTuneNote; 
}

//===============Infinite Loop =======================================================
while (true){
    cereal.onInts(16) => now; //reading of current line 
    cereal.getInts() @=>  int line[]; //cast reading to variable
    
    //<<<line[0], line[1], line[2], line[3], line[4], line[5], line[6], line[7], line[8], line[9], line[10], line[11], line[12], line[13], line[14], line[15] >>>;    
    line[0] => int check;
    line[1] => int f1; //for readability 
    line[2] => int v1;
    line[3] => int f2;
    line[4] => int v2;
    line[5] => int f3;
    line[6] => int v3;
    line[7] => int f4;
    line[8] => int v4;
    line[9] => int a1;
    line[10] => int a2;
    line[11] => int a3;
    line[12] => int av1;
    line[13] => int av2;
    line[14] => int av3;
    line[15] => int orient;
   
    //[f1, f2, f3, f4] @=> int f[]; //flex values array  
    //[v1, v2, v3, f4] @=> int v[]; //finger velocities array 
    //[a1,a2,a3] @=> int a[]; //acceleration array
    //[av1,av2,av3] @=> int av[]; //velocity of acceleration array 

    fingerScream(f1, f2, f3, f4);
    accelTwitch(a1, a2, a3, av1, av2, av3);
    //orientChange(orient);
    //accelSwoosh(av1, av2, av3); DOES NOT WORK 
    
    10::ms => now;
}