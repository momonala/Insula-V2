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
//STKinstruments: BlowBotl, Saxofony, Brass, ModalBar, BandedWG, Moog, Rhodey. HevyMetl 
//-----FINGERS----- 
0.005 => float fGain; 
10 => int fThresh; 
NRev fRev; 
0.2 => fRev.mix;
true => int orientOn; //turn on/off orientation detector on fingers 

StkInstrument f_osc[4];
//oscillators for orient off feature
//you can select an STKinst for each finger, it will not change with orient 
Rhodey inst0 @=> f_osc[0] => fRev => dac;
Rhodey inst1 @=> f_osc[1] => fRev => dac;
Rhodey inst2 @=> f_osc[2] => fRev => dac;
Rhodey inst3 @=> f_osc[3] => fRev => dac;

//oscillator array for each orientation 
//same inst on each finger, will change with orientation
HevyMetl f_osc0[4]; 	//fingers up
Rhodey f_osc1[4];       //fingers down
Saxofony f_osc2[4];     //palm left
ModalBar f_osc3[4];     //palm right 
BlowBotl f_osc4[4];     //palm down

for (0 => int i; i < 4; i++){ 
    f_osc0[i] => fRev => dac; //connect each to DAC, with reverb
    f_osc1[i] => fRev => dac;        
    f_osc2[i] => fRev => dac;
    f_osc3[i] => fRev => dac;
    f_osc4[i] => fRev => dac;
    
}

//-----ACCEL-----
0.2 => float aGain; 
1 => int accelThreshold;        //thresh accelVel must cross to register sound 
SinOsc a_inst[3];                             // instruments for accel
for (0 => int i; i < a_inst.cap(); i++){ 
    a_inst[i] => NRev aRev => dac;            //to DAC with reverb                                   
    0.2 => aRev.mix;
}

//=======================FUNCTIONS ==================================================
//--------Fingers Fuction ----------------------------------------------------
fun void fingerScream(int f1, int f2, int f3, int f4, int orient){
    [f1, f2, f3, f4] @=> int f[]; //flex values array 
    for (0 => int i; i < f_osc.cap(); i++){
        if (f[i] > fThresh){            //thresh avoid constant low freq's     
            if (orientOn == true){
                if (orient == 0){
                    Std.mtof(aTune(f[i])) => f_osc0[i].freq;
                    1 => f_osc0[i].noteOn; 
                    fGain => f_osc0[i].gain;
                }
                else if (orient == 1){ 
                    Std.mtof(aTune(f[i])) => f_osc1[i].freq;
                    1 => f_osc1[i].noteOn;
                    fGain => f_osc1[i].gain;
                }
                else if (orient == 2){ 
                    Std.mtof(aTune(f[i])) => f_osc2[i].freq;
                    1 => f_osc2[i].noteOn;
                    fGain => f_osc2[i].gain;
                }
                else if (orient == 3){
                    Std.mtof(aTune(f[i])) => f_osc3[i].freq;
                    1 => f_osc3[i].noteOn;
                    fGain => f_osc3[i].gain;
                }
                else if (orient == 4){
                    Std.mtof(aTune(f[i])) => f_osc4[i].freq;
                    1 => f_osc4[i].noteOn;
                    fGain => f_osc4[i].gain;
                }
            }
            else if(orientOn == false){ 
                Std.mtof(aTune(f[i])) => f_osc[i].freq; //autotune, convert midi to freq
                1 => f_osc[i].noteOn;             //COMMENT FOR SINOSC   
                fGain => f_osc[i].gain;           //switch on all oscillators 
            }       
             
        }
        else{ //if thresh not reached, turn osc's off 
            0 => f_osc0[i].gain => f_osc1[i].gain => f_osc2[i].gain => f_osc3[i].gain => f_osc4[i].gain; 
            1 =>  f_osc0[i].noteOff => f_osc1[i].noteOff => f_osc2[i].noteOff => f_osc3[i].noteOff => f_osc4[i].noteOff;         //COMMENT FOR SINOSC
        } 
    }
}  

//-----------Accel Function ----------------------------------------------------
fun void accelTwitch(int a1, int a2, int a3, int av1, int av2, int av3){
    [a1,a2,a3] @=> int a[]; //acceleration array 
    [av1,av2,av3] @=> int av[]; //velo of accel array 
    for (0 => int i; i < a.cap(); i++){
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

    accelTwitch(a1, a2, a3, av1, av2, av3);
    fingerScream(f1, f2, f3, f4, orient);
    
    10::ms => now;
}