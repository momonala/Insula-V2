<<<"\n","--------------------------------------------------------">>>;
<<<" Welcome to Insula. Prepare for a fantasic musical jounrey.", "\n", "--------------------------------------------------------", "\n">>>;
//--------Open Serial Connectopn ----------------------------------------------------
SerialIO.list() @=> string list[]; // array of available serial devices
for (int i; i<list.size(); i++){
    <<<i, ":", list[i] >>>;
}

SerialIO cereal; // instance of serial IO 
cereal.open(0, SerialIO.B115200, SerialIO.ASCII); //open port [port# from list, baud, ASCII or BINARY]

//read a few lines, check to see if "100" marker is at right position, blackhole if not
for (0 => int i; i < 5; i++){
    cereal.onInts(16) => now; //reading of current line 
    cereal.getInts() @=>  int line[]; //cast reading to variable 
    if (i == 4 && line[0] != 100){
        <<<"poor data stream... replace shred or check COM port.">>>;       
        SinOsc s => blackhole; 
        1::week => now; 
    }
    else if (i == 4 && line[0] == 100){
        <<<"data stream looks dank. lets go!">>>;
    }
}

//--------Intialize Oscillators ----------------------------------------------------
//STKinstruments: BlowBotl, Saxofony, Brass, ModalBar, BandedWG, Moog, Rhodey. HevyMetl 
//-----FINGERS----- 
0.05 => float fGain; 
30 => int fThresh; 
NRev fRev; 
0.10 => fRev.mix;

StkInstrument f_osc[4]; 
Saxofony inst0 @=> f_osc[0] => fRev => dac;
Saxofony inst1 @=> f_osc[1] => fRev => dac;
Saxofony inst2 @=> f_osc[2] => fRev => dac;
Flute inst3 @=> f_osc[3] => fRev => dac;

//-----ACCEL-----
0.2 => float aGain; 
1 => int accelThreshold;        //thresh accelVel must cross to register sound 
SinOsc a_inst[3];                             // instruments for accel
for (0 => int i; i < a_inst.cap(); i++){ 
    a_inst[i] => NRev aRev => dac;            //to DAC with reverb                                   
    0.2 => aRev.mix;
}

//--------Fingers Fuction ----------------------------------------------------
fun void fingerScream(int f1, int f2, int f3, int f4, int orient){
    [f1, f2, f3, f4] @=> int f[]; //flex values array 
    for (0 => int i; i < f_osc.cap(); i++){
        //thresh avoid constant low freq's
        if (f[i] > fThresh){              
            Std.mtof(aTune(f[i])) => f_osc[i].freq; //autotune, convert midi to freq
            fGain => f_osc[i].gain;           //switch on oscillator
            1 => f_osc[i].noteOn;             //COMMENT FOR SINOSC   
        }
        else{  
            0 => f_osc[i].gain; 
            1 =>  f_osc[i].noteOff;         //COMMENT FOR SINOSC
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
        }
        else{ 
            0 => a_inst[i].gain;
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
    
    <<<line[0], line[1], line[2], line[3], line[4], line[5], line[6], line[7], line[8], line[9], line[10], line[11], line[12], line[13], line[14], line[15] >>>;    
    line[0] => int check;
    line[1] => int f1; 
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

    accelTwitch(a1, a2, a3, av1, av2, av3);
    fingerScream(f1, f2, f3, f4, orient);
    
    10::ms => now;
}